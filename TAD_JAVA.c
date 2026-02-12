#include "TAD_JAVA.h"

#include "TAD_UPD_TIME.h"
#include "TAD_UART.h"
#include "TAD_TMP_manager.h"
#include "TAD_RTC_var.h"
#include "TAD_SENDGRAPH.h"
#include "TAD_EEPROM.h"
#include "TAD_RAM.h"

const unsigned char INIT[] = "INIT:";           // Default initialization command
const unsigned char TIME[] = "SET_TIME:";       // Set the real-time clock
const unsigned char LOGS[] = "LOG_GET";         // Request logs
const unsigned char GRAPH[] = "GRAPH_GET";      // Request graph data
const unsigned char RESET[] = "RESET";          // Reset command

unsigned char javaReset;

void motor_JAVA()
{
    static unsigned char i = 0;
    static unsigned char stJava = 0;
    static unsigned char info[STR_SIZE];
    static unsigned char javaReset = 0;
    
    switch(stJava)
    {
        case 0:
            
            if (SIO_RXAvail() == TRUE){
                info[i] = SIO_GetChar();
                i++;
                
                if (info[0] == 'I') {
                    stJava++;
                } else if (info[0] == 'S') {
                    stJava = S_SET_TIME;
                } else if (info[0] == 'G') {
                    stJava = S_GRAPH;
                } else if (info[0] == 'L') {
                    stJava = S_LOG;
                }else if (info[0] == 'R') {
                    stJava = S_RESET;
                } else {
                   stJava = 25;
                }
                
            }
            
            break;
        case S_INIT:                        //Init: check
            
            if (SIO_RXAvail() == TRUE) {
                info[i] = SIO_GetChar();
                
                if (info[i] == ':') {
                    stJava++;
                    i=0;
                } else if (info[i] != INIT[i]) {
                    stJava = 25;
                } else 
                    i++;   
            }
            
            break;
        case 2:                             //year
            
            if (SIO_RXAvail() == TRUE) {
                info[i] = SIO_GetChar();
                
                if (info[i] == '-') {
                    stJava++;
                    info[i]='\0';
                    setYearToRTC(info);
                    i=0;
                } else {
                    i++;
                }
            }
            
            break;
        case 3:                             //month
            
            if (SIO_RXAvail() == TRUE) {
                info[i] = SIO_GetChar();
                
                if (info[i] == '-') {
                    stJava++;
                    info[i]='\0';
                    setMonthToRTC(info);
                    i=0;
                } else {
                    i++;
                }
            }
            
            break;
        case 4:                             //day
            
            if (SIO_RXAvail() == TRUE) {
                info[i] = SIO_GetChar();
                
                if (info[i] == ' ') {
                    stJava++;
                    info[i]='\0';
                    setDayToRTC(info);
                    i=0;
                } else {
                    i++;
                }
            }
            
            break;
        case 5:                             //hour
            
            if (SIO_RXAvail() == TRUE){
                info[i] = SIO_GetChar();
                
                if (info[i] == ':') {
                    stJava++;
                    info[i]='\0';
                    setHourToRTC(info);
                    i=0;
                } else 
                    i++;
            }
            
            break;
        case 6:                             //minutes
            
            if (SIO_RXAvail() == TRUE){
                info[i] = SIO_GetChar();
                
                if (info[i] == '$') {
                    stJava++;
                    info[i]='\0';
                    setMinToRTC(info);
                    i=0;
                } else 
                    i++;
            }
            
            break;
        case 7:                                 //polling rate
            
            
            if (SIO_RXAvail() == TRUE){
                info[i] = SIO_GetChar();
                
                if (info[i] == '$') {
                    stJava++;
                    info[i]='\0';
                    setPollingRate(info);       //S'ENVIA BE
                    i=0;
                } else 
                    i++;
            }
            
            
            break;
        case 8:                                 //low temp
            
            if (SIO_RXAvail() == TRUE){
                info[i] = SIO_GetChar();
                
                if (info[i] == '$') {
                    stJava++;
                    info[i]='\0';
                    setThresholdLow(info);
                    i=0;
                } else 
                    i++;
            }
            
            break;
        case 9:                                 //Moderate temp
            
            
            if (SIO_RXAvail() == TRUE) {
                info[i] = SIO_GetChar();
                
                if (info[i] == '$') {
                    stJava++;
                    info[i]='\0';
                    setThresholdMod(info);
                    i=0;
                } else 
                    i++;
            }
            
            break;
        case 10:                                //high temp
            
            
            if (SIO_RXAvail() == TRUE){
                info[i] = SIO_GetChar();
                
                if (info[i] == '$') {
                    stJava++;
                    info[i]='\0';
                    i=0;
                    setThresholdHigh(info);
                } else  
                    i++;
            }
            
            break;
        case 11:                                    //critical temp
            
            if (SIO_RXAvail() == TRUE){
                info[i] = SIO_GetChar();
                
                if (info[i] == '\n') {
                    stJava = S_START_RTC;
                    i--;
                    info[i--]='\0';
                    setThresholdCrit(info);
                    i=0;
                } else {
                    i++;
                }
            }
            
            break;
        case S_SET_TIME:                    //SET_TIME comprovation
                        
            if (SIO_RXAvail() == TRUE) {
                info[i] = SIO_GetChar();
                
                if (info[i] == ':') {
                    stJava++;
                    i=0;
                } else if (info[i] != TIME[i]) {
                    stJava = 25;
                } else 
                    i++;   
            }
            
            break;
        case 19:                        //hour
            
            if (SIO_RXAvail() == TRUE) {
                info[i] = SIO_GetChar();
                
                if (info[i] == ':') {
                    stJava++;
                    info[i]='\0';
                    setHourToRTC(info);
                    i=0;
                } else {
                    i++;
                }
            }
            
            break;
        case 20:                        //minute
            
            if (SIO_RXAvail() == TRUE) {
                info[i] = SIO_GetChar();
                
                if (info[i] == '\n') {
                    i--;
                    stJava++;
                    info[i--]='\0';
                    setMinToRTC(info);
                    i=0;
                } else {
                    i++;
                }
            }
            
            break;
        case 21:
            if (WriteRTCinfo(0))
                stJava = 0;
            break;
        case 22:             //Get logs
            
            if (SIO_RXAvail() == TRUE) {
                info[i] = SIO_GetChar();
                
                if (info[i] == '\n') {
                    info[--i] = '\0';
                    stJava = 30;
                    i=0;
                } else if (info[i] != LOGS[i] && info[i] != '\r') {
                    stJava = 25;
                } else 
                    i++;   
            }
            
            break;
        case 23:            //Get graph
            
            if (SIO_RXAvail() == TRUE) {
                info[i] = SIO_GetChar();
                
                if (info[i] == '\n') {
                    startSendingGraph();
                    stJava = 0;
                    i=0;
                } else if (info[i] != GRAPH[i] && info[i] != '\r') {
                    stJava = 25;
                } else 
                    i++;   
            }
            
            break;
        case S_RESET:           //NO VA
            
            if (SIO_RXAvail() == TRUE) {
                info[i] = SIO_GetChar();
                
                if (info[i] == '\n') {
                    javaReset = 1;
                    stJava = 33;
                    i=0;
                } else if (info[i] != RESET[i] && info[i] != '\r') {
                    stJava++;
                } else 
                    i++;   
            }
            
            break;
        case 25:
            
            if (SIO_RXAvail() == TRUE){
                info[i] = SIO_GetChar();
                
                if (info[i] == '\n') {
                    while(!SIO_PutString(info)){}
                    while(!SIO_PutString("\r\n")){}
                    stJava = 0;
                    i=0;
                }
                
                i++;
                
            }
            
            break;
        case S_START_RTC:
            if (WriteRTCinfo(1))
                stJava++;
            break;
        case 27:
            if (startAlarm() == 1) 
                stJava = 0;
            break;
        case 30:
            if (isEEPROMAvail())
                stJava++;
            break;
        case 31:
            if (readEEPROMPhase())
                stJava++;
            break;
        case 32:
            if (SIO_PutString("END\r\n"))
                stJava = 0;
            break;
        case 33:
            if (resetEEPROM() == 1)
                stJava++;
            break;
        case 34:
            if (resetTemp() == 1) {
                resetAddress();
                stJava = 0;
            }
            break;
            
            
    }
    
}