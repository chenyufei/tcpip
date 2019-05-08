#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <libgen.h>

#define BUFFER_SIZE 4096 /*读缓冲区大小*/
/*主状态机的丙种可能状态，分别表示：当有下大分析请求行，当前正在分析头部字段*/
typedef enum CHECK_STATE {CHECK_STATE_REQUESTLINE=0,CHECK_STATE_HEADER}CHECK_STATE;
/*从状态机的三种可能状态，即行的读取状态，分别表示：读取到一个完整的行，行出错和行数据尚且联完整*/
typedef enum LINE_STATUS {LINE_OK=0,LINE_BAD,LINE_OPEN}LINE_STATUS;
/*服务器处理HTTP请求的结果：
* NO_REQUEST表示请求不完整，需要继续读取客户数据;
* GET_REQUEST表示获得了一个完整的客户请求;
* BAD_REQUEST表示客户请求有语法错误；
* FORBIDDEN_REQUEST表示客户对资源没有足够的访问权限;
* INTERNAL_ERROR表示服务器内部错误;
* CLOSED_CONNECTION表示客户端已经关闭连接
*/
typedef enum HTTP_CODE {NO_REQUEST,GET_REQUEST,BAD_REQUEST,FORBIDDEN_REQUEST,INTERNAL_ERROR,CLOSED_CONNECTION}HTTP_CODE;

/*为了简化问题，我们没有给客户端发送一个完整的HTTP应答报文，而只是根据服务器的处理结果发送发如下成功或者失败信息*/

static const char*szret[] = {"I get a correct result\n","Something wrong\n"};

/*从状态机，用于解析出一行内容*/
LINE_STATUS parse_line(char*buffer,int& checked_index,int& read_index)
{
	char temp;
	/*
	checked_index指向buffer(应用程序的读缓冲区)中当前正在分析的字节，
	read_index指向buffer中客户数据的尾部的下一字节，buffer中第0~checked_index字节都已分析完毕，
	第checked_index~(read_index-1)字节由下面的循环挨个分析
	*/

	for(;checked_index<read_index;++checked_index)
	{
		/*获得当前要分析的字符*/
		temp = buffer[checked_index];
		/*如果当前的字节是\r"即回车符，则说明可能读到一完整的行*/
		if('\r' == temp)
		{
			/*如果"\r"字符碰巧是目前buffer中的最后一个已经被读入的客户数据，那么这次分析
			没有读取到一个完整的行，返回LINE_OPEN以表示还需要继续读取客户数据才能进一步分析
			*/
			if( (checked_index + 1) == read_index)
			{
				return LINE_OPEN;
			}
			/*如果下一个字符是"\n",则说明我们成功读到一个完整的行*/
			else if (buffer[checked_index + 1] == '\n')
			{
				buffer[checked_index++] = '\0';
				buffer[checked_index++] = '\0';
				return LINE_OK;
			}
			/*否则的话，说明客户发送的HTTP请求存在语法问题*/
			return LINE_BAD;
		}
		else if('\n' == temp)
		{
			if( (checked_index > 1) && buffer[checked_index-1] == '\r')
			{
				buffer[checked_index-1] = '\0';
				buffer[checked_index++] = '\0';
				return LINE_OK;
			}	
			return LINE_BAD;
		}
		
	}
	/*
	如果所有内容都分析完毕也没有遇到"\r"字符，则返回LINE_OPEN，表示还需要继续读取客户数据才能进一步分析
	*/
	return LINE_OPEN;
}

/*分析请求行*/
HTTP_CODE parse_requestline(char *temp,CHECK_STATE &checkstate)
{
	char * url = strpbrk(temp, " \t");
	/*如果请求行中没有空白字符或"\t"字符，则HTTP请求必有问题*/
	if(NULL == url)
	{
		return BAD_REQUEST;
	}

	*url++ ='\0';
	
	char *method = temp;
	if(strcasecmp(method,"GET") == 0)/*仅支持GET方法*/
	{
		printf("The request method is GET\n");
	}
	else
	{
		return BAD_REQUEST;
	}

	url += strspn(url," \t");
	char * version = strpbrk(url," \t");
	if(NULL == version)
	{
		return BAD_REQUEST;
	}
	*version++ = '\0';
	version += strspn(version," \t");
	/*仅支持HTTP/1.1 */
	if(strcasecmp(version,"HTTP/1.1") != 0)
	{
		return BAD_REQUEST;
	}
	/*检查URL是否合法*/
	if(strncasecmp(url,"http://",7) == 0)
	{
		url += 7;
		url = strchr(url,'/');
	}
	
	if(NULL == url || url[0] != '/')
	{
		return BAD_REQUEST;
	}

	printf("The request URL is :%s\n",url);
	/*HTTP请求处理完毕，状态转移到头部字段的分析*/
	checkstate = CHECK_STATE_HEADER;
	return NO_REQUEST;
}

/*分析头部字段*/
HTTP_CODE parse_headers(char*temp)
{
	/*遇到一个空行，说明我们得到了一个正确的HTTP请求*/
	if(temp[0] == '\0')
	{
		return GET_REQUEST;
	}
	else if(strncasecmp(temp,"Host:",5) == 0)/*处理HOST头部字段*/
	{
		temp += 5;
		temp += strspn(temp," \t");
		printf("the request host is: %s\n",temp);
	}
	else /*其他头部字段都不处理*/
	{
		printf("I can not handle this header\n");
	}
	return NO_REQUEST;
}

/*分析HTTP请求的入口函数*/
HTTP_CODE parse_content(char*buffer,int &checked_index,CHECK_STATE&checkstate,int&read_index,int&start_line)
{
	LINE_STATUS linestatus = LINE_OK;/*记录当前行的读取状态*/
	HTTP_CODE retcode = NO_REQUEST;  /*记录HTTP请求的处理结果*/
	/*主状态机，用于从buffer中取出所有完整的行*/
	while(LINE_OK == (linestatus=parse_line(buffer,checked_index,read_index)))
	{
		char *temp = buffer + start_line;/*start_line是行在buffer中的起始位置*/
		start_line = checked_index;/*记录下行的起始位置*/
		/*checkstate记录主状态机当前的状态*/
		switch(checkstate)
		{
			case CHECK_STATE_REQUESTLINE:/*第一个状态，分析请求行*/
			{
				retcode = parse_requestline(temp,checkstate);
				if(BAD_REQUEST == retcode)
				{
					return BAD_REQUEST;
				}
				break;
			}
			case CHECK_STATE_HEADER:/*第二个状态，分析头部字段*/
			{
				retcode = parse_headers(temp);
				if(BAD_REQUEST == retcode)
				{
					return BAD_REQUEST;
				}
				else if (GET_REQUEST == retcode)
				{
					return GET_REQUEST;
				}
				break;
			}
			default:
			{
				return INTERNAL_ERROR;
			}
		}
	}
	
	/*若没有读取到一个完整的行，则表示还需要继续读取客户数据才能进一步分析*/
	if(LINE_OPEN == linestatus)
	{
		return NO_REQUEST;
	}
	else
	{
		return BAD_REQUEST;
	}
}

int main(int argc,char*argv[])
{
	if(argc <= 2)
	{
		printf("usage: %s ip_address port_number\n",basename(argv[0]));
		return 1;
	}

	const char * ip = argv[1];
	int port = atoi(argv[2]);

	struct sockaddr_in address;
	bzero(&address,sizeof(address));
	address.sin_family = AF_INET;
	inet_pton(AF_INET,ip,&address.sin_addr);
	address.sin_port = htons(port);

	int listenfd = socket(PF_INET,SOCK_STREAM,0);
	assert(listenfd>=0);
	
	int ret = bind(listenfd,(struct sockaddr*)&address,sizeof(address));
	assert(ret != -1);
	
	ret = listen(listenfd,5);
	assert(ret != -1);

	struct sockaddr_in client_address;
	socklen_t client_addrlength = sizeof(client_address);
	
	int fd = accept(listenfd,(struct sockaddr*)&client_address,&client_addrlength);
	if(fd < 0)
	{
		printf("errno is: %d\n",errno);
	}
	else
	{
		char buffer[BUFFER_SIZE];/*读缓冲区*/
		memset(buffer,'\0',BUFFER_SIZE);
		int data_read = 0;
		int read_index = 0;/*当前已经读取了多少个字节的客户数据*/
		int checked_index = 0;/*当前已经分析完了多少个字节的客户数据*/
		int start_line = 0;/*行在buffer中的起始位置*/
		/*设置主状态机的初始状态*/
		CHECK_STATE checkstate = CHECK_STATE_REQUESTLINE;
		while(1)/*循环读取客户数据并分析之*/
		{
			data_read = recv(fd,buffer+read_index,BUFFER_SIZE-read_index,0);
			if(-1 == data_read)
			{
				printf("reading failed\n");
				break;
			}
			else if (0 == data_read)
			{
				printf("remote client has closed the connection\n");
				break;
			}
			read_index += data_read;
			/*分析目前已经获取得的所有客户数据*/
			HTTP_CODE result = parse_content(buffer,checked_index,checkstate,read_index,start_line);
			if(NO_REQUEST == result)
			{
				continue;
			}
			else if (GET_REQUEST == result)
			{
				send(fd,szret[0],strlen(szret[0]),0);
				break;
			}
			else /*其他情况表示发生错误*/
			{
				send(fd,szret[1],strlen(szret[1]),0);
				break;
			}
		}
		close(fd);
	}
	close(listenfd);
	return 0;
}
