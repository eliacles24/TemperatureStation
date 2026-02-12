#ifndef TAD_MANAGER_TMP
#define	TAD_MANAGER_TMP

#include <xc.h>
#include "pic18f4321.h"


/*******************************************************************************
 **************************** PRIVATE FUNCTIONS ********************************
 ******************************************************************************/


/*******************************************************************************
 ***************************** PUBLIC FUNCTIONS ********************************
 ******************************************************************************/

void initTMPMan(void);
//Post: Precondition for the rest of the functions

void setThresholdLow(unsigned char temp[]);
//Post: Recieves a number, and sets it as the threshold for low temperature

void setThresholdMod(unsigned char temp[]);
//Post: Recieves a number, and sets it as the threshold for moderate temperature

void setThresholdHigh(unsigned char temp[]);
//Post: Recieves a number, and sets it as the threshold for high temperature

void setThresholdCrit(unsigned char temp[]);
//Post: Recieves a number, and sets it as the threshold for critical temperature

void setPollingRate(unsigned char time[]);
//Post: Recieves a number, and sets it as polling rate in seconds in which the temperature will be read, analysed and stored

void motorTemp(void);

unsigned char resetTemp();

#endif