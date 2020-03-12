#ifndef __TIMER__
#define __TIMER__

#include <avr/interrupt.h>

void timer_init();

// Requires user to define ISR(TIMER1_COMPA_vect) with scaling

#endif // __TIMER__
