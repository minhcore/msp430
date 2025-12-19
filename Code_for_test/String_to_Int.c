#include <stdio.h>
#include <stdint.h>

char text[50];
int32_t String_to_Int(char *text) {
	char* ptr = text;
	while (*ptr == ' ') ptr++;
	
	int signFlag = 0;
	if (*ptr == '-') {
		ptr++;
		signFlag = 1;
	} else if (*ptr == '+') ptr++; 
	
	int32_t result = 0;
	while (*ptr != '\0') {
		if (*ptr < '0' || *ptr > '9') break;
		int digit = (*ptr++) - '0';
		result = (result << 3) + (result << 1) + digit;
	
	}
	
	if (signFlag) result = -result;
	
	return result;
	
}
int main () {
	text[0] = '+';
	text[1] = '5';
	text[2] = '7';
	text[3] = '\0';
	printf("%d", String_to_Int(text));
}
