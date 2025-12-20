#include <stdio.h>
#include <stdint.h>
int32_t String_to_Int(char *text) {
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
int main () {
	char cmd[15] = "PAN 90";
	printf("%s\n", cmd);
	char tmp1[8];
	char tmp2[8];
	char* p = cmd;
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
	printf("%s\n", tmp1);
	printf("%d", String_to_Int(tmp2));

	
}
