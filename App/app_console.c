#include "app_console.h"
#include <string.h> // Để dùng strcmp


static App_Console_Callback_t console_cb = NULL;

void App_Console_RegisterCallback(App_Console_Callback_t cb) {
    console_cb = cb; // Lưu lại địa chỉ hàm xử lý từ Controller
}

// Hàm phụ trợ chuyển string thành int
static int32_t String_to_Int(char *text) {
	char* ptr = text;
	while (*ptr == ' ') ptr++;
	
	if (*ptr == '+') ptr++; 
	
	int32_t result = 0;
	while (*ptr != '\0') {
		if (*ptr < '0' || *ptr > '9') break;
		int digit = (*ptr++) - '0';
		result = (result << 3) + (result << 1) + digit;
	
	}


	return result;
}


static char cmd_buffer[CMD_BUFFER_SIZE];
static uint8_t cmd_index = 0;

static void Process_Command(void)
{
	char tmp1[8];
	char tmp2[8];
	char* p = cmd_buffer;
	char* p1 = tmp1;
	char* p2 = tmp2; 
	while (*p == ' ') p++;
	while (*p != ' ') 
	{
		*p1 = *p;
		p1++;
		p++;
	}
	*p1 = '\0';
	p++;
	while (*p != '\0')
	{
		*p2 = *p;
		p2++;
		p++;
	}
	*p2 = '\0';
	
	int32_t value = String_to_Int(tmp2);
	
	if (console_cb != NULL)
	{
		console_cb(tmp1, value);
	}
	
	
}
void App_Console_Init(void) 
{
	for (uint8_t i = 0; i < CMD_BUFFER_SIZE; i++) 
	{	
		cmd_buffer[i] = 0;
	}
	
	cmd_index = 0;
	
	BSP_UART_PutString("\r\n System Ready! \r\n");
	BSP_UART_PutString("MSP430> ");
}
void App_Console_Task (void)
{
	uint8_t data_received;
	if (BSP_UART_Read(&data_received) == false) return; // Buffer Ring trống -> Quay về
	
		// Ép kiểu về char
		char c = (char)data_received; 
		
		// Xử lý ký tự c
		if (c == '\r') // TH1: Enter
		{
			cmd_buffer[cmd_index] = '\0';
			
			BSP_UART_PutString("\r\n");
			
			if (cmd_index > 0) Process_Command(); // Chỉ xử lý nếu có nội dung
			
			// Reset
			cmd_index = 0;
			BSP_UART_PutString("\r\n");
			BSP_UART_PutString("MSP430> ");
		}
		else if (c == 8 || c == 127) // TH2: Backspace
		{
			if (cmd_index > 0)
			{
			cmd_index--;
			BSP_UART_PutString("\b \b");
			}
		}
		
		
		else if (c >= 32 && c <= 126) // TH3: Ký tự in được
		{
			if (cmd_index < CMD_BUFFER_SIZE	- 1) // Nếu còn chỗ, CMD_BUFFER_SIZE - 1 chừa chỗ cho \0
			{
				cmd_buffer[cmd_index] = c;
				cmd_index++;
				BSP_UART_PutChar(c); // Echo
			}
		}
}




