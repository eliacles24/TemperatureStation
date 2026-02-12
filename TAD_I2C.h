#ifndef _TAD_I2C_H
#define _TAD_I2C_H

/* pin out
SDA: Serial Data  PIN C4
SCL: Serial Clock PIN C3
SQW: Active low interrupt (if used connect to the desired pin)
32KHZ: not connected
GND
VCC: min=2.3V | typ = 3.3V | max = 5.5V
*/

#define BAUDRATE 99

//-------------- Private functions: --------------
unsigned char I2C_Ready();
char I2C_Ack();
char I2C_Nack();

//-------------- Public functions: --------------
void InitI2C();
char StartI2C(char slave_rw_addr);
char ReStartI2C_(char slave_rw_addr);
char I2C_Stop_(void);

char I2C_Write(unsigned char data);
//Post: Sends information using i2C to the DS3231 chip (RTC)

char I2C_Read(char flag, char* done);
//Post: Reads data sent from I2C communication, and returns byte

void setRTCAlarms();

#endif
