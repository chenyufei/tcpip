#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc,char*argv[])
{
	FILE *out_file;
	int count = 1;
	char buf[80];

	out_file = fopen("mypipe","w");

	if(NULL == out_file)
	{
		printf("Error opening pipe.\n");
		exit(1);
	}

	sprintf(buf,"this is test data for the named pipe example\n");
	fwrite(buf,1,80,out_file);

	fclose(out_file);
	return 0;
}
