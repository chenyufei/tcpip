#include <stdio.h>
#include <stdlib.h>

int g_var = 0;

int main(int argc, char **argv)
{
	int	pid;
	g_var = 2;
	printf("faster g_var1= %d\n", g_var);
	if ( (pid == fork()) == 0) {
		printf("child g_var1= %d\n", g_var);
		g_var = 1;
		printf("child g_var2= %d\n", g_var);
		exit(0);
	}
	printf("faster g_var2= %d\n", g_var);

	printf("在LINUX中，子进程继承父进程的全局变量的值，但是子进程和父进程不是共享同一个全局变量\n");

	exit(0);
}
