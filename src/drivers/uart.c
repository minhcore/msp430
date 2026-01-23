#include "uart.h"
#include "common/ring_buffer.h"
#include "common/defines.h"

#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

#define UART_BUFFER_SIZE (16)
static uint8_t buffer[UART_BUFFER_SIZE];
static struct ring_buffer tx_buffer = { .buffer = buffer, .size = UART_BUFFER_SIZE };

static inline void uart_tx_clear_interrupt()
{
    IFG2 &= ~UCA0TXIFG;
}

static inline void uart_tx_enable_interrupt()
{
    IE2 |= UCA0TXIE;
}

static inline void uart_tx_disable_interrupt()
{
    IE2 &= ~UCA0TXIE;
}

INTERRUPT_FUNCTION(USCIAB0TX_VECTOR) isr_uart_tx()
{
    if (!empty_ring_buffer(&tx_buffer)) {
        UCA0TXBUF = ring_buffer_get(&tx_buffer);
    } else {
        uart_tx_disable_interrupt();
    }
}

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

    IE2 &= ~UCA0TXIE;
}

void uart_put_char_polling(char c)
{
    while (!(IFG2 & UCA0TXIFG)) { };
    UCA0TXBUF = c;
}

void uart_put_char_interrupt(char c)
{
    __disable_interrupt();

    bool hardware_ready = (IFG2 & UCA0TXIFG);

    if (empty_ring_buffer(&tx_buffer) && hardware_ready) {
        // kickstart
        UCA0TXBUF = c;
    } else {
        ring_buffer_put(&tx_buffer, c);
        uart_tx_enable_interrupt();
    }

    __enable_interrupt();
}
void uart_put_string(char const *str)
{
    while (*str) {
        uart_put_char_polling(*str);
        str++;
    }
}
