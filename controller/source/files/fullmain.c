#include <avr/io.h>
#include "../include/stick.h"
#include "../include/stepper.h"
#include "../include/timer.h"


ISR(TIMER_COMPA_vect)
{
    steppers[0]->speed = speeds[get_region(sticks[0], X_AXIS)];
    steppers[1]->speed = speeds[get_region(sticks[0], Y_AXIS)];
    steppers[0]->counter++;
    steppers[1]->counter++;
    if (steppers[0]->counter >= steppers[0]->speed)
    {
        step(steppers[0]);
        steppers[0]->count = 0;
    }
    if (steppers[1]->counter >= steppers[1]->speed)
    {
        step(steppers[1]);
        steppers[1]->count = 0;
    }
}

#define ADC_PINS_USED 2

adc_i = 0;                          // index of ADC polling
ISR(ADC_vect)                       // runs every time a conversion is complete
{
    *(adc[adc_i++]) = ADCH;         // write current conversion to pointer

    if (adc_i == ADC_PINS_USED)     // loop back if last pin used
        adc_i = 0;

    ADMUX = (ADMUX & 0xF0) | adc_i; // set next conversion
    ADCSRA |= (1 << ADSC);          // start next conversion
}

int main(void)
{
    Stepper * steppers[2];
    Stick * sticks[1];

    steppers[0] = make_stepper(&PORTB, LOW_NIB, DEG_90_N, DEG_90);
    steppers[1] = make_stepper(&PORTB, HIGH_NIB, DEG_90_N, DEG_90);
    sticks[0] = make_stick();

    // attach adc storage pointers to stick struct
    adc[0] = &sticks[0]->x;
    adc[1] = &sticks[0]->y;

    // make_stepper() does not do port initialization
    DDRB = 0xff; PORTB = 0x00;

    timer_init();           // timer interrupts every 1ms

    ADCSRA |= (1 << ADSC);  // begin first conversion

    while(1) {  }
    return 0;
}
