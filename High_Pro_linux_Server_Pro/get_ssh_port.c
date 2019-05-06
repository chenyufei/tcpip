#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
	assert(argc == 2);
	char *host = argv[1];
	struct hostent* hostinfo = gethostbyname(host);
	assert(hostinfo);

	struct servent * servinfo = getservbyname("ssh","tcp");
	assert(servinfo);
	printf("ssh port is %d\n",ntohs(servinfo->s_port));

	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = servinfo->s_port;

	address.sin_addr = *(struct in_addr*)*hostinfo->h_addr_list;
	printf("sin_addr=%s\n",inet_ntoa(address.sin_addr));

	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	int result = connect(sockfd,(struct sockaddr*)&address,sizeof(address));
	assert(result != -1);

	char buffer[128];
	result = read(sockfd,buffer,sizeof(buffer));
	assert(result > 0);
	buffer[result] = '\0';
	printf("the read str is : %s",buffer);
	close(sockfd);
	return 0;
}
