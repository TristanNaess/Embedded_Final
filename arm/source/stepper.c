#include <stdlib.h>

#include "../include/stepper.h"

Stepper * make_stepper(volatile uint8_t * _port, uint8_t _nibble, uint16_t _min, uint16_t _max)
{
    Stepper * s = malloc(sizeof(Stepper));
    s->port = _port;
    s->nibble = _nibble;
    s->min = _min;
    s->pos = 0;
    s->max = _max;
    s->step = 0;
    s->speed = 0;
    s->counter = 0;

    return s;
}

void step(Stepper * s)
{
    if ((s->pos < s->max) && (s->speed > 0))
    {
        s->pos++;
        s->step++;
        if (s->step > STEP_C)
            s->step = 0;
    }
    if ((s->pos > s->min) && (s->speed < 0))
    {
        s->pos--;
        s->step--;
        if (s->step == 255) // s = -1
            s->step = STEP_C - 1;
    }
    *(s->port) = (*(s->port) & (0xF << s->nibble)) | steps[s->step];
}

