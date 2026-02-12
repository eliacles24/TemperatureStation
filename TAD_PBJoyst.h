#ifndef TAD_PBJoyst
#define	TAD_PBJoyst

#include <xc.h>
#include "pic18f4321.h"

#define defButton TRISBbits.TRISB2
#define Button PORTBbits.RB2

#define PB_ON 0
#define PB_OFF 1

#define LED8 LATAbits.LATA3 
#define LED9 LATAbits.LATA4 

/*******************************************************************************
 **************************** PRIVATE FUNCTIONS ********************************
 ******************************************************************************/


/*******************************************************************************
 ***************************** PUBLIC FUNCTIONS ********************************
 ******************************************************************************/
void init_PBJoyst(void);
//Post: Precondition for the other functions

void motorPBJoyst(void);

unsigned char getButtonIsPressed(void);
//Post: Returns 1 if button is pressed, 0 otherwise


#endif