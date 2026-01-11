// cppcheck-suppress unusedFunction
#include <msp430.h>

int sum (int a, int b)
{
    return a + b;
}


int main (void) 
{
    const int arr[5] = {0};
    return arr[1];
}