#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/select.h>
using namespace std;
int main()
{
	int sockClient,nMsgLen,nReady;
	char szRecv[1024],szSend[1024],szMsg[1024];
	struct sockaddr_in addrServer,addrClient,addrLocal;
	socklen_t addrLen;
	fd_set setHold,setTest;


	sockClient=socket(AF_INET,SOCK_DGRAM,0);
	addrLen=sizeof(struct sockaddr_in);
	bzero(&addrServer,sizeof(addrServer));
	addrServer.sin_family=AF_INET;
	addrServer.sin_addr.s_addr=inet_addr("127.0.0.1");
	addrServer.sin_port=htons(8000);


	addrLocal.sin_family=AF_INET;//bind to a local port
	addrLocal.sin_addr.s_addr=htonl(INADDR_ANY);
	addrLocal.sin_port=htons(9000);
	if(bind(sockClient,(struct sockaddr*)&addrLocal,sizeof(addrLocal))==-1)
	{
		cout<<"error in binding"<< endl;
		exit(2);
	}


	if(connect(sockClient,(struct sockaddr*)&addrServer,sizeof(addrServer))==-1)
	{
		cout <<"error in connecting"<< endl;
		exit(1);
	}

	FD_ZERO(&setHold);
	FD_SET(STDIN_FILENO,&setHold);
	FD_SET(sockClient,&setHold);
	cout<<"you can type in sentences any time"<<endl;
	while(true)
	{
		setTest=setHold;
		nReady=select(sockClient+1,&setTest,NULL,NULL,NULL);
		if(FD_ISSET(0,&setTest))
		{
			nMsgLen=read(0,szMsg,1024);
			write(sockClient,szMsg,nMsgLen);
		}
		if(FD_ISSET(sockClient,&setTest))
		{
			nMsgLen=read(sockClient,szRecv,1024);
			szRecv[nMsgLen]='\0';
			cout<<"read:"<<szRecv<<endl;
		}
	}
}
