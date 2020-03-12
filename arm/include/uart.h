#ifndef __UART__
#define __UART__

#include <avr/io.h>

void uart_init(uint32_t baud);

uint8_t uart_recieve();

#endif
