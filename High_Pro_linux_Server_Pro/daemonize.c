#include <unistd.h>
//使用daemon(int nochdir,int noclose)可同样实现如下函数的功能
bool daemonize()
{
	pid_t pid = fork();
	if(pid < 0)
	{
		return false;
	}
	else if(pid > 0)
	{
		exit(0);
	}

	/*设备文件权限掩码，当进程创建新文件时（使用open(const char *pathname,int flags,mode_t mode)系统调用)时,文件的权限将是mode & 0777*/
	umask(0);

	/*创建新的会话，设置本进程为进程组的首领*/
	pid_t sid = setsid();
	if(sid < 0)
	{
		return false;
	}

	/*切换工作目录*/
	if((chdir("/")) < 0)
	{
		return false;
	}
	
	/*关闭标准输入设备、标准输出设备和标准错误输出设备*/
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	/*关闭其他已经打开的文件描述符*/
	/*将标准输入、标准输出和标准错误输出都定向到/dev/null文件*/
	open("/dev/null",O_RDONLY);
	open("/dev/null",O_RDWR);
	open("/dev/null",O_RDWR);

	return true;
}
