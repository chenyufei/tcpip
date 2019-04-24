#include <stdio.h>
#include <unistd.h>

int main(int argc,char*argv[])
{
	pid_t pid = fork();
	if (0 == pid)
	{
		printf("Hi, I am a child process");
	}
	else
	{
		printf("Child Process ID: %d \n",pid);
		sleep(30);//Sleep 30 sec.
	}

	if(0 == pid)
	{
		printf("End child process");
	}
	else
	{
		printf("End parent process");
	}

	return 0;
}
