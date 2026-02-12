#include "TAD_UPD_TIME.h"

#include "TAD_RTC_var.h"
#include "TAD_UART.h"

static unsigned char new_time_str[] = "NEW_TIME:00:00\r\n";
static unsigned char initiated = 1;

void intiTimeUpdate()
{
    TRISBbits.TRISB1 = 1;
    initiated = 0;
}

void timeUpdateMotor()
{
    static unsigned char stTimeUpd = 0;
    static unsigned char hour;
    static unsigned char min;
    
    switch(stTimeUpd) {
        case 0:
            if (SQW == 0 && initiated) {
                stTimeUpd++;
            }
            break;
        case 1:
            if (RTCisAvail()) {
                stTimeUpd++;
            }
            break;
        case 2:
            if (resetAlarmMotor() == 1) {
                stTimeUpd++;
            }
            break;
        case 3:
            if (RTCisAvail())
                stTimeUpd++;
            break;
        case 4:
            if (readRTC())
                stTimeUpd++;
            break;
        case 5:
            new_time_str[9] = '0' + getDecimalHour();
            new_time_str[10] = '0' + getUnitHour();
            stTimeUpd++;
            break;
        case 6:
            new_time_str[12] = '0' + getDecMinute();
            new_time_str[13] = '0' + getUnitMinute();
            stTimeUpd++;
            break;
        case 7:
            if (SIO_TXAvail())
                stTimeUpd++;
            break;
        case 8:
            if (SIO_PutString(new_time_str))
                stTimeUpd = 0;
            break;
            
    }
}

unsigned char startAlarm() {
    
    static unsigned char stAlarm = 0;
    
    switch(stAlarm) {
        case 0:
            if (RTCisAvail()) {
                stAlarm++;
            }
            break;
        case 1:
            if (resetAlarmMotor() == 1) {
                initiated = 1;
                stAlarm--;
                return 1;
            }
            break;
    }
    
    return 0;
}