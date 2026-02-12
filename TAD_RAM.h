#ifndef TAD_RAM 
#define	TAD_RAM

#include <xc.h>
#include "pic18f4321.h"

#define RCounter LATCbits.LATC0 
#define TCounter LATCbits.LATC5

#define OE LATBbits.LATB3
#define WE LATBbits.LATB0
#define CE LATBbits.LATB4

#define DB5 LATDbits.LATD5
#define DB6 LATDbits.LATD6
#define DB7 LATDbits.LATD7
#define DB4 LATDbits.LATD4
#define DB0 LATDbits.LATD0
#define DB1 LATDbits.LATD1
#define DB3 LATDbits.LATD3
#define DB2 LATDbits.LATD2

#define Ram_WR() TRISD = 0
#define Ram_RD() TRISD = 1

#define READING 1
#define WRITING 0

//(for RAM control bits, which are active LOW)
#define OFF 1
#define ON 0

/*******************************************************************************
 **************************** PRIVATE FUNCTIONS ********************************
 ******************************************************************************/

void writing_DB(void);
void reading_DB();

/*******************************************************************************
 ***************************** PUBLIC FUNCTIONS ********************************
 ******************************************************************************/

//==============================================================================
//RAM instructions
void init_RAM(void);
//Pre: -
//Post: Initializes TAD, Precondition for all the other functions

unsigned int getRamAddress(void);
//Post: returns the current address of the ram

unsigned char getWRRound(void);
//Post: returns if the round had started yet, if it's the first or if it's at least the second

char getRamIsBusy(void);
//Post: returns 1 if the RAM is busy (already performing an operation), 0 if it's available

unsigned char writeToRam(unsigned char);
//Pre: RAM address is the one where data will be written (new data will overwrite old data).
//     RAM is not busy
//Post: Writes the input data in the current @ of the RAM.

signed char readFromRam(void);
//Post: Returns information from current address

void RAMprova(void);
void RAMprovaSetup(void);

//==============================================================================
//Counter instructions

void resetAddress(void);
//Post: resets the @Bus of the RAM

void incrementAddress(void);
//Post: increments the @Bus of the Ram by 1

void blockRamWriting(void);
//Post: Sets RAM as busy

void unblockRamWriting(void);
//Post: Sets RAM as free for further use


//==============================================================================

#endif