#ifndef __STEPPER__
#define __STEPPER__

#include <avr/io.h>

// array of values passed to port for each step
// 4076 steps per revolution
#define STEP_C 8
const uint8_t steps[] = {0x01, 0x03, 0x02, 0x06, 0x04, 0x0C, 0x08, 0x09};

// Common angles for bounds in steps
#define DEG_90    2038
#define DEG_90_N -2038
#define DEG_60    1359
#define DEG_60_N -1359
#define DEG_45    1019
#define DEG_45_N -1019

// delays per step; 0 denotes no change
const uint8_t speeds[] = {-10, -40, -70, -100, 0, 100, 70, 40, 10};

// offsets for bitshifting
#define HIGH_NIB 0
#define LOW_NIB 4

// struct for holding stepper data
typedef struct _stepper
{
    volatile uint8_t * port;     // port where stepper is attached
    volatile uint8_t nibble;     // uses defined HIGH and LOW to set offsets
    volatile uint8_t step;       // current step
    volatile int16_t pos;        // stores displacement in steps. 0 is default
    volatile int16_t min, max;   // minimum and maximum positions in steps
    volatile uint8_t counter;
    volatile uint8_t speed;

} Stepper;

// build stepper and reutrn pointer
Stepper * make_stepper(volatile uint8_t * _port, uint8_t _nibble, uint16_t _min, uint16_t _max);

// step each direction
void step(Stepper * s);

#endif // __STEPPER__
