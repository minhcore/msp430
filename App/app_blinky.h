#ifndef APP_BLINKY_H
#define APP_BLINKY_H

// Định nghĩa các trạng thái của hệ thống
typedef enum
{
	STATE_IDLE, 	// Tắt
	STATE_SLOW,		// Nháy chậm
	STATE_FAST		// Nháy nhanh
} BlinkyState_t;

// Hàm khởi tạo trạng thái ban đầu của ứng dụng
void App_Blinky_Init(void);

// Hàm thực thi chính
void App_Blinky_Run(void);

#endif