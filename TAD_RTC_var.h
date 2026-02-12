#ifndef TAD_RTC_var
#define	TAD_RTC_var

#include <xc.h>
#include "pic18f4321.h"

#define ST_WR 1
#define ST_ADD_time 2
#define ST_WR_TIME 3
#define ST_WR_DOW 5
#define ST_WR_DATE 6
#define ST_SET_ALARMS 9
#define ST_END 14

#define ST_RD 27//TODO CHANGE
#define ST_RD_SEC 15
#define ST_RD_MIN 16
#define ST_RD_HOUR 17

#define ST_RD_DAY 19
#define ST_RD_MONTH 20
#define ST_RD_YEAR 21

#define ST_ADD_contrAlarm 25
#define ST_WR_TIME_RES 26

#define ST_ADD_sec 28

#define ADD_RD  0b11010001
#define ADD_WR  0b11010000

#define CONT 0
#define STOP 1

typedef struct 
{
    char day;
    char month;
    char year;
    char hour;
    char minutes;
} DateForRTC;

typedef struct
{
    char day[2];
    char month[2];
    char year[4];
    char hour[2];
    char minutes[2];
    char seconds[2];
    
} DateForUART;

void init_RTC();
//Post: Precondition for the rest of the functions

void setYearToRTC(unsigned char data[]);
//Pre: RTCisAvail() has returned 1
//Post: Sets year to be sent to RTC

void setMonthToRTC(unsigned char data[]);
//Pre: RTCisAvail() has returned 1
//Post: Sets month to be sent to RTC

void setDayToRTC(unsigned char data[]);
//Pre: RTCisAvail() has returned 1
//Post: Sets day to be sent to RTC

void setHourToRTC(unsigned char data[]);
//Pre: RTCisAvail() has returned 1
//Post: Sets hour to be sent to RTC

void setMinToRTC(unsigned char data[]);
//Pre: RTCisAvail() has returned 1
//Post: Sets minute to be sent to RTC

unsigned char getDecimalHour();
unsigned char getUnitHour();

unsigned char getDecMinute();
unsigned char getUnitMinute();

unsigned char getDecSecs();
unsigned char getUnitSecs();

unsigned char getDecDay();
unsigned char getUnitDay();

unsigned char getDecMonth();
unsigned char getUnitMonth();

unsigned char getMilYear();
unsigned char getCentYear();
unsigned char getDecYear();
unsigned char getUnitYear();

//void getYearRTC(unsigned char byte);
//void getMonthRTC(unsigned char byte);
//void getDayRTC(unsigned char byte);
//void getHourRTC(unsigned char byte);
//void getMinRTC(unsigned char byte);

unsigned char RTCisAvail();
//Post: Returns 1 if RTC is available for use, 0 otherwise

unsigned char WriteRTCinfo(unsigned char initialInput);
//Pre: RTCisAvail() has returned 1
//Post: Sends saved date to the RTC (if no date has been set, 0/0/0 00:00 is sent)

unsigned char readRTC();

unsigned char resetAlarmMotor();

//NO
void motorRTC();
unsigned char getInfoEEPROM();
unsigned char setTimeRTC();
//unsigned char getTimeRTC(unsigned char *hours, unsigned char *minutes);


#endif	

