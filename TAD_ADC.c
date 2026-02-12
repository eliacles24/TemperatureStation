#include "TAD_ADC.h"

#include "TAD_UART.h"

static unsigned int valorX;
static unsigned int valorY;
static unsigned int valorTemp;

void initADC(void) 
{
    ADCON0bits.ADON = 1;
    
    ADCON1bits.VCFG1 = 0;
    ADCON1bits.VCFG0 = 0;
    
    // 3 analog 11 dig
    ADCON1bits.PCFG3 = 1;
    ADCON1bits.PCFG2 = 1;
    ADCON1bits.PCFG1 = 0;   
    ADCON1bits.PCFG0 = 0;
    
    ADCON2 = 0x2C;
    
    valorX = 0;
    valorY = 0;
    valorTemp = 0;

}


void motorADC(void){
    static unsigned char ADCstate = 0;
    
    switch(ADCstate){
        case 0:
            ChannelJoystickX;
            Done = 1;
            ADCstate++;
            break;
        case 1:
            if (Done == 0){
                valorX = (unsigned int) ADRESH << 2 | (ADRESL >> 6);
                ChannelJoystickY;
                ADCstate++;
            }
            break;
        case 2:
            Done = 1;
            ADCstate++;
        case 3:
            if (Done == 0) {
                valorY = (unsigned int) ADRESH << 2 | (ADRESL >> 6);
                ChannelTemp;
                ADCstate++;
            }
            break;
        case 4:
            Done = 1;
            ADCstate++;
        case 5:
            if (Done == 0) {
                valorTemp = (unsigned int) ADRESH << 2 | (ADRESL >> 6);
                ADCstate = 0;
            }
            break;
    }
    
}

unsigned char getDirectionJoystick() {

    if (valorX < 100 && valorY < 900 && valorY > 150) { return LEFT;}
    else if (valorX > 700 && valorY < 900 && valorY > 150) { return RIGHT;}
    else if (valorY < 100 && valorX < 900 && valorX > 150) { return UP;}
    else if (valorY > 700 && valorX < 900 && valorX > 150) { return DOWN;}
    else if (valorY >= 475 && valorY <= 525 && valorX >= 475 && valorX <= 525) { return CENTER;}
    else { return DEAD_ZONE; }
}

unsigned int getTemp() 
{   
    return valorTemp;
}