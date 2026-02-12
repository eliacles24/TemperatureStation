#include "TAD_UART.h"

#define TRUE 1
#define FALSE 0

void initEusart (){
    
    RCSTAbits.SPEN = 1; 
    TRISCbits.RC6 = 1; 
    TRISCbits.RC7 = 1; 
    TXSTAbits.SYNC = 0; 
    TXSTAbits.TXEN = 1; 
    RCSTAbits.CREN = 1;
    
    TXSTAbits.BRGH = 1; 
    BAUDCONbits.BRG16 = 1; 
    SPBRG = 86;
        
}

unsigned char SIO_RXAvail(void)
{
    return PIR1bits.RCIF;
}

unsigned char SIO_GetChar(void)
{
    //We read the character received
    return RCREG;
}

unsigned char SIO_TXAvail(void)
{
    return TXSTAbits.TRMT;
}

void SIO_PutChar(unsigned char value)
{
    TXREG = value;
}

char printString(unsigned char *s) {
    
    if (*s){
        if (SIO_TXAvail() ){
            SIO_PutChar(*s++);
        }
        return 0;   //Function hasn't finished yet
    }

    return 1;       //Function has finished printing
}


unsigned char SIO_PutString(char *s) {
    static char *a = NULL;
    
    if (a == NULL) a = s;   //Copy pointer to first character of input string in first call
    
    if (SIO_TXAvail() ){
        SIO_PutChar(*a++);  //If char was printed successfully, advance the pointer to next char
    }
    
    if (!(*a) || a == NULL || a == '\0'){
        a = NULL;
        return 1;           //Function has finished printing
    } 
    
    return 0;               //Function hasn't finished yet

}

void SIO_PrintInt(int number) {
    unsigned char num[4] = {0, 0, 0, 0};

    // Assegurem que el número està dins del rang 0 a 9999
    //if (number < 0) number = 0;
    //if (number > 9999) number = 9999;

    num[0] = number / 1000;             // milers
    num[1] = (number / 100) % 10;       // centenes
    num[2] = (number / 10) % 10;        // desenes
    num[3] = number % 10;               // unitats

    while (!SIO_TXAvail()) {}
    SIO_PutChar('0' + num[0]);

    while (!SIO_TXAvail()) {}
    SIO_PutChar('0' + num[1]);

    while (!SIO_TXAvail()) {}
    SIO_PutChar('0' + num[2]);

    while (!SIO_TXAvail()) {}
    SIO_PutChar('0' + num[3]);
    
    while(!SIO_PutString("\r\n")){}
    
}
