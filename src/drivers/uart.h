#ifndef UART_H
#define UART_H

void uart_init(void);
void uart_put_char_polling(char c);
void uart_put_char_interrupt(char c);
void uart_put_string(char const *str);

// TODO: uart rx using polling -> interrupt tx, rx

#endif