#ifndef UART_H
#define UART_H

void uart_init(void);
void uart_put_char(char c);
void uart_put_string(char *str);

// TODO: uart rx using polling -> interrupt tx, rx

#endif