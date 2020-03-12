#include <avr/io.h>
#include <avr/interrupt.h>

#include "../include/stick.h"

enum States {INIT, CALIBRATE, READ} state;

Stick * stick;

void tick()
{
    switch (state)
    {
        case INIT:
            stick = make_stick();       // create stick
            adc[0] = &stick->y;         // attach adc to stick
            adc[1] = &stick->x;
            ADC_init();                 // initialize ADC and start conversion
            int i;
            for (i = 0; i < 750; i++)   // delay for ADC to read zeros
            {
                asm("nop");
            }
            zero_stick(stick);          // set zero positions
            state = CALIBRATE;
            break;
        case CALIBRATE:
            check_bound(stick);
            if ((~PIND) & 0x80)         // finish calibration
            {
                set_div(stick);
                state = READ;
            }
            break;
        case READ:
            PORTB = get_region(stick, Y_AXIS) & 0x0f;
            PORTB |= (get_region(stick, X_AXIS) << 4) & 0xf0;
            break;
        default:
            state = READ;
    }
}

int main(void)
{
    // PORT initializations
    DDRB = 0xff; PORTB = 0x00; // LED out
    DDRD = 0x3f; PORTD = 0xc0; // input select and calibrate 


    while (1)
    {
        tick();
    }
    return 1;
}
