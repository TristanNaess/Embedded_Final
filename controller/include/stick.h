#ifndef __STICK__
#define __STICK__

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

typedef struct _stick
{
    uint8_t x, y;                   // x and y position
    uint8_t x_min, x_zero, x_max;   // x limits and zero
    uint8_t y_min, y_zero, y_max;   // y limits and zero
    uint8_t x_div, y_div;           // x and y region size for stepped output

} Stick;

// Construct stick and set ports
Stick * make_stick();

// check that mins and maxes are more extreme than current values
void check_bound(Stick * s);

// set divide bounds into four regions (to match stepper speeds)
void set_div(Stick * s);

// get value, [-4,4] of axis
#define X_AXIS 0
#define Y_AXIS 1
uint8_t get_region(Stick * s, uint8_t axis);

// set x, y-zero and zero bounds
void zero_stick(Stick * s);

// ADC storage; pointers to x and y in Sticks
uint8_t * adc[8];
uint8_t adc_i;

// setup ADC
void ADC_init();

#endif // __STICK__
