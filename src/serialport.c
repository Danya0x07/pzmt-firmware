#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <serialport.h>

#define INTERRUPT_ON()  (UCSRB |= _BV(RXCIE))
#define INTERRUPT_OFF() (UCSRB &= ~_BV(RXCIE))

static volatile char rxBuffer[0x10] = {0};
#define INDEX_MASK  (sizeof(rxBuffer) - 1)

static volatile uint8_t count = 0;
static volatile uint8_t writeIndex = 0;
static uint8_t readIndex = 0;

static inline bool BufferEmpty(void)
{
    return count == 0;
}

static inline bool BufferFull(void)
{
    return count == sizeof(rxBuffer);
}

static char PeekLast(void)
{
    if (BufferEmpty())
        return 0;

    uint8_t idx = (writeIndex - 1) & INDEX_MASK;
    return rxBuffer[idx];
}

static inline bool IsPacketTerminator(char c)
{
    return c == '\n' || c == ' ';
}

ISR(USART_RX_vect)
{
    if (!BufferFull()) {
        rxBuffer[writeIndex++] = UDR;
        writeIndex &= INDEX_MASK;
        count++;
    } else {
        (void)UDR;
    }
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

char SerialPort_ReadChar(void)
{
    INTERRUPT_OFF();

    if (BufferEmpty())
        return '\0';
    
    char c = rxBuffer[readIndex++];
    readIndex &= INDEX_MASK;
    count--;

    INTERRUPT_ON();
    return c;
}

bool SerialPort_PacketReceived(void)
{
    char last = PeekLast();

    if (IsPacketTerminator(last))
        return true;

    if (BufferFull())
        SerialPort_Flush();

    return false;
}

uint8_t SerialPort_ReadPacket(char *buff)
{
    uint8_t len = 0;
    
    if (!SerialPort_PacketReceived())
        goto out;

    char c = SerialPort_ReadChar();
    while (!IsPacketTerminator(c)) {
        *buff++ = c;
        len++;
        c = SerialPort_ReadChar();
    }

out:
    *buff = '\0';
    return len;
}

void SerialPort_Flush(void)
{
    INTERRUPT_OFF();
    readIndex = writeIndex = count = 0;
    INTERRUPT_ON();
}