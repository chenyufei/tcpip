#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define LISTENQ 100

int main(int argc,char** argv) {
    if (argc != 2) {
        printf("please add <port>");
        return 1;
    }
    int sockfd;
    if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0) {
        printf("socket error: %s\n",strerror(errno));
        return 1;
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(atoi(argv[1]));
    if (bind(sockfd,(struct sockaddr*)&addr,sizeof(addr)) < 0) {
        printf("bind error: %s\n",strerror(errno));
        return 1;
    }
    if (listen(sockfd,LISTENQ) < 0) {
        printf("listen error: %s\n",strerror(errno));
        return 1;
    }
    int connfd;
    for ( ; ;) {
        if ((connfd = accept(sockfd,NULL,NULL)) < 0) {
            if (errno == EINTR) {
                continue;
            }
            printf("accept error: %s\n",strerror(errno));
            return 1;
        }
        int ret = close(STDOUT_FILENO);
        if (ret < 0) {
            printf("close error: %s",strerror(errno));
            exit(1);
        }
        if (dup(connfd) < 0) {
            printf("dup error: %s\n",strerror(errno));
            return 1;
        }
        printf("which is server send by STDOUT_FILENO\n");
        fflush(stdout);
        close(connfd);
    }
    return 0;
}
