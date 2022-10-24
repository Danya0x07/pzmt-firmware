#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <serialport.h>
#include <cbuffer.h>

static char memblock[16];
static struct CircularBuffer buffer = {
    .memblock = memblock, .elementSize = 1, .len = sizeof(memblock)
};

static char _PeekNewest(void)
{
    char c = '\0';

    CircularBuffer_PeekNewest(&buffer, &c);
    return c;
}

static char _PeekOldest(void)
{
    char c = '\0';

    CircularBuffer_PeekOldest(&buffer, &c);
    return c;
}

ISR(USART_RX_vect)
{
    uint8_t byte = UDR;
    CircularBuffer_Add(&buffer, &byte);
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
    SerialPort_PrintString(itoa(n, memblock, 10));
}

char SerialPort_ReadChar(void)
{
    char c;

    if (CircularBuffer_Get(&buffer, &c) < 0)
        return 0;

    return c;
}

bool SerialPort_LineReceived(void)
{
    return _PeekNewest() == '\n';
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

void SerialPort_Flush(void)
{
    CircularBuffer_Reset(&buffer);
}
