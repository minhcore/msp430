#include <msp430.h>

#define LED_PIN BIT6

int main (void) 
{
    WDTCTL = WDTPW + WDTHOLD;
    P1DIR = LED_PIN;
    P1OUT = LED_PIN;

    while (1)
    {
        P1OUT ^= LED_PIN;
        __delay_cycles(500000);
    }
}