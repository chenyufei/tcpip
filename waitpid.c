#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
        int status;
        pid_t pid=fork();

        if(0 == pid)
        {
                sleep(15);
                return 24;
        }
        else
        {
                while(!waitpid(-1,&status,WNOHANG))
                {
                        sleep(3);
                        printf("sleep 3sec.\n");
                }
		
		if(WIFEXITED(status))
		{
			printf("Child send %d \n",WEXITSTATUS(status));
		}
        }

	return 0;
}
