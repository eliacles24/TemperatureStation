#include "TAD_EEPROM.h"

#include "TAD_UART.h"

static unsigned char EEPROM_available;

static unsigned char head;
static unsigned char tail;
static unsigned char entries;

void init_EEPROM()
{
	EECON1bits.CFGS = 0;
	EECON1bits.EEPGD = 0;
	EEPROM_available = TRUE;
    
    head = 1;
    tail = readEEPROM(0);
}

unsigned char isEEPROMAvail()
{
	return EEPROM_available;
}

unsigned char readEEPROM(unsigned char address)
{
	unsigned char data_read;
	
	EEADR = address;
	EECON1bits.RD = 1;
	data_read = EEDATA;
	
	return data_read;
}

unsigned char writeEEPROM(unsigned char data, unsigned char address)
{
    
    static unsigned char stateWr = 0;
    
	switch(stateWr) {
		case 0:
            EEADR = address;
            EEDATA = data;
	
            EECON1bits.WREN = 1;
            INTCONbits.GIE = 0;             //Deactivate interrupts
	
            EECON2 = 0x055;
            EECON2 = 0x0AA;
	
            EECON1bits.WR = 1;
            stateWr++;
			break;
		case 1:
			if (EECON1bits.WR != 1) {
                INTCONbits.GIE = 1;         //Activate interrupts
                EECON1bits.WREN = 0;
                stateWr--;
                return 1;
            }
			break;
	}
	
	return 0;
}

unsigned char writePhraseEEPROM(unsigned char data[], unsigned char size) {
    
    static unsigned char read = 0;
    static unsigned char stWritePhrase = 0;
    
    switch (stWritePhrase) {
        case 0:
            EEPROM_available = FALSE;
    
        if(writeEEPROM(data[read], tail) == 1){
            tail++;        
            read++;
        }
    
        if (tail+1 == MAX_ADD) {
            tail = 1;
        }
        
        if(read > size) {
            read = 0;
            stWritePhrase++;
        }
            
            break;
        case 1:
            if(writeEEPROM(tail, 0) == 1) {
                EEPROM_available = TRUE;
                entries++;
                stWritePhrase--;
                return 1;
            }
            break;
    }
    
    
        
    return 0;
}

unsigned char readEEPROMPhase() {
    
    static unsigned char stRdEeprom = 0;
    static unsigned char formatedData[] = "Critical state: 00-00-0000 at 00:00:00\r\n";
    static unsigned char readEntries = 0;
    static unsigned char pos;
    
    unsigned char value;
    
    switch(stRdEeprom) {
        case 0:
            head = 1;
            
            if (head == tail) {
                return 1;
            } else {
                stRdEeprom++;
                EEPROM_available = FALSE;
                pos = 30;
            }
            break;
        case 1:
            
            value = readEEPROM(head);
            formatedData[pos] = value;
            
            if (pos == 17 || pos == 20 || pos == 31 || pos == 34) {
                pos += 2;
            } else if (pos == 37) {
                pos = 16;
            } else {
                pos++;
            }
            
            if (pos == 26) {
                stRdEeprom++;
                pos = 30;
            }
    
            head++;
        
            if (head+1 == MAX_ADD) {
                head = 1;
            }
            
            break;
        case 2:
            if(SIO_PutString(formatedData)) {
                
                if(head == tail) {
                    EEPROM_available = TRUE;
                    stRdEeprom = 0;
                    return 1;
                } else {
                    stRdEeprom--;
                }
            }
            break;
    
    }
    
    return 0;
}

unsigned char resetEEPROM() {
    EEPROM_available = TRUE;
    
    head = 1;
    tail = 1;
    
    if (writeEEPROM(1, 0)){ //Escriure 1 a la adreça on guardem el punter
        return 1;
    } else {
        return 0;
    }   
}