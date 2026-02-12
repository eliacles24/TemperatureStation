#ifndef TAD_EEPROM
#define	TAD_EEPROM

#include <xc.h>
#include "pic18f4321.h"

#define b_READ EECON1bits.RD
#define b_WRITE EECON1bits.WR
#define enable_Wr EECON1bits.WREN

#define MAX_ADD 210

#define READ 1
#define WRITE 1

#define TRUE 1
#define FALSE 0

/*******************************************************************************
 **************************** PRIVATE FUNCTIONS ********************************
 ******************************************************************************/
unsigned char readEEPROM(unsigned char address);
//Pre: isEEPROMAvail() returned TRUE
//Post: Returns data from the single address selected, and sets EEPROM status as available

unsigned char writeEEPROM(unsigned char data, unsigned char address);
//Pre: isEEPROMAvail() returned TRUE
//Post: It writes data in the following empty address, returning a 1, 0 otherwise.

/*******************************************************************************
 ***************************** PUBLIC FUNCTIONS ********************************
 ******************************************************************************/

void init_EEPROM(void);
//Pre: -
//Post: initializes EEPROM. Precondition for all the other funcitons in this TAD

unsigned char isEEPROMAvail(void);
//Post: returns 1 if EEPROM can be used, 0 otherwise.

unsigned char writePhraseEEPROM(unsigned char data[], unsigned char size);
//Pre: isEEPROMAvail() returned TRUE
//Post: It writes data in the following empty addresses, until it finished, returning a 1, 0 otherwise.

unsigned char readEEPROMPhase();
//NO UTILITZAR, FUNCIONA PERO NO ESTA DEFINIDA PER DIFERENTS CASOS DE LECTURA.
//NO COOPERATIVA, ERA PER DEBUGGING

unsigned char resetEEPROM();
//Post: guarda un 0 a la posició on guardem el punter a l'ultim log.

#endif