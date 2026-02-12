#include "TAD_LED.h"

#include "TAD_TIMER.h"

static unsigned char state_LED;
static unsigned char timLed;

void init_LED()
{
    //set pins as output
    TRISAbits.TRISA3 = 0;
    TRISAbits.TRISA4 = 0;
    TRISAbits.TRISA5 = 0;
    
    reset_LEDS();
    
    TI_NewTimer(&timLed);
    
}

void reset_LEDS()
{
    RED = OFF;
    GREEN = OFF;
    BLUE = OFF;
}

void ledVermella() {
    RED = ON;
    GREEN = OFF;
    BLUE = OFF;
}

void ledBlava() {
    RED = OFF;
    GREEN = OFF;
    BLUE = ON;
}

void ledVerda() {
    RED = OFF;
    GREEN = ON;
    BLUE = OFF;
}

void ledLila() {
    RED = ON;
    GREEN = OFF;
    BLUE = ON;
}

void ledBlanca() {
    RED = ON;
    GREEN = ON;
    BLUE = ON;
}

void ledGroga() {
    RED = ON;
    GREEN = ON;
    BLUE = OFF;
}

void ledTurquesa() {
    RED = OFF;
    GREEN = ON;
    BLUE = ON;
}

void motor_LED()
{
    
    static unsigned char i = 0;
    
    switch(state_LED) {
        case 0:
            reset_LEDS();
            break;
        case LOW_TEMP:
            ledVerda();
            break;
        case MOD_TEMP:
            ledBlava();
            break;
        case HIGH_TEMP:
            ledVermella();
            break;
        case CRITICAL_TEMP:
            ledVermella();
            if (TI_GetTics(timLed) >= 100) {
                TI_ResetTics(timLed);
                state_LED++;
            }
            break;
        case 5:
            BLUE = ON;
            if (TI_GetTics(timLed) >= 100) {
                TI_ResetTics(timLed);
                state_LED--;
            }
            break;
    }
}

void recieve_temp(unsigned char temp)
{   
    state_LED = temp;
    
    //if (temp == CRITICAL_TEMP) {
        //TI_ResetTics(timLed);
    //}
    
}