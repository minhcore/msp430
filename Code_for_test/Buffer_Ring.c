#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#define BUFFER_SIZE 8

typedef struct {
	uint8_t	buffer [BUFFER_SIZE]; 
	volatile uint16_t front;
	volatile uint16_t rear; 
}RingBuffer;

// Init
RingBuffer rx_buffer = {{0}, 0, 0};

// 2 function quan trong
bool RingBuffer_Put(RingBuffer *rx, uint8_t data) {
	
	uint16_t next_rear = (rx->rear + 1) & (BUFFER_SIZE - 1);
	
	if (next_rear != rx->front) {  
		rx->buffer[rx->rear] = data;
		rx->rear = next_rear;
		return true;
	}
	return false;
}

bool RingBuffer_Get(RingBuffer *rx, char *c) {
	if (rx->front == rx->rear) return false; // Empty buffer
	
	*c = rx->buffer[rx->front];
	rx->front = (rx->front + 1) & (BUFFER_SIZE - 1);
	return true;
} 
int main () {
	RingBuffer_Put(&rx_buffer, 'k');
	RingBuffer_Put(&rx_buffer, 'b');
	RingBuffer_Put(&rx_buffer, 'c');
	RingBuffer_Put(&rx_buffer, 'd');
	RingBuffer_Put(&rx_buffer, 'e');
	RingBuffer_Put(&rx_buffer, 'f');
	RingBuffer_Put(&rx_buffer, 'g');
	RingBuffer_Put(&rx_buffer, 'm');
	char c;
	RingBuffer_Get(&rx_buffer, &c);
	RingBuffer_Get(&rx_buffer, &c);
	RingBuffer_Get(&rx_buffer, &c);
	RingBuffer_Get(&rx_buffer, &c);
	RingBuffer_Get(&rx_buffer, &c);
	RingBuffer_Get(&rx_buffer, &c);
	RingBuffer_Get(&rx_buffer, &c);
	RingBuffer_Get(&rx_buffer, &c);
	RingBuffer_Get(&rx_buffer, &c);
	RingBuffer_Get(&rx_buffer, &c);
	RingBuffer_Put(&rx_buffer, 'm');
	RingBuffer_Get(&rx_buffer, &c);
	printf("%c", c);
}


