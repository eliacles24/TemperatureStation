#include "TAD_TMP_manager.h"

#include "TAD_TIMER.h"
#include "TAD_ADC.h"
#include "TAD_LED.h"
#include "TAD_FANS.h"
#include "TAD_RAM.h"
#include "TAD_EEPROM.h"
#include "TAD_RTC_var.h"

#include "TAD_UART.h"

static unsigned char t_low;
static unsigned char t_mod;
static unsigned char t_high;
static unsigned char t_crit;

static unsigned char stTMP;
unsigned char TMPtmr;
static unsigned char temperatureTime = 0;       //The time (in seconds) between temperature reads
static unsigned char criticalPWM;               //The timer for the LED in critical state ( RED <-> Magenta) 

void initTMPMan()
{
    t_low = 0;
    t_mod = 0;
    t_high = 0;
    t_crit = 0;
    temperatureTime = 1;
    
    stTMP = 0;
    TI_NewTimer(&TMPtmr);
    TI_ResetTics(TMPtmr);
    
    TI_NewTimer(&criticalPWM);
    TI_ResetTics(criticalPWM);
    
    TRISCbits.TRISC5 = 0;
    LATCbits.LATC5 = 0;
    
}

void setThresholdLow(unsigned char temp[])
{
    unsigned char i = 0;
    t_low = 0;
    
    while(temp[i] != '\0') {
        t_low = temp[i] - '0' + (t_low*10);
        i++;
    }
    
}

void setThresholdMod(unsigned char temp[])
{
    unsigned char i = 0;
    t_mod = 0;
    
    while(temp[i] != '\0') {
        t_mod = temp[i] - '0' + (t_mod*10);
        i++;
    }
    
}

void setThresholdHigh(unsigned char temp[])
{
    unsigned char i = 0;
    t_high = 0;
    
    while(temp[i] != '\0') {
        t_high = temp[i] - '0' + (t_high*10);
        i++;
    }
    
}
void setThresholdCrit(unsigned char temp[])
{
    unsigned char i = 0;
    t_crit = 0;
    
    while(temp[i] != '\0') {
        t_crit = temp[i] - '0' + (t_crit*10);
        i++;
    }
    
}

void setPollingRate(unsigned char time[]) {
    
    unsigned char i = 0;
    temperatureTime = 0;
    
    while(time[i] != '\0') {
        temperatureTime = time[i] - '0' + (temperatureTime*10);
        i++;
    }
    
}


void motorTemp() 
{
    static int temp;
    unsigned int num = 0;
    static unsigned char variable = 0;
    
    static unsigned char phEeprom[] = "00000000000000";
    
    switch(stTMP) {
        
        case 0:
            
            if (temperatureTime != 0) {
                TI_ResetTics(TMPtmr);
                stTMP++;
            }
            break;
        case 1:
            
            if (TI_GetTics(TMPtmr) > (temperatureTime*2000)) {
                stTMP++;
            }
            
            break;
        case 2:      
            
            temp = getTemp() >> 1;
            temp = temp - 58;
            stTMP++;
            break;
            
        case 3:
            
            if (writeToRam(temp) == 1) {
                stTMP++;
            }
            break;
        case 4:
            if (temp <= t_low) {
                stTMP = 5;
            } else if (temp <= t_mod) {
                stTMP = 6;

            } else if (temp <= t_high) {
                stTMP = 7;

            } else if (temp <= t_crit) {
                stTMP = 8;
            } else {
                recieve_temp(6);
                stTMP = 0;
            }
            
            break;
        case 5:             //low
            recieve_temp(LOW_TEMP);
            setFansState(LOW_STATE);
            TI_ResetTics(TMPtmr);
            stTMP = 0;
            
            break;
        case 6:             //moderate
            
            recieve_temp(MOD_TEMP);
            setFansState(MODERATE_STATE);
            TI_ResetTics(TMPtmr);
            stTMP = 0;
            break;   
        case 7:             //high
            setFansState(HIGH_STATE);
            recieve_temp(HIGH_TEMP);
            stTMP = 0;
            break;
        case 8:             //critical
            setFansState(CRITICAL_STATE);
            recieve_temp(CRITICAL_TEMP);
            stTMP++;
            break;
        case 9:
            if (readRTC())
                stTMP++;
            break;
        case 10:
            
            phEeprom[0] = '0' + getDecimalHour();
            phEeprom[1] = '0' + getUnitHour();
            phEeprom[2] = '0' + getDecMinute();
            phEeprom[3] = '0' + getUnitMinute();
            stTMP++;
            break;
        case 11:
            
            phEeprom[4] = '0' + getDecSecs();
            phEeprom[5] = '0' + getUnitSecs();
            phEeprom[6] = '0' + getDecDay();
            phEeprom[7] = '0' + getUnitDay();
            stTMP++;
            
            break;
        case 12:
            
            phEeprom[8] = '0' + getDecMonth();
            phEeprom[9] = '0' + getUnitMonth();
            phEeprom[10] = '0' + getMilYear();
            phEeprom[11] = '0' + getCentYear();
            phEeprom[12] = '0' + getDecYear();
            phEeprom[13] = '0' + getUnitYear();
            stTMP++;
            
            break;
        case 13:
            if (isEEPROMAvail())
                stTMP++;
            break;
        case 14:
            if (writePhraseEEPROM(phEeprom, 13))
                stTMP = 0;
            break;
    }
    
}

unsigned char resetTemp() {
    static unsigned char stReset = 0;
    
    switch (stReset) {
        case 0:
            t_low = 0;
            t_mod = 0;
            t_high = 0;
            t_crit = 0;
            temperatureTime = 0;
            stReset++;
            break;
        case 1:
            setFansState(CRITICAL_STATE);
            stReset++;
            break;
        case 2:
            recieve_temp(0);
            stReset = 0;
            return 1;
    }
    
    return 0;
    
}