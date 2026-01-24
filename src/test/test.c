#include "drivers/io.h"
#include "drivers/mcu_init.h"
#include "drivers/led.h"
#include "drivers/uart.h"

#include "common/assert_handler.h"
#include "common/defines.h"

#include <msp430.h>
#include <stdint.h>

SUPPRESS_UNUSED
static void test_setup(void)
{
    mcu_init();
}

SUPPRESS_UNUSED
static void test_assert(void)
{
    test_setup();
    ASSERT(0);
}

SUPPRESS_UNUSED
static void test_blink_led(void)
{
    test_setup();
    led_init();
    led_state_e led_state = LED_STATE_OFF;
    while (1) {
        led_state = (led_state == LED_STATE_OFF) ? LED_STATE_ON : LED_STATE_OFF;
        led_set(LED_TEST, led_state);
        BUSY_WAIT_ms(2000);
    }
}

SUPPRESS_UNUSED
static void test_gpio_output_pin(void)
{
    test_setup();
    struct io_config output_config = { .direction = IO_DIR_OUTPUT,
                                       .out = IO_OUT_LOW,
                                       .resistor = IO_RESISTOR_DISABLED,
                                       .select = IO_SELECT_GPIO };
    for (io_generic_e io = IO_10; io <= IO_27; io++) {
        io_configure(io, &output_config);
    }

    while (1) {
        for (io_generic_e io = IO_10; io <= IO_27; io++) {
            io_set_out(io, IO_OUT_HIGH);
            BUSY_WAIT_ms(10);
            io_set_out(io, IO_OUT_LOW);
        }
    }
}

SUPPRESS_UNUSED
static void test_gpio_input_pin(void)
{
    test_setup();
    led_init();
    const struct io_config input_config = {
        .select = IO_SELECT_GPIO,
        .resistor = IO_RESISTOR_ENABLED,
        .direction = IO_DIR_INPUT,
        .out = IO_OUT_HIGH // pull-up
    };

    // Configure all pins as input
    for (io_generic_e io = IO_10; io <= IO_27; io++) {
        if (io == (io_generic_e)IO_TEST_LED) {
            continue;
        }
        io_configure(io, &input_config);
    }

    for (io_generic_e io = IO_10; io <= IO_27; io++) {
        if (io == (io_generic_e)IO_TEST_LED) {
            continue;
        }
        for (uint8_t i = 0; i < 2; i++) {
            led_set(LED_TEST, LED_STATE_ON);
            BUSY_WAIT_ms(50);
            led_set(LED_TEST, LED_STATE_OFF);
            BUSY_WAIT_ms(50);
        }
        led_set(LED_TEST, LED_STATE_ON);
        // Wait for user to pull pin low
        while (io_get_input(io) == IO_IN_HIGH) {
            BUSY_WAIT_ms(100);
        }
        led_set(LED_TEST, LED_STATE_OFF);
        // Wait for user to disconnect
        while (io_get_input(io) == IO_IN_LOW) {
            BUSY_WAIT_ms(100);
        }
    }

    // Blink LED when test is done
    while (1) {
        led_set(LED_TEST, LED_STATE_ON);
        BUSY_WAIT_ms(500);
        led_set(LED_TEST, LED_STATE_OFF);
        BUSY_WAIT_ms(2000);
    }
}

SUPPRESS_UNUSED
static void io_11_isr(void)
{
    led_set(LED_TEST, LED_STATE_ON);
}

SUPPRESS_UNUSED
static void io_20_isr(void)
{
    led_set(LED_TEST, LED_STATE_OFF);
}

SUPPRESS_UNUSED
static void test_io_interrupt(void)
{
    test_setup();
    const struct io_config input_config = {
        .select = IO_SELECT_GPIO,
        .resistor = IO_RESISTOR_ENABLED,
        .direction = IO_DIR_INPUT,
        .out = IO_OUT_HIGH // pull-up
    };
    io_configure(IO_11, &input_config);
    io_configure(IO_20, &input_config);
    led_init();
    io_configure_interrupt(IO_11, IO_TRIGGER_FALLING, io_11_isr);
    io_configure_interrupt(IO_20, IO_TRIGGER_FALLING, io_20_isr);
    io_enable_interrupt(IO_11);
    io_enable_interrupt(IO_20);
    while (1)
        ;
}

SUPPRESS_UNUSED
static void test_uart(void)
{
    test_setup();
    __enable_interrupt();
    while (1) {
        char c;
        if (uart_get_char(&c)) {
            uart_put_char_interrupt(c);
            uart_put_string("\r\n");
            switch (c) {
            case 'o':
                led_set(LED_TEST, LED_STATE_ON);
                break;
            case 'i':
                led_set(LED_TEST, LED_STATE_OFF);
                break;
            default:
                break;
            }
        }
    }
}

int main()
{
    TEST();
    ASSERT(0);
    return 0;
}