#include "TAD_RTC_var.h"

#include "TAD_I2C.h"
#include "TAD_UART.h"

static unsigned char caseRTC;
DateForRTC date;
DateForUART dateC;
static unsigned char blockRTC;

void init_RTC() 
{
    date.day = 0;
    date.hour = 0;
    date.minutes = 0;
    date.month = 0;
    date.year = 0;
    
    dateC.minutes[0] = 0;
    dateC.minutes[1] = 0;
    dateC.hour[0] = 0;
    dateC.hour[1] = 0;
    
    dateC.year[0] = 0;
    dateC.year[1] = 0;
    dateC.year[2] = 0;
    dateC.year[3] = 0;
    dateC.month[0] = 0;
    dateC.month[1] = 0;
    dateC.day[0] = 0;
    dateC.day[1] = 0;
    
    dateC.seconds[0] = 0;
    dateC.seconds[1] = 0;
    
    caseRTC = 0;
    blockRTC = 0;
}

void setYearToRTC(unsigned char data[]) {
    date.year = 0;
    date.month = 0;
    
    date.year = ((data[2] & 0x0F) << 4) | (data[3] & 0x0F);
    
    if (data[0] == '2') {
        date.month |= (1 << 7);
    }
    
}

void setMonthToRTC(unsigned char data[]) {
    date.month &= 0b10000000;
    date.month |= (0 << 6) | ((data[0] == 1) << 4) | (data[1] & 0x0F);
}

void setDayToRTC(unsigned char data[]) {
    date.day = (0 << 7) | (0 << 6) | ((data[0] & 0x03) << 4) | (data[1] & 0x0F);
}

void setHourToRTC(unsigned char data[]) {
    
    date.hour = 0;
    
    if (data[0] == 2) {
        date.hour |= (1 << 5);
    }
    if (data[0] >= '1') {
        date.hour |= (1 << 4);
    }

    date.hour |= (data[1] & 0x0F);
    
}

void setMinToRTC(unsigned char data[]) {
    date.minutes = ((data[0] << 4) | (data[1] & 0x0F));
}

void setFormatedYear(unsigned char byte) {
    
    dateC.year[2] = (byte >> 4) & 0x0F;
    dateC.year[3] = byte & 0x0F;
    
}

void setFormatedMonth(unsigned char byte) {
    
    if ((byte & 0x80))
        dateC.year[0] = 2;
    else {
        dateC.year[0] = 1;
        dateC.year[1] = 9;
    }
        
    dateC.month[0] = (byte & 0x20);
    dateC.month[1] = byte & 0x0F;
    
}

void setFormatedDay(unsigned char byte) 
{
    dateC.day[0] = (byte >> 4) & 0x03;
    dateC.day[1] = byte & 0x0F;
    
}

void setFormatedHour(unsigned char byte) 
{
    dateC.hour[0] = ((byte & 0x20) ? 2 : (byte & 0x10) ? 1 : 0);
    dateC.hour[1] = byte & 0x0F;
    
}

void setFormatedMin(unsigned char byte) 
{
    dateC.minutes[0] = (byte >> 4) & 0x07;
    dateC.minutes[1] = byte & 0x0F;   
}

void setFormatedSec(unsigned char byte) 
{
    dateC.seconds[0] = (byte >> 4) & 0x07;
    dateC.seconds[1] = byte & 0x0F;   
}

unsigned char getDecimalHour() {
    return dateC.hour[0];
}

unsigned char getUnitHour() {
    return dateC.hour[1];
}

unsigned char getDecMinute() {
    return dateC.minutes[0];
}

unsigned char getUnitMinute() {
    return dateC.minutes[1];
}

unsigned char getDecSecs() {
    return dateC.seconds[0];
}

unsigned char getUnitSecs() {
    return dateC.seconds[1];
}

unsigned char getDecDay() {
    return dateC.day[0];
}

unsigned char getUnitDay() {
    return dateC.day[1];
}

unsigned char getDecMonth() {
    return dateC.month[0];
}

unsigned char getUnitMonth() {
    return dateC.month[1];
}

unsigned char getMilYear() {
    return dateC.year[0];
}

unsigned char getCentYear() {
    return dateC.year[1];
}

unsigned char getDecYear() {
    return dateC.year[2];
}

unsigned char getUnitYear() {
    return dateC.year[3];
}


unsigned char resetAlarmMotor() {
    
    static unsigned char stAlarm = 0;
    
    switch(stAlarm) {
        case 0:
            blockRTC = 1;
            if (StartI2C(ADD_WR))
                stAlarm++;
            break;
        case 1:
            if (I2C_Write(0x0F))
                stAlarm++;
            break;
        case 2:
            if (I2C_Write(0x00))
                stAlarm++;
            break;
        case 3:
            if (I2C_Stop_()) {
                stAlarm = 0;
                blockRTC = 0;
                return 1;
            }
            break;
    }
    
    return 0;
}

unsigned char WriteRTCinfo(unsigned char initialInput) {
    
    static unsigned char stWrRTC = 0;
    
    switch (stWrRTC) {
        case 0:
            blockRTC = 1;
            if(StartI2C(ADD_WR))
                stWrRTC++;
            break;
        case 1:
            if(I2C_Write(0x00))
                stWrRTC++;
            break;
        case 2:
            if(I2C_Write(0x00))
                stWrRTC++;
            break;
        case 3:
            if(I2C_Write(date.minutes))
                stWrRTC++;
            break;
        case 4:
            if(I2C_Write(date.hour)) {
                if (initialInput == 1) {
                    stWrRTC++;
                } else {
                    stWrRTC = 9;
                }
            }
            break;
        case 5:
            if(I2C_Write(0x00))
                stWrRTC++;
            break;
        case 6:
            if(I2C_Write(date.day))
                stWrRTC++;
            break;
        case 7:
            if(I2C_Write(date.month))
                stWrRTC++;
            break;
        case 8:
            if(I2C_Write(date.year))
                stWrRTC++;
            break;
        case 9:
            if(I2C_Stop_()) {
                blockRTC = 0;
                stWrRTC = 0;
                return 1;
            }
            break;
    }
    
    return 0;
}

unsigned char startRTCAlarm() {
    
    static unsigned char stAlarmRTC = 0;
    
    switch (stAlarmRTC) {
        case 0:
            blockRTC = 1;
            if(StartI2C(ADD_WR))
                stAlarmRTC++;
            break;
        case 1:
            if(I2C_Write(0x0B))
                stAlarmRTC++;
            break;
        case 2:
            if(I2C_Write(0x80))
                stAlarmRTC++;
            break;
        case 3:
            if(I2C_Write(0x80))
                stAlarmRTC++;
            break;
        case 4:
            if(I2C_Write(0x80))
                stAlarmRTC++;
            break;
        case 5:
            if(I2C_Stop_()) {
                blockRTC = 0;
                stAlarmRTC = 0;
                return 1;
            }
            break;
    }
    
    
}

unsigned char readRTC() {
    
    blockRTC = 1;
    
    static unsigned char done = 0;
    static unsigned char data;
    
    while(!StartI2C(ADD_WR)){}
    while(!I2C_Write(0x00)){}
    
    
    while(!ReStartI2C_(ADD_RD)){}
    
    
    done = 0;
    
    while(!done) {
        data = I2C_Read(CONT, &done);
    }
    
    setFormatedSec(data);
    
    
    done = 0;
    
    while(!done) {
        data = I2C_Read(CONT, &done);
    }
    
    setFormatedMin(data);
    
    //hours
    done = 0;
    
    while(!done) {
        data = I2C_Read(CONT, &done);
    }
    
    setFormatedHour(data); 
    
    
    done = 0;
    
    //day of week
    while(!done) {
        data = I2C_Read(CONT, &done);
    }
   
    
    done = 0;
    
    while(!done) {
        data = I2C_Read(CONT, &done);
    }
    
    setFormatedDay(data);  
    
    done = 0;
    
    while(!done) {
        data = I2C_Read(CONT, &done);
    }
    
    setFormatedMonth(data);
    
    
    done = 0;
    
    while(!done) {
        data = I2C_Read(STOP, &done);
    }
    
    setFormatedYear(data);
    
    blockRTC = 0;
    
    return 1;
}

unsigned char RTCisAvail() {
    return !blockRTC;
}