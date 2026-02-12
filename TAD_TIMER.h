#ifndef TAD_TIMER
#define TAD_TIMER

#define TI_FALSE 0
#define TI_TRUE 1

//The ISR
void RSI_Timer0(void);
    // IMPORTANT! This function needs to be called from the Interrupt Service Routine (ISR) when TMR0IF==1.
    // This ISR must contain: if (TMR0IF==1) RSI_Timer0();

void TI_Init (void);
	// Pre: Global interrupts are enabled
	// Post: Constructor. It is a global preconidtion to have instantiated this method before using any other function of this ADT

unsigned char TI_NewTimer(unsigned char *TimerHandle) ;
	// Post: Returns TI_TRUE when a new virtual timer has been successfully created, it returns TI_FALSE otherwise
    // Writes in *TimerHandle the handled of the assigned timer

void TI_ResetTics (unsigned char TimerHandle);
	// Pre: TimerHandle comes from Ti_NewTimer.
	// Post: Stores the time reference associated to TimerHandle.

unsigned int TI_GetTics (unsigned char TimerHandle);
	// Pre: TimerHandle comes from Ti_NewTimer.
	// Post: Returns the number of tics that have happened since the last TI_ResetTics of TimerHandle.

void TI_CloseTimer (unsigned char TimerHandle);
    // Pre: TimerHandle comes from Ti_NewTimer.
    // Post: Releases the virtual timer associated to TimerHandle

void TI_End (void);
	// Post: Destructor of the ADT

#endif
