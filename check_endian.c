#include <stdio.h>

typedef union{
int a;
char b;
}Endian;

static union { char c[4]; unsigned long l; } endian_test= { { 'l', '?', '?', 'b' } };
#define ENDIANNESS ((char)endian_test.l)
int main(void)
{
	Endian end;
	end.a = 1;
	if(end.b != 1)
	{
		printf("big endian\n");
	}
	else
	{
		printf("small endian\n");
	}
	printf("endial=%c\n",ENDIANNESS);

	return 0;	
}
