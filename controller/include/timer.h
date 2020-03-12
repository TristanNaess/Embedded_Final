#ifndef __TIMER__
#define __TIMER__

#include <avr/interrupt.h>

void timer_init()
{
    TCCR1B = 0x0B;  // CTC mode, prescaler 64 125,000 ticks / s
    OCR1A = 125;    // count 125 ticks before interrupt
    TIMSK1 = 0x02;  // compare match interrupt enable

    TCNT1 = 0;      // timer init

    SREG |= 0x80;   // interupt enable
}

// Requires user to define ISR(TIMER1_COMPA_vect) with scaling

#endif // __TIMER__
