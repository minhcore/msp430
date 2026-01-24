#include "assert_handler.h"
#include "defines.h"
#include <msp430.h>

#define BREAKPOINT __asm volatile("CLR.B R3");

void assert_handler(void)
{
    BREAKPOINT

    // Configure TEST LED
    P1SEL &= ~(BIT0);
    P1SEL2 &= ~(BIT0);
    P1DIR |= BIT0;
    P1OUT &= ~(BIT0);

    while (1) {
        P1OUT ^= BIT0;
        BUSY_WAIT_ms(100);
    }
}