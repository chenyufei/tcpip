#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main(int argc,char *argv[])
{
	if(daemon(0,0))
	{
		printf("no ok\n");
	}
	else
	{
		printf("ok\n");
	}

	while(true)
	{
		sleep(10);
	}

	return 0;
}
