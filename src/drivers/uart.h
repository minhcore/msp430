#ifndef UART_H
#define UART_H

#include <stdbool.h>

void uart_init(void);
void _putchar(char c);
bool uart_get_char(char *c);

#endif