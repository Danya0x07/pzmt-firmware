#ifndef _SERIALPORT_H
#define _SERIALPORT_H

#include <main.h>

void SerialPort_Init(void);
void SerialPort_PrintChar(char c);
void SerialPort_PrintString(const char *str);
void SerialPort_PrintDecimal(int16_t n);
char SerialPort_ReadChar(void);
bool SerialPort_PacketReceived(void);
uint8_t SerialPort_ReadPacket(char *buff);
void SerialPort_Flush(void);

#endif // _SERIALPORT_H