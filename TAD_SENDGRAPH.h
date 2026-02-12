#ifndef TAD_SENDGRAPH_H
#define	TAD_SENDGRAPH_H

#include <xc.h> 
#include "pic18f4321.h"

void motor_sendGraph(void);

void startSendingGraph(void);
//Post: Starts process to read and send all the stored information in the RAM, so all the temperature readings

void readRAM(unsigned char prevData);

#endif	