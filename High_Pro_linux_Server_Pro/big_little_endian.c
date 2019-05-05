#include <stdio.h>
/*
*大端字节序:指一个整数的高位字节存储在内存的低地址处，低位字节存储在内存的高地址处
*小端字节序:指一个整数的高位字节存储在内存的高地址处，低位字节存储在内存的低地址处
*/
void byteorder()
{
	union
	{
		short value;
		char union_bytes[sizeof(short)];
	}test;
	test.value = 0x0102;

	if((test.union_bytes[0] == 1) && (test.union_bytes[1] == 2))
	{
		printf("big endian\n");
	}
	else if((test.union_bytes[0] == 2) && (test.union_bytes[1] == 1))
	{
		printf("little endian\n");
	}
	else
	{
		printf("unknow...\n");
	}
}


int main(int argc,char*argv[])
{
	byteorder();
	return 0;
}
