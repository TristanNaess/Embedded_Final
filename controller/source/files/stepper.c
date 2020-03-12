#ifndef __STEPPER__
#define __STEPPER__

#include "../include/stepper.h"

Stepper * make_stepper(uint8_t * _port, uint8_t _nibble, uint16_t _min, uint16_t _max)
{
    Stepper * s = malloc(sizeof(Stepper));
    s->port = _port;
    s->nibble = _nibble;
    s->min = _min;
    s->pos = 0;
    s->max = _max;
    s->step = steps; // initialize to steps first element
    s->speed = 0;
    s->counter = 0;
}

void step(Stepper * s)
{
    if ((s->pos < s->max) && (s->speed > 0))
    {
        s->pos++;
        s->step++;
        if (s->step > STEPS)
            s->step = 0;
    }
    if ((s->pos > s->min) && (s->speed < 0))
    {
        s->pos--;
        s->step--;
        if (s->step == 255) // s = -1
            s->step = &steps[STEPS - 1];
    }
    *(s->port) = (*(s->port) & (0xF << s->nibble)) | s->step;
}


#endif // __STEPPER__
