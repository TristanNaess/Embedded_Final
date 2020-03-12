#include <avr/io.h>
#include <avr/interrupt.h>

#include "../include/stick.h"
#include "../include/uart.h"

// specifies which value, upper nibble, and value, lower nibble
// 0: right stick x
// 1: right stick y
// 2: left  stick x
// 3: left  stick y
// 4: buttons
uint8_t current_message = 0;
uint8_t messages[] = {0x04, 0x14, 0x24, 0x34, 0x41}; // all set to zero position

ISR(USART0_TX_vect)
{
    uart_send(messages[current_message++]); // send next message, and increment
    if (current_message > 4)
        current_message = 0; // loop back
}

enum States {INIT, CALIBRATE, READ} state;

Stick * sticks[2];

void tick()
{
    switch (state)
    {
        case INIT:
            // create stick objects
            sticks[0] = make_stick();   // right stick: elbow and wrist
            sticks[1] = make_stick();   // left stick:  base and shoulder

            // attach ADC output to sticks (sticks mounted sideways, x and y switched to match controller layout)
            adc[0] = &(sticks[0]->x);   // elbow
            adc[1] = &(sticks[0]->y);   // wrist
            adc[2] = &(sticks[1]->x);   // base
            adc[3] = &(sticks[1]->y);   // shoulder
            
            // Initialize ADC, start conversion
            ADC_init();
            int i;

            // delay 1 ms to read all sticks
            for (i = 0; i < 775; i++)
            {
                asm("nop");
            }

            // set stick zeros
            zero_stick(sticks[0]);
            zero_stick(sticks[1]);

            // init UART0
            uart_init(38400); // defined baud rate

            state = CALIBRATE;
            break;

        case CALIBRATE:
            check_bound(sticks[0]);     // set new bound if greater
            check_bound(sticks[1]);
            if ((~PINC) & 0x03)        // both buttons pressed
            {
                set_div(sticks[0]);     // calculate division widths
                set_div(sticks[1]);
                state = READ;
            }
            break;

        case READ:
            // update output values
            messages[0] = (messages[0] & 0xf0) | get_region(sticks[0], X_AXIS);
            messages[1] = (messages[1] & 0xf0) | get_region(sticks[0], Y_AXIS);
            messages[2] = (messages[2] & 0xf0) | get_region(sticks[1], X_AXIS);
            messages[3] = (messages[3] & 0xf0) | get_region(sticks[1], Y_AXIS);
            if ((~PINC) == 1)
                messages[4] = (messages[4] & 0xf0);     // 0 is left  button            (close)
            if ((~PINC) == 2)
                messages[4] = (messages[4] & 0xf0) | 2; // 2 is right button            (open)
            else
                messages[4] = (messages[4] & 0xf0) | 1; // 1 is both or neither  button (nothing)
            
            // messages are sent by interupt

            break;
        default:
            state = READ;
    }
}



int main(void)
{
    // port init
    // ADC_init(); called in INIT state
    DDRC = 0xfc; PORTC = 0x03; // initalize PORTC for buttons

    while (1) {  } // all code handled by interrupts
    return 1;
}
