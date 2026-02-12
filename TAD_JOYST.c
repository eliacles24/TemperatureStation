#include "TAD_JOYST.h"

#include "TAD_ADC.h"
#include "TAD_PBJoyst.h"
#include "TAD_UART.h"

#include "TAD_RTC_var.h"
#include "TAD_RAM.h"
#include "TAD_SENDGRAPH.h"

static unsigned char stateJoyst;

void init_JOYSTICK()
{
    stateJoyst = 0;
    
}

void motor_Joyst()
{
    static unsigned char lastDirection = CENTER;
    static unsigned char prevCorrect = CENTER;
    static unsigned char temp = -1;
    unsigned char gotDir;
    
    switch(stateJoyst){
        
        case 0:
            gotDir = getDirectionJoystick();
            
            if (gotDir != lastDirection) {
                
                if (lastDirection == CENTER) {
                    
                    if (gotDir == LEFT) {
                        stateJoyst++;
                    }
                    if (gotDir == RIGHT) {
                        stateJoyst = 2;
                    }
                    if (gotDir == UP) {
                        stateJoyst = 3;
                    }
                    if (gotDir == DOWN) {
                        stateJoyst = 4;
                    }
                
                }
            } else {
                if (getButtonIsPressed())
                    stateJoyst = 5;
            }
              
            if (gotDir != DEAD_ZONE)
                lastDirection = gotDir;
            
            break;
        case 1:
            while(!SIO_TXAvail());
            if (SIO_PutString("LEFT\r\n") == 1) {
                stateJoyst = 0;
            }
            break;
        case 2:
            if (SIO_PutString("RIGHT\r\n") == 1) {
                stateJoyst = 0;
            }
            break;
        case 3:
            if (SIO_PutString("UP\r\n") == 1)
                stateJoyst = 0;
            break;
        case 4:
            if (SIO_PutString("DOWN\r\n") == 1)
                stateJoyst = 0;
            break;
        case 5:
            if (SIO_PutString("CLICK\r\n") == 1) {
                stateJoyst = 0;
            }
            
            break;
        
        
    }
    
    
}