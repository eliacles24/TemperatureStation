#ifndef TAD_FANS
#define	TAD_FANS

#include <xc.h>
#include "pic18f4321.h"

#define FAN0 LATCbits.LATC2
#define FAN1 LATCbits.LATC1

#define LOW_STATE 0
#define MODERATE_STATE 1
#define HIGH_STATE 2
#define CRITICAL_STATE 3

/*******************************************************************************
 **************************** PRIVATE FUNCTIONS ********************************
 ******************************************************************************/


//Resum Enhanced PWM: ------
/*
 * (sense '[]'
 * CCP1CON.DC1B[1..0] els 2 MSB duty cycle
 * CCPR1L els 8 LSB duty cycle
 * 
 * CCP1M[3..0] seleccionar mode
 *  ""



/*******************************************************************************
 ***************************** PUBLIC FUNCTIONS ********************************
 ******************************************************************************/

void init_FANS(void);
//Post: Precondition for the rest of the TAD

void PWM_FANS(void);

void setFansState(char newState);
//Post: Sets temperature state for fan motor, activating corresponding action to temperature

#endif