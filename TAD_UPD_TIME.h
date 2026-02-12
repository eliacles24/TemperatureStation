#ifndef TAD_UPD_TIME
#define	TAD_UPD_TIME

#include <xc.h>
#include "pic18f4321.h"

#define SQW PORTBbits.RB1

void intiTimeUpdate();
//Post: Precondition for the rest of the functions in the TAD

void timeUpdateMotor(void);
unsigned char startAlarm();
//Post: Starts reading the alarms for every minute


#endif

