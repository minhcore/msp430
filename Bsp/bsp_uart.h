#ifndef BSP_UART_H
#define BSP_UART_H	

#include <stdint.h>
#include <stdbool.h>

#define UART_RX_BUFFER_SIZE 64

typedef struct {
	uint8_t buffer [UART_RX_BUFFER_SIZE];
	volatile uint16_t front;
	volatile uint16_t rear;
} RingBuffer;

void BSP_UART_Init(void);

// Gửi 1 ký tự 
void BSP_UART_PutChar(char c);

// Gửi cả chuỗi ký tự 
void BSP_UART_PutString(char *str);


// Hàm cho App lấy dữ liệu
bool BSP_UART_Read(uint8_t *c);

#endif