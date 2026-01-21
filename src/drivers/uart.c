#include "uart.h"

#include <msp430.h>

void uart_init(void)
{
    UCA0CTL1 |= UCSWRST;
    // Use SMCLK clock
    UCA0CTL1 |= UCSSEL_2;
    // Use 16 Mhz clock and 115200 baudrate
    UCA0BR0 = 8;
    UCA0BR1 = 0;
    UCA0MCTL = UCBRS_0 + UCBRF_11 + UCOS16;

    UCA0CTL1 &= ~UCSWRST;

    // use polling first => no enable interrupt
}

void uart_put_char(char c)
{
    while (!(IFG2 & UCA0TXIFG)) { };
    UCA0TXBUF = c;
}

void uart_put_string(char *str)
{
    while (*str) {
        uart_put_char(*str);
        str++;
    }
}
