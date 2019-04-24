#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024
void error_handling(char*message);
void read_routine(int sock,char *buf);
void write_routine(int sock,char*buf);

int main(int argc,char*argv[])
{
	int sock;
	char message[BUF_SIZE];
	int str_len,recv_len,recv_cnt;
	struct sockaddr_in serv_adr;

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

	memset(&serv_adr,0,sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	if(-1 == connect(sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr)))
	{
		error_handling("connect() error!");
	}
	else
	{
		puts("Connected..........");
	}

	pid_t pid = fork();
	if(pid == 0)
	{
		write_routine(sock,message);
	}
	else
	{
		read_routine(sock,message);
	}
	close(sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}
void read_routine(int sock,char *buf)
{
	while(1)
	{
		int str_len = read(sock,buf,BUF_SIZE);
		if(0 == str_len)
		{
			return;
		}

		buf[str_len] = 0;
		printf("Message from server: %s",buf);
	}
}


void write_routine(int sock,char*buf)
{
	while(1)
	{
		fgets(buf,BUF_SIZE,stdin);
		if(!strcmp(buf,"q\n") || !strcmp(buf,"Q\n"))
		{
			shutdown(sock,SHUT_WR);
			return;
		}
		write(sock,buf,strlen(buf));
	}
}
