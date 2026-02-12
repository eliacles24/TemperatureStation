#ifndef TAD_LED
#define	TAD_LED

#include <xc.h>
#include "pic18f4321.h"

#define RED LATAbits.LATA3
#define GREEN LATAbits.LATA4
#define BLUE LATAbits.LATA5

#define LOW_TEMP 1
#define MOD_TEMP 2
#define HIGH_TEMP 3
#define CRITICAL_TEMP 4

#define VERMELL 3
#define VERD    1
#define BLAU    2
#define TURQUESA    6
#define BLANC   7
#define LILA    8
#define GROC    9

#define ON 0
#define OFF 1


/*******************************************************************************
 **************************** PRIVATE FUNCTIONS ********************************
 ******************************************************************************/

void reset_LEDS(void);

/*******************************************************************************
 ***************************** PUBLIC FUNCTIONS ********************************
 ******************************************************************************/

void init_LED(void);
//Pre: -
//Post: Initializes TAD, Precondition for all the other functions

void motor_LED(void);
//Post: Sets RGB led to the desired colour

void recieve_temp(unsigned char temp);
//Post: Recieves new temperature reading, and sets the state of the room, changing colour of the light

#endif