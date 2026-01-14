#ifndef IO_H
#define IO_H

typedef enum
{
    IO_TEST_LED,
    IO_UART_RXD,
    IO_UART_TXD,
    IO_UNUSED_1,
    IO_UNUSED_2,
    IO_UNUSED_3,
    IO_UNUSED_4,
    IO_UNUSED_5,
    IO_UNUSED_6,
    IO_UNUSED_7,
    IO_UNUSED_8,
    IO_UNUSED_9,
    IO_UNUSED_10,
    IO_UNUSED_11,
    IO_UNUSED_12,
    IO_UNUSED_13,
} io_e;

typedef enum
{
    IO_SELECT_GPIO,
    IO_SELECT_ALT1,
    IO_SELECT_ALT2,
    IO_SELECT_ALT3,
} io_selected_e;

typedef enum
{
    IO_DIR_OUTPUT,
    IO_DIR_INPUT,
} io_dir_e;

typedef enum
{
    IO_RESISTOR_DISABLED,
    IO_RESISTOR_ENABLED,
} io_resistor_e;

typedef enum
{
    IO_OUT_LOW,
    IO_OUT_HIGH,
} io_out_e;

typedef enum
{
    IO_IN_LOW,
    IO_IN_HIGH,
} io_in_e;

void io_set_select(io_e io, io_selected_e select);
void io_set_direction(io_e io, io_dir_e direction);
void io_set_resistor(io_e io, io_resistor_e resistor);
void io_set_out(io_e io, io_out_e out);
io_in_e io_get_input(io_e io);

#endif