#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#define BUF_SIZE 3

int main(int argc,char *argv[])
{
	FILE *fd1;
	FILE *fd2;
	
	char buf[BUF_SIZE];

	fd1 = fopen("news.txt","r");
	fd2 = fopen("cpy.txt","w");

	while(fgets(buf,BUF_SIZE,fd1) != NULL)
	{
		int result = fputs(buf,fd2);
		printf("the result = %d\n",result);
	}
	
	fclose(fd1);
	fclose(fd2);
	return 0;
}
