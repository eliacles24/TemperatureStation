#ifndef TAD_SIO
#define	TAD_SIO

#include <xc.h>
#include "pic18f4321.h"

#define TRUE 1
#define FALSE 0

/**************************************************
 *          PRIVATE FUNCTIONS
 *************************************************/

/**************************************************
 *          PUBLIC FUNCTIONS
 *************************************************/
void initEusart(void);
//Post: configure the UART in async mode, sets baudrate...

unsigned char SIO_RXAvail(void);
//POST: returns TRUE if there's a receive character pending to be read. Otherwise, false

unsigned char SIO_GetChar(void);
//PRE: SIO_RXAvail() has returned true
//POST: Performs a destructive read of the received character

unsigned char SIO_TXAvail(void);
//POST: returns TRUE if there's availability to send a character. Otherwise, false

void SIO_PutChar(unsigned char value);
//PRE: SIO_TXAvail has returned true
//POST: Queues a new character for transmission

char printString(unsigned char *s);

unsigned char SIO_PutString(char *s);
//PRE: SIO_TXAvail has returned true
//POST: Transmits a whole string of characters, returns 1 when it's done, 0 otherwise

void SIO_PrintInt(int number);

#endif