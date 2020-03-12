#include <avr/io.h>

#include "../include/uart.h"

void uart_init(uint32_t baud)
{
    // Initialize baud
    uint16_t val = (((8000000) / (baud * 16UL)) - 1);
    UBRR0H = (uint8_t)(val >> 8);
    UBRR0L = (uint8_t)(val);

    // Initialize for transmit with interrupt
    UCSR0B = (1 << TXCIE0) | (1 << TXEN0);

    // Initialize 8 bit frame
    UCSR0C = (3 << UCSZ00);
}

void uart_send(uint8_t data)
{
    UDR0 = data;
}
