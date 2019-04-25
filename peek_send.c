#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
void error_handling(char*message);

int main(int argc,char*argv[])
{
	int sock;
	struct sockaddr_in send_adr;

	if(3 != argc)
	{
		printf("Usage : %s <IP> <port>\n",argv[0]);
		exit(1);
	}

	sock = socket(PF_INET,SOCK_STREAM,0);
	if(-1 == sock)
	{
		error_handling("socket() error");
	}

	memset(&send_adr,0,sizeof(send_adr));
	send_adr.sin_family=AF_INET;
	send_adr.sin_addr.s_addr=inet_addr(argv[1]);
	send_adr.sin_port = htons(atoi(argv[2]));

	if(-1 == connect(sock,(struct sockaddr*)&send_adr,sizeof(send_adr)))
	{
		error_handling("connect() error!");
	}
	else
	{
		puts("Connected..........");
	}

	write(sock,"123",strlen("123"));

	close(sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}
