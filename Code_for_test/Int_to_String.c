#include <stdio.h>
#define buffer_size 64
char buffer [buffer_size];

int Int_to_String(int n, char *buffer) {
	char *ptr = buffer;
	int signFlag = 0;
	if (n < 0) {
		n = -n;
		signFlag = 1;
	}
	do {
		int digit = n % 10;
		char c = digit + '0';
		*ptr++ = c;
		n = n / 10;
	} while (n > 0);
	
	if (signFlag) *ptr++ = '-';
	
	*ptr = '\0';
	
	// Reverse strings
	char *start = buffer;
	char *end = ptr - 1;
	char temp;
	
	while (start < end) {
		temp = *start;
		*start = *end;
		*end = temp;
		
		start++;
		end--;
	}
	return (int)(ptr - buffer);
}


int main () {
	Int_to_String(0, buffer);
	printf("%s", buffer);
}
