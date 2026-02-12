#ifndef TAD_ADC
#define	TAD_ADC

#include <xc.h> 
#include "pic18f4321.h"

#define ChannelJoystickX ADCON0bits.CHS = 0
#define ChannelJoystickY ADCON0bits.CHS = 1
#define ChannelTemp ADCON0bits.CHS = 2
#define Done ADCON0bits.GODONE

#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3
#define CENTER 4
#define DEAD_ZONE 5

void initADC(void);
//Post: prerequisit for the rest of the functions

void motorADC(void);

unsigned char getDirectionJoystick(void);
//Post: Returns current direction of the joystick, varying from UP, DOWN, LEFT, RIGHT or CENTER

unsigned int getTemp(void);
//Post: Returns value corresponding to temperature, which needs to be converted to degrees

#endif	

