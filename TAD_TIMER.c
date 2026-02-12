#include <xc.h>
#include "pic18f4321.h"
#include "TAD_TIMER.h"

// Definitions, to interrupt every 0,5ms, so 2000 tics == 1s
#define T0CON_CONFIG 0x82
#define RELOAD_TMR0 64911           // 2 ms, assuming FOsc at 10MHz. If HSPLL, 0.5ms

#define TI_NUMTIMERS 12              // Maximum mumber of virtual timers, to be adjusted at will

// GLOBAL VARIABLES OF THE TAD
struct Timer {
	unsigned int InitialTics;
	unsigned char Busy;
} static Timers[TI_NUMTIMERS];

static volatile unsigned int Tics = 0;

//Routine that must be called when the TMR0 interruption triggers to update tics
void RSI_Timer0 () {
    // Pre: IMPORTANT! This function needs to be called from the Interrupt Service Routine (ISR) when TMR0IF==1.
    TMR0 = RELOAD_TMR0;
    TMR0IF = 0;
    Tics++;    
}

//Initialise timer
void TI_Init () {
	//Pre: Global interrupts need to be enabled
	for (unsigned char counter=0; counter<TI_NUMTIMERS; counter++) {
		Timers[counter].Busy = TI_FALSE;
	}
	T0CON = T0CON_CONFIG;
    TMR0 = RELOAD_TMR0;
	INTCONbits.TMR0IF = 0;
	INTCONbits.TMR0IE = 1;
}

/*We send the by reference the variable that stores the reference to Timers[].
 * The function then starts a counter and starts looking for a free timer in
 * Timers[]. If it doesn't find one, it returns false. If it finds one, it 
 * modifies the *TimerHandle so the function that requested the timer 
 * has the reference to its assigned timer. 
*/
unsigned char TI_NewTimer(unsigned char *TimerHandle) {
	static unsigned char Counter = 0;
	while (Timers[Counter].Busy == TI_TRUE) {
		if (++Counter == TI_NUMTIMERS) return (TI_FALSE);
	}
	Timers[Counter].Busy = TI_TRUE;
	*TimerHandle = Counter;
    return (TI_TRUE);
}


//Reset tics to 0
void TI_ResetTics (unsigned char TimerHandle) {
	di(); Timers[TimerHandle].InitialTics = Tics; ei();
}

//Returns the tics of a timer
unsigned int TI_GetTics (unsigned char TimerHandle) {
    di(); unsigned int CopyCurrentTics=Tics; ei();
	return (CopyCurrentTics-(Timers[TimerHandle].InitialTics));
}

//Frees a timer that is no longer needed
void TI_CloseTimer (unsigned char TimerHandle) {
	Timers[TimerHandle].Busy  =TI_FALSE;
}

void TI_End () {
}
