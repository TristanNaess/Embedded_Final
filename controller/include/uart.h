#ifndef __UART__
#define __UART__

#include <avr/io.h>

void uart_init(uint32_t baud);

void uart_send(uint8_t data);

#endif
