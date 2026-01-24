#ifndef UART_H
#define UART_H

#include <stdbool.h>

void uart_init(void);
void uart_put_char_polling(char c);
void uart_put_char_interrupt(char c);
void uart_put_string(char const *str);
bool uart_get_char(char *c);

#endif