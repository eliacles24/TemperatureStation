#include "TAD_RAM.h"

#include "TAD_UART.h"

#include "TAD_LED.h"
#include "TAD_PBJoyst.h"

static unsigned int ramAddress;    //NOTE: stores the first available address to WRITE
static unsigned char blockWriting; //Set when reading to avoid overwriting values
static unsigned char roundRAM;

void init_RAM()
{
    //set control signals as outputs
    TRISBbits.TRISB4 = 0;
    TRISBbits.TRISB3 = 0;
    TRISBbits.TRISB0 = 0;
    
    TRISEbits.TRISE0 = 0;
    
    TRISCbits.TRISC0 = 0;
    TRISCbits.TRISC5 = 0;
    //TRISCbits.TRISC2 = 0;
    
    //Set initial variables
    OE = OFF;       //Output enable
    WE = OFF;       //Write enable
    CE = OFF;
    
    RCounter = ON;  //Counter Reset
    TCounter = 0;   //Counter Clock Signal
    RCounter = OFF; 
    
    ramAddress= 0;
    blockWriting = 0;
    Ram_RD();
    resetAddress();
    
    roundRAM = 0;
    
}

unsigned char writeToRam(unsigned char data){
    static unsigned char stWrite = 0;
    
    switch(stWrite) {
        case 0:
            if (blockWriting == 0) {
                blockWriting = 1;
                Ram_WR();
                OE = OFF;
                CE = ON;
                WE = ON;
                stWrite++;
            }
            break;
        case 1:
            LATD = data;
            
//            SIO_PrintInt(data);
//            while(!SIO_PutString("\r\n")){}
            
            if (data & 0x02) {
                LATEbits.LATE0 = 1;
            } else {
                LATEbits.LATE0 = 0;
            }
        
            if (roundRAM == 0) {
                roundRAM = 1;
            }
        
            incrementAddress();
            stWrite++;
            break;
        case 2:
            CE = OFF;
            WE = OFF;
            Ram_RD();
            stWrite = 0;
            blockWriting = 0;
            
            char number;
            number = ramAddress;
            resetAddress();
                
            while(ramAddress < number) {
                while(readFromRam() == -1){}
            }
              
            
            return 1;
    }
    
    return 0;
    
}

signed char readFromRam(void)
{
    static unsigned char stRead = 0;
    static unsigned char data;
    

    switch(stRead) {
        case 0:
            TRISEbits.TRISE0 = 1;
            Ram_RD();
            WE = OFF;
            CE = ON;
            OE = ON;
            stRead++;
            break;
        case 1:
            data = PORTD;
            
            if (PORTEbits.RE0 == 1) {
                data = data | 0x02;
            }
            
//            while(!SIO_PutString("Reading: ")){}
//            SIO_PrintInt(data);
//            while(!SIO_PutString("\r\n")){}
            
            incrementAddress();
            stRead++;
            break;
        case 2:
            CE = OFF;
            OE = OFF;
            stRead = 0;
            TRISEbits.TRISE0 = 0;
            return data;
            break;
    }
    
    return -1;
}

unsigned int getRamAddress(void){
    return ramAddress;
}


void resetAddress(void) {
    RCounter = ON;
    //Nop();
    RCounter = OFF;
    ramAddress = 0;
}

void incrementAddress(void) {
    
    TCounter = 1;
    //Nop();
    TCounter = 0;
    
    ramAddress++;
    
    if (ramAddress == 32769) {
        resetAddress();
        roundRAM = 2;
    }
    
}

void blockRamWriting(void) {
    blockWriting = 1;
}

void unblockRamWriting(void) {
    blockWriting = 0;
}

unsigned char getWRRound() {
    return roundRAM;
}

char getRamIsBusy(void) {
    return blockWriting;
}