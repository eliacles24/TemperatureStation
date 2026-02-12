#include "TAD_SENDGRAPH.h"

#include "TAD_RAM.h"
#include "TAD_UART.h"

#include "TAD_TIMER.h"

#define END_GRAPH 10

unsigned char sendGraph = 0;
unsigned char Value[5];

void motor_sendGraph(){
    static unsigned char sendGraphState = 0;
    static signed char temp;
    static unsigned char dec, unit, address;
    
    switch (sendGraphState) {
        case 0:
            if (sendGraph == 1 && getRamIsBusy() == 0) {
                sendGraph = 0;
                
                if (getWRRound() != 0) {
                    blockRamWriting();
                    address = getRamAddress();
                    resetAddress();
                    sendGraphState++;
                } else {
                    sendGraphState = 7;
                }
            }
            break;
        case 1:
            if (SIO_PutString("GRAPH:")){
                sendGraphState++;
            }
            break;
        case 2:
            temp = readFromRam();
            
            if (temp != -1) {
                dec = temp / 10;
                unit = temp % 10;
                sendGraphState++;
            }
            break;
        case 3:
            if (SIO_TXAvail()) {
                SIO_PutChar('0' + dec);
                sendGraphState++;
            }
            break;
        case 4:
            if (SIO_TXAvail()) {
                SIO_PutChar('0' + unit);
                sendGraphState++;
            }
            break;
        case 5:
            if (SIO_PutString("\r\n")) {
                sendGraphState++;
            }
            break;
        case 6:
            if (getRamAddress() == address) {
                sendGraphState++;
            } else {
                sendGraphState = 1;
            }
            break;
        case 7:
            if (SIO_PutString("END\r\n")) {
                sendGraphState = 0;
                unblockRamWriting();
            }
            break;
    }
    
}

void startSendingGraph(void){
    sendGraph = 1;
}