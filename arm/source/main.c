#include <avr/io.h>
#include <avr/interrupt.h>

#include "../include/stepper.h"
#include "../include/timer.h"
#include "../include/uart.h"

#define STEPPER_C 5
Stepper * steppers[5];

// called once a millisecond
uint8_t s_i;
ISR(TIMER0_COMPA_vect)
{
    for (s_i = 0; s_i < STEPPER_C; s_i++)
    {
        if (steppers[s_i]->speed != 0)
        {
            if (++(steppers[s_i]->counter) > steppers[s_i]->speed)
            {
                steppers[s_i]->counter = 0;
                step(steppers[s_i]);
            }
        }
    }
}

ISR(USART0_RX_vect)
{
    uint8_t val = UDR0;
    if ((val & 0xf0) == 0x00)
    {
        steppers[3]->speed = (val & 0x0f);
    }
    if ((val & 0xf0) == 0x10)
    {
        steppers[4]->speed = (val & 0x0f);
    }
    if ((val & 0xf0) == 0x20)
    {
        steppers[0]->speed = (val & 0x0f);
    }
    if ((val & 0xf0) == 0x30)
    {
        steppers[1]->speed = (val & 0x0f);
    }
    if ((val & 0xf0) == 0x40)
    {
        steppers[4]->speed = (val & 0x0f) + 3; // offset 0-2 to 3-5
    }
}

int main(void)
{
    steppers[0] = make_stepper(&PORTB, LOW_NIB,  DEG_90_N, DEG_90); // Base
    steppers[1] = make_stepper(&PORTB, HIGH_NIB, DEG_45_N, DEG_45); // Shoulder
    steppers[2] = make_stepper(&PORTA, LOW_NIB,  DEG_60_N, DEG_60); // Elbow
    steppers[3] = make_stepper(&PORTA, HIGH_NIB, DEG_60_N, DEG_60); // Wrist
    steppers[4] = make_stepper(&PORTC, LOW_NIB,  0,        DEG_90); // Claw

    timer_init();
    uart_init(38400);

    while(1){  }
    return 1;
}
