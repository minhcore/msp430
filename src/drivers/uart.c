#include "uart.h"
#include "common/ring_buffer.h"
#include "common/defines.h"
#include "common/assert_handler.h"

#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

#define UART_BUFFER_SIZE (64)
static uint8_t buffer_t[UART_BUFFER_SIZE];
static uint8_t buffer_r[UART_BUFFER_SIZE];
static struct ring_buffer tx_buffer = { .buffer = buffer_t, .size = UART_BUFFER_SIZE };
static struct ring_buffer rx_buffer = { .buffer = buffer_r, .size = UART_BUFFER_SIZE };

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

static inline void uart_rx_enable_interrupt()
{
    IE2 |= UCA0RXIE;
}

INTERRUPT_FUNCTION(USCIAB0RX_VECTOR) isr_uart_rx()
{
    char c = UCA0RXBUF;
    if (!full_ring_buffer(&rx_buffer)) {
        ring_buffer_put(&rx_buffer, c);
    }
}

static void uart_configure(void)
{
    UCA0CTL1 |= UCSWRST;
    // Use SMCLK clock
    UCA0CTL1 |= UCSSEL_2;
    // Use 16 Mhz clock and 115200 baudrate
    UCA0BR0 = 8;
    UCA0BR1 = 0;
    UCA0MCTL = UCBRS_0 + UCBRF_11 + UCOS16;

    UCA0CTL1 &= ~UCSWRST;

    uart_rx_enable_interrupt();
    uart_tx_disable_interrupt();
}

static bool initialized = false;
void uart_init(void)
{
    ASSERT(!initialized);
    uart_configure();
    initialized = true;
}

void _putchar(char c)
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

bool uart_get_char(char *c)
{
    __disable_interrupt();
    if (empty_ring_buffer(&rx_buffer)) {
        __enable_interrupt();
        return false;
    } else {
        *c = ring_buffer_get(&rx_buffer);
    }
    __enable_interrupt();
    return true;
}

void uart_init_assert(void)
{
    uart_tx_disable_interrupt();
    uart_configure();
}

static void uart_putchar_polling(char c)
{
    UCA0TXBUF = c;
    while (!(IFG2 & UCA0TXIFG)) { }
}

void uart_trace_assert(const char *string)
{
    int i = 0;
    while (string[i] != '\0') {
        uart_putchar_polling(string[i]);
        i++;
    }
}
