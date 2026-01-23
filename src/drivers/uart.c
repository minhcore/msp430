#include "uart.h"

#include <msp430.h>
#include <stdint.h>

volatile char buffer[64];
volatile uint8_t buffer_idx = 0;

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

void uart_put_string(char const *str)
{
    while (*str) {
        uart_put_char(*str);
        str++;
    }
}

void uart_get_char(void)
{
    while (!(IFG2 & UCA0RXIFG))
        ;
    char c = UCA0RXBUF;
    if (buffer_idx < 64) {
        buffer[buffer_idx++] = c;
    }
}
