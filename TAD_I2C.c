#include <xc.h>
#include "TAD_I2C.h"

static unsigned char stRd = 0;

//-------------- Private functions: --------------

unsigned char I2C_Ready()
{
    static unsigned char stReady = 0;
    
    switch(stReady) {
        case 0:                     /* Wait if bit collision interrupt flag is set*/
            if (!PIR2bits.BCLIF)
                stReady++;
            break;
        case 1:
            if (!SSPSTATbits.BF && !(SSPSTATbits.R_nW)) {
                PIR1bits.SSPIF=0;
                stReady--;
                return 1;
            }
            break;
    }
    
    return 0;
}

char I2C_Ack()
{
    static unsigned char stACK = 0;
    
    switch(stACK) {
        case 0:
            SSPCON2bits.ACKDT=0;
            SSPCON2bits.ACKEN=1;
            stACK++;
            break;
        case 1:
            
            if (!SSPCON2bits.ACKEN) {
                stACK--;
                return 1;
            }
            
            break;
    }
    
    return 0;
    
 }

char I2C_Nack()
{
    static unsigned char stNACK = 0;
    
    switch(stNACK) {
        case 0:
            SSPCON2bits.ACKDT=1;        /* Acknowledge data 1:NACK,0:ACK */
            SSPCON2bits.ACKEN=1;        /* Enable ACK/NACK to send */
            stNACK++;
            break;
        case 1:
            
            if (!SSPCON2bits.ACKEN) {
                stNACK--;
                return 1;
            }
            
            break;
    }
    
    return 0;
    
}

//-------------- Public functions: --------------

void InitI2C()
{
  TRISCbits.TRISC3 = 1;
  TRISCbits.TRISC4 = 1;
  SSPSTAT =0x80;
  SSPCON1 = 0x28;       //Master mode, clock = Fosc / (4 * (SSPADD + 1)
  SSPCON2 = 0x0;
  SSPADD = BAUDRATE;
  PIE1bits.SSPIE=0;
  PIR1bits.SSPIF=0;
}


char StartI2C(char slave_rw_addr)
{
    static unsigned char stSt = 0;
    unsigned char ans;
    
    switch(stSt) {
        case 0:
            SSPCON2bits.SEN = 1;
            stSt++;
            break;
        case 1:
            if(!SSPCON2bits.SEN)
                stSt++;
            break;
        case 2:
            PIR1bits.SSPIF = 0;
            
            if((SSPSTATbits.S))
                stSt++;
            else
                stSt = 0;
            break;
        case 3:
            ans = (I2C_Write(slave_rw_addr));
            
            if (ans != 0)
                stSt = 0;
            
            return ans;
            break;
    }
    
    return 0;
  
}

char ReStartI2C_(char slave_rw_addr)
{
    
    static unsigned char stRSt = 0;
    unsigned char ans;
    
    switch(stRSt) {
        case 0:
            SSPCON2bits.RSEN = 1;
            stRSt++;
            break;
        case 1:
            
            if(!SSPCON2bits.RSEN)
                stRSt++;
            
            break;
        case 2:
            
            PIR1bits.SSPIF = 0;
            
            if((SSPSTATbits.S))
                stRSt++;
            else
                stRSt = 0;
            break;
        case 3:
            ans = (I2C_Write(slave_rw_addr));
            
            if (ans != 0)
                stRSt = 0;
            
            return ans;
            break;
    }
    
    return 0;
}
 
 char I2C_Write(unsigned char data)
{
     
     static unsigned char st = 0;
     
     switch(st) {
         case 0:
            SSPBUF=data;	/* Write data to SSPBUF*/
            st++;
            break;
         case 1:
             
            if (I2C_Ready()) {
                st = 0;
                if (SSPCON2bits.ACKSTAT)	/* Check for acknowledge bit from slave*/
                    return 1;
                else
                    return 2;
            }
             
            break;
     }
      
      return 0;
}
 
 char I2C_Stop_()
{
     
     static unsigned char stStop = 0;
     
     switch(stStop) {
         case 0:
             if (I2C_Ready()) {
                 stStop++;
                 SSPCON2bits.PEN=1;		/* Stop communication*/
             }
             
            break;
         case 1:
             
            if(!SSPCON2bits.PEN) {
                PIR1bits.SSPIF = 0;
                stStop++;
            }
             
             break;
         case 2:
             stStop = 0;
            if (!SSPSTATbits.P)
                return 0;
            return 1;
            break;
     }
     
    return 0;
}

char I2C_Read(char flag, char* done)
{
    
    static unsigned char stRd = 0;
    static char buffer = 0;
    
    switch(stRd) {
        case 0:
            SSPCON2bits.RCEN=1;			/* Enable receive */
            buffer = 0;
            stRd++;
            break;
        case 1:

            /* Wait for buffer full flag which when complete byte received */
            if(SSPSTATbits.BF)
                stRd++;
            
            break;
        case 2:
            
            buffer=SSPBUF;		/* Copy SSPBUF to buffer */
            
            if(flag==0)
                stRd++;
            else
                stRd = 4;
            
            break;
        case 3:
            if(I2C_Ack())
                stRd = 5;
            break;
        case 4:
            if(I2C_Nack())
                stRd++;
            break;
        case 5:
            if(I2C_Ready()) {
                *done = 1;
                stRd = 0;
                return(buffer);
            }
            break;
    }
    
    *done = 0;
    
}

void setRTCAlarms() {
    
    while(!StartI2C(0b11010000)){}
    while(!I2C_Write(0x0B)){}
    
    while(!I2C_Write(0b10000000)){}
    while(!I2C_Write(0b10000000)){}
    while(!I2C_Write(0b10000000)){}
    
    while(!I2C_Write(0b00000110)){}
    
    while(!I2C_Stop_()){}
    
}