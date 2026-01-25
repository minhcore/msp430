#ifndef UART_H
#define UART_H

#include <stdbool.h>

void uart_init(void);
void _putchar(char c);
bool uart_get_char(char *c);
void uart_init_assert(void);
void uart_trace_assert(const char *string);

#endif