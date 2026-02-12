#include <xc.h>
#include "pic18f4321.h"

#pragma config OSC = HSPLL
#pragma config PBADEN = DIG	
#pragma config MCLRE = ON
#pragma config DEBUG = OFF
#pragma config PWRT = OFF
#pragma config BOR = OFF
#pragma config WDT = OFF
#pragma config LVP = OFF

#include "TAD_UART.h"
#include "TAD_TIMER.h"
#include "TAD_ADC.h"
#include "TAD_JOYST.h"
#include "TAD_PBJoyst.h"
#include "TAD_FANS.h"
#include "TAD_LED.h"
#include "TAD_TMP_manager.h"
#include "TAD_JAVA.h"
#include "TAD_RTC_var.h"
#include "TAD_I2C.h"
#include "TAD_RAM.h"
#include "TAD_SENDGRAPH.h"
#include "TAD_EEPROM.h"
#include "TAD_UPD_TIME.h"

void __interrupt(high_priority) MyRSI(void){
    if (TMR0IF == 1){
        RSI_Timer0();
    }
}

void resetVars() {
    
    TI_Init();
    initEusart();
    initADC();
    init_JOYSTICK();
    init_PBJoyst();
    init_FANS();
    init_LED();
    initTMPMan();
    init_RTC();
    InitI2C();
    init_RAM();
    init_EEPROM();
    intiTimeUpdate();
    setRTCAlarms();
    
}

void main(void) {
    
    resetVars();
   
    
    while (1) {
        
        motor_JAVA();
        motorADC();
        motorPBJoyst();
        motor_Joyst();
        PWM_FANS();
        motor_LED();
        motorTemp();
        motor_sendGraph();
        timeUpdateMotor();
        
    }

}