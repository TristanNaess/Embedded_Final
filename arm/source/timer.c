#include "../include/timer.h"

void timer_init()
{
    TCCR1B = 0x0B;  // CTC mode, prescaler 64 125,000 counts / s
    OCR1A = 125;    // count 125 ticks before interrupt (1ms tick period)
    TIMSK1 = 0x02;  // compare match interrupt enable

    TCNT1 = 0;      // timer init

    SREG |= 0x80;   // interupt enable
}
