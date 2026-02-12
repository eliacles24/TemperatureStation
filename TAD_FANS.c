#include "TAD_FANS.h"

#include "TAD_TIMER.h"

char fansState;
unsigned char fansTmr;

void init_FANS()
{
    TRISCbits.TRISC1 = 0;
    TRISCbits.TRISC2 = 0;
    
    fansState = CRITICAL_STATE;
    TI_NewTimer(&fansTmr);
    TI_ResetTics(fansTmr);
    
    FAN0 = 0;
    FAN1 = 0;
    
}

void PWM_FANS()
{   
    switch(fansState){
        case LOW_STATE:
            FAN1 = 0;
            
            if(TI_GetTics(fansTmr) & 0x01){
                FAN0 = 0;
            } else {    
                FAN0 = 1;
            }    
            break;
        case MODERATE_STATE:
            if(TI_GetTics(fansTmr) & 0x01){
                FAN0 = 0;
                FAN1 = 0;
            } else {    
                FAN0 = 1;
                FAN1 = 1;
            }
            break;
        case HIGH_STATE:
            FAN0 = 1;
            FAN1 = 1;
            break;
        case CRITICAL_STATE:
            FAN0 = 0;
            FAN1 = 0;
            break;
    }
    
}

void setFansState(char newState){
    fansState = newState;
}