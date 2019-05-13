#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc,char*argv[])
{
	FILE *in_file;
	int count = 1;
	char buf[80];

	in_file = fopen("mypipe","r");

	if(NULL == in_file)
	{
		printf("Error in fdopen.\n");
		exit(1);
	}

	while((count = fread(buf,1,80,in_file)) > 0)
	{
		printf("received from pipe: %s\n",buf);
	}

	fclose(in_file);
	return 0;
}
