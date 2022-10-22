#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <serialport.h>

static char rxBuffer[0x10] = {0};  // 16 bytes buffer, index mask 0xF
static volatile bool empty = true;
static volatile uint8_t writeIndex = 0;
static uint8_t readIndex = 0;

static char numBuffer[8];

static char _PeekLast(void)
{
    if (empty)
        return 0;

    uint8_t idx = (writeIndex - 1) & 0xF;
    return rxBuffer[idx];
}

static char _PeekFirst(void)
{
    return empty ? 0 : rxBuffer[readIndex];
}

void _FlushInputBuffer(void)
{
    memset(rxBuffer, 0, sizeof(rxBuffer));
    empty = true;
}

ISR(USART_RX_vect)
{
    rxBuffer[writeIndex++] = UDR;
    writeIndex &= 0xF;
    empty = false;
}

void SerialPort_Init(void)
{
    UBRRH = (uint8_t)(51 >> 8);  // baud: 9600
    UBRRL = (uint8_t)51;
    UCSRC = _BV(UCSZ1) | _BV(UCSZ0);
    UCSRB = _BV(TXEN) | _BV(RXEN) | _BV(RXCIE);
}

void SerialPort_PrintChar(char c)
{
    loop_until_bit_is_set(UCSRA, UDRE);
    UDR = c;
}

void SerialPort_PrintString(const char *str)
{
    while (*str)
        SerialPort_PrintChar(*str++);
}

void SerialPort_PrintDecimal(int16_t n)
{
    SerialPort_PrintString(itoa(n, numBuffer, 10));
}

char SerialPort_ReadChar(void)
{
    if (empty)
        return '\0';
    
    char c = rxBuffer[readIndex++];
    readIndex &= 0xF;
    if (readIndex == writeIndex)
        empty = true;
    return c;
}

bool SerialPort_LineReceived(void)
{
    return _PeekLast() == '\n';
}

uint8_t SerialPort_ReadLine(char *buff)
{
    if (!SerialPort_LineReceived()) {
        buff = NULL;
        return 0;
    }

    uint8_t len = 0;
    char c;
    while ((c = SerialPort_ReadChar()) != '\n') {
        *buff++ = c;
        len++;
    }
    *buff = '\0';
    return len;
}
