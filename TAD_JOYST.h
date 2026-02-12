#ifndef TAD_JOYSTICK
#define	TAD_JOYSTICK

#include <xc.h>
#include "pic18f4321.h"

#define button LATBbits.AN8

/*******************************************************************************
 **************************** PRIVATE FUNCTIONS ********************************
 ******************************************************************************/


/*******************************************************************************
 ***************************** PUBLIC FUNCTIONS ********************************
 ******************************************************************************/

void init_JOYSTICK(void);
//Pre: -
//Post: Initializes TAD, Precondition for all the other functions

void motor_Joyst(void);

#endif