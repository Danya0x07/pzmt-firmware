#ifndef _SERIALPORT_H
#define _SERIALPORT_H

#include <main.h>

void SerialPort_Init(void);
void SerialPort_PrintChar(char c);
void SerialPort_PrintString(const char *str);
void SerialPort_PrintDecimal(int16_t n);
char SerialPort_ReadChar(void);
bool SerialPort_LineReceived(void);
uint8_t SerialPort_ReadLine(char *buff);

#endif // _SERIALPORT_H