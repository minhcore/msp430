#ifndef UART_H
#define UART_H

void uart_init(void);
void uart_put_char(char c);
void uart_put_string(char const *str);
void uart_get_char(void);

// TODO: uart rx using polling -> interrupt tx, rx

#endif