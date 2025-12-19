#include "bsp_uart.h"
#include <msp430.h>

// Init buffer
static RingBuffer rx = {{0}, 0, 0};

void BSP_UART_Init(void)
{
	// 1. Set UCSWRST
	UCA0CTL1 |= UCSWRST;
	
	// 2. Configure registers (while reset)
	// a. Choose clock: SMCLK
	UCA0CTL1 |= UCSSEL_2;
	
	// b. Configure baudrate: 115200
	UCA0BR0 = 8; 	// Low byte (phần nguyên)
	UCA0BR1 = 0;	// High byte (phần nguyên)
	
	// c. Configure	Modulation 
	UCA0MCTL = UCBRF_11 + UCOS16 + UCBRS_0;
	
	// 3. Configure	Ports 
	P1SEL |= BIT1 | BIT2;
	P1SEL2 |= BIT1 | BIT2;
	
	// 4. Delete UCSWRST 
	UCA0CTL1 &= ~UCSWRST;
	
	// 5. Enable interrupt 
	IE2 |= UCA0RXIE; // Enable USCI_A0 RX interrupt
	
}

void BSP_UART_PutChar(char c)
{
	
	// UCA0TXIFG = 1 là bộ đệm trống, = 0 là bận 
	// IFG2 là thanh ghi chứa cờ ngắt TX/RX của USCI_A0
	// Vòng lặp chờ (blocking)
	while (!(IFG2 & UCA0TXIFG));
	
	// Nhét dữ liệu vào
	UCA0TXBUF = c;
}
void BSP_UART_PutString(char *str)
{
	// Duyệt từng ký tự cho đến khi gặp \0
	while (*str)
	{
		BSP_UART_PutChar(*str);
		str++; // Nhảy sang ký tự tiếp theo
	}
}

// Hàm đẩy dữ liệu vào buffer (chỉ dùng trong ISR)
static bool RingBuffer_Put(RingBuffer *rx, uint8_t data) {
	
	uint16_t next_rear = (rx->rear + 1) & (UART_RX_BUFFER_SIZE - 1);
	
	if (next_rear != rx->front) {  
		rx->buffer[rx->rear] = data;
		rx->rear = next_rear;
		return true;
	}
	return false;
}




// Ngắt nhận dữ liệu UART
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
	if (IFG2 & UCA0RXIFG)
	{
		uint8_t received_char = UCA0RXBUF;
		RingBuffer_Put(&rx, received_char);
	}
}

// Cho lớp App

bool BSP_UART_Read(uint8_t *c) {
	if (rx.front == rx.rear) return false; // Empty buffer
	
	*c = rx.buffer[rx.front];
	rx.front = (rx.front + 1) & (UART_RX_BUFFER_SIZE - 1);
	return true;
} 