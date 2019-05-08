#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<stdio.h>
#include<syslog.h>
#include<signal.h>
int daemon_init(void)
{
	pid_t pid;
	if((pid=fork())<0)
	{
		return(-1);
	}
	else if(pid!=0)
	{
		exit(0);/*parent exit*/
	}
	/*child continues*/
	setsid();/*become session leader*/
	chdir("/");/*change working directory*/
	umask(0);/*clear file mode creation mask*/
	close(0);/*close stdin*/
	close(1);/*close stdout*/
	close(2);/*close stderr*/
	return(0);
}
void sig_term(int signo)
{
	if(signo==SIGTERM)
		/*catched signal sent by kill(1) command*/
	{
		syslog(LOG_INFO,"program terminated.");
		closelog();
		exit(0);
	}
}
int main(void)
{
	if(daemon_init()==-1)
	{
		printf("can'tforkself\n");
		exit(0);
	}
	openlog("daemontest",LOG_PID,LOG_USER);
	syslog(LOG_INFO,"program started.");
	signal(SIGTERM,sig_term);/*arrange to catch the signal*/
	while(1)
	{
		sleep(1);/*put your main program here*/
	}
	return(0);
}
