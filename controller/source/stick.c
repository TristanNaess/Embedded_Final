#include "../include/stick.h"

void ADC_init()
{
    ADMUX = (1 << ADLAR); // remove bottom two bits
    ADCSRA |= (1 << ADEN) | (1 << ADIE); // enable ADC, enable interrupts
    adc_i = 0; // select first adc
    ADCSRA |= (1 << ADSC); // start conversion
    SREG |= 0x80;
}

#define ADC_C 4

ISR(ADC_vect)
{
   *(adc[adc_i]) = ADCH;
   adc_i++;
   if (adc_i == ADC_C)
   {
       adc_i = 0;
   }
   ADMUX = (ADMUX & 0xf0) | adc_i;
   ADCSRA |= (1 << ADSC);
}

// Construct stick and set ports
Stick * make_stick()
{
    // create object and ports
    Stick * s = malloc(sizeof(Stick));

    // initial values
    s->x = 0;
    s->y = 0;
    
    // zero to be calibrated
    s->x_min = 0;
    s->x_max = 0;

    s->y_min = 0;
    s->y_max = 0;
    return s;
}

// zero stick and bounds
void zero_stick(Stick * s)
{
    s->x_zero = s->x;
    s->y_zero = s->y;
}

// set divide bounds into four regions (to match stepper speeds)
void set_div(Stick * s)
{
    s->x_div = (s->x_max - s->x_min) / 7;
    s->y_div = (s->y_max - s->y_min) / 7;
}

// get value, [-4,4] of axis
uint8_t get_region(Stick * s, uint8_t axis)
{
    if (axis) // y
    {
        if (s->y > s->y_zero + (s->y_div >> 1)) // positive
        {
            if (s->y < s->y_zero + (1 * s->y_div))
                return 5;
            else if (s->y < s->y_zero + (2 * s->y_div))
                return 6;
            else if (s->y < s->y_zero + (3 * s->y_div))
                return 7;
            else
                return 8;
        }
        else if (s->y < s->y_zero - (s->y_div >> 1)) // negative
        {
            if (s->y > s->y_zero - (1 * s->y_div))
                return 3;
            else if (s->y > s->y_zero - (2 * s->y_div))
                return 2;
            else if (s->y > s->y_zero - (3 * s->y_div))
                return 1;
            else
                return 0;
        }
        else // zero
            return 4;
    }
    else // x
    {
        if (s->x > s->x_zero + (s->x_div >> 1)) // positive
        {
            if (s->x < s->x_zero + (1 * s->x_div))
                return 5;
            else if (s->x < s->x_zero + (2 * s->x_div))
                return 6;
            else if (s->x < s->x_zero + (3 * s->x_div))
                return 7;
            else
                return 8;
        }
        else if (s->x < s->x_zero - (s->x_div >> 1)) // negative
        {
            if (s->x > s->x_zero - (1 * s->x_div))
                return 3;
            else if (s->x > s->x_zero - (2 * s->x_div))
                return 2;
            else if (s->x > s->x_zero - (3 * s->x_div))
                return 1;
            else
                return 0;
        }
        else // zero
            return 4;
    }
}

// check that mins and maxes are more extreme than current values
void check_bound(Stick * s)
{
    if (s->x > s->x_max)
        s->x_max = s->x;
    if (s->x < s->x_min)
        s->x_min = s->x;
    if (s->y > s->y_max)
        s->y_max = s->y;
    if (s->y < s->y_min)
        s->y_min = s->y;
}
