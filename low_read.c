#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#define BUF_SIZE 100

void error_handling(char*message);

int main(void)
{
	int fd;
	char buf[BUF_SIZE];
	fd = open("data.txt",O_RDONLY);
	if(-1 == fd)
	{
		error_handling("open() error!");
	}
	printf("file descriptor: %d \n",fd);
	
	if(-1 == read(fd,buf,sizeof(buf)))
	{
		error_handling("read() error!");
	}

	printf("file data: %s\n",buf);
	close(fd);

	return 0;
}
void error_handling(char *message)
{
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}
