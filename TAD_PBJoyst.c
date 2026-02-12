#include "TAD_PBJoyst.h"

#include "TAD_TIMER.h"

static unsigned char PB;
static unsigned char PBtimer = 0;

void init_PBJoyst() 
{
    defButton = 1;
    Button = PB_OFF;
    INTCON2bits.RBPU = 0;   //Pull ups in port B
    char trahs;
    PB = 0;
    
    TI_NewTimer(&PBtimer);
    
}

void motorPBJoyst() 
{
    static unsigned char statePB = 0;
    
    switch (statePB){
        case 0:
            
            if (Button == PB_ON){
                TI_ResetTics(PBtimer);
                statePB++;
            }
            break;
        case 1: //Rebots d'entrada (1ms per tic)
            
            if (TI_GetTics(PBtimer) >= 20){
                if (Button == PB_ON) {
                    statePB++;
                    PB = 1;
                }
                else statePB--;
            }
            break;
        case 2:
            
            if (Button == PB_OFF) {
                TI_ResetTics(PBtimer);
                statePB++;
            }
            break;
        case 3:
            
            if (TI_GetTics(PBtimer) >= 20){
                if (Button == PB_OFF) statePB = 0;
                else (statePB = 2);
            }
            break;
    }
}

unsigned char getButtonIsPressed()
{
    if (PB == 1) {
        PB = 0;
        return 1;
    }
    else {
        return 0;
    }
}