#if 0
I. 同步机制
线程间的同步机制主要包括三个：

互斥锁：
	以排他的方式，防止共享资源被并发访问；互斥锁为二元变量， 状态为0-开锁、1-上锁;
	开锁必须由上锁的线程执行，不受其它线程干扰.
条件变量：
	满足某个特定条件时，可通过条件变量通知其它线程do-something;
	必须与互斥锁*联合使用，单独无法执行.
读写锁：
	针对多读者，少写者的情况设定,允许多读，但此时不可写；唯一写，此时不可读.
函数的头文件为：
#include <phtread.h>

1. 互斥锁
操作流程：

I. 创建互斥锁
II. 申请锁：若可用，立刻占用；否则，阻塞等待
III. do-something
IV. 释放锁
V. 销毁锁
以下是互斥锁的基本操作函数：

功能	                    函数	           参数	          返回值	 说 明
初始化锁	int pthread_mutex_init(pthread_mutex_t *mutex,const pthread_mutexattr_t *attr)	
1. mutex: 欲建立的互斥锁
2.attr:属性，一般为NULL	成功：0
失败：非零值	

阻塞申请锁	int pthread_mutex_lock(pthread_mutex_t *mutex)	mutex:互斥锁	成功：0
失败：非零值	若未申请到，阻塞等待

非阻塞申请	int pthread_mutex_trylock(pthread_mutex_t *mutex)	mutex:互斥锁	成功：0
失败：非零值	若未申请到，
返回错误

释放锁	int pthread_mutex_unlock(pthread_mutex_t *mutex)	mutex:互斥锁	成功：0
失败：非零值	

销毁锁	int pthread_mutex_destroy(pthread_mutex_t *mutex)	mutex:互斥锁	成功：0
失败：非零值	

2. 条件变量
注意，条件变量必须与互斥锁共同使用；

以下是条件变量的基本操作函数：

功能	函数	参数	返回值	说明
初始化锁	int pthread_cond_init(pthread_cond_t *cond,const pthread_condattr_t *attr)	
1. cond: 欲建立的条件变量
2.attr:属性，一般为NULL	成功：0
失败：非零值	

等待条件变量	int pthread_cond_wait(pthread_cond_t *cond,pthread_mutex_t *mutex)	
1.cond:条件变量
2.mutex:互斥锁	成功：0
失败：非零值	阻塞等待
隐含释放申请到的互斥锁

限时等待条件变量  int pthread_cond_timewait(pthread_cond_t *cond,pthread_mutex_t *mutex,const struct timespec *time)	3.time:等待过期的绝对时间
从1970-1-1:0:0:0起	成功：0
失败：非零值	
struct timespec
{
	long ts_sec;
	long ts_nsec
}

单一通知	int pthread_cond_signal(pthread_cond_t *cond)	cond:条件变量	成功：0
失败：非零值	唤醒等待cond的第一个线程
隐含获取需要的互斥锁

广播通知	int pthread_cond_broadcast(pthread_cond_t *cond)	cond:条件变量	成功：0
失败：非零值	唤醒所有等待cond的线程 
隐含获取需要的互斥锁

销毁条件变量	int pthread_cond_destroy(pthread_cond_t *cond)	cond:条件变量	成功：0
失败：非零值	

3. 读写锁
读写基本原则：

若当前线程读数据，则允许其他线程读数据，但不允许写
若当前线程写数据，则不允许其他线程读、写数据
以下是基本的操作：

功能	函数	参数	返回值	说明
初始化锁	int pthread_rwlock_init(pthread_rwlock_t *rwlock,const pthread_rwlockattr_t *attr)	
1. rwlock: 欲建立的读写锁
2.attr:属性，一般为NULL	成功：0
失败：非零值	

阻塞申请读锁	int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock)	rwlock:读写锁	成功：0
失败：非零值	若未申请到，
阻塞等待

非阻塞申请	int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock)	rwlock:读写锁	成功：0
失败：非零值	若未申请到，
返回错误

阻塞申请写锁	int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock)	rwlock:读写锁	成功：0
失败：非零值	若未申请到，
阻塞等待

非阻塞申请写锁	int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock)	rwlock:读写锁	成功：0
失败：非零值	若未申请到，
返回错误

释放锁	int pthread_mutex_unlock(pthread_rwlock_t *rwlock)	rwlock:读写锁	成功：0
失败：非零值	

销毁锁	int pthread_rwlock_destroy(pthread_rwlock_t *rwlock)	rwlock:读写锁	成功：0
失败：非零值	

4. 线程信号量
线程信号量类似进程的信号量，主要是使得多个线程访问共享资源时，顺序互斥访问。
与互斥锁的区别在于：

互斥锁：只有一个bool类型的值，只允许2个线程进行排队；
信号量：允许多个线程共同等待一个共享资源
函数如下：

     #include <semaphore.h>
功能	函数	参数	返回值	说明
创建信号量	int sem_init(sem_t *sem,int pshared, unsigned int value)	
1. sem:信号量地址; 
2. pshared:是(!=0)否(0)为共享信号量
3. value:信号量初值	
0: 成功-1: 失败	

P操作(阻塞)	int sem_wait(sem_t *sem)	sem:信号量地址	0: 成功 -1: 失败	
P操作(非阻塞)	int sem_trywait(sem_t *sem)	sem:信号量地址	0: 成功 -1: 失败	
P操作(时间)	int sem_timedwait(sem_t *sem,const struct timespec *abs_timeout)	
1. sem:信号量地址
2. abs_timeout:超时时间	0: 成功 -1: 失败	
struct timespec 见下面
V操作	int sem_post(sem_t *sem)	sem:信号量地址	0: 成功  -1: 失败	
获取信号量值	int sem_getvalue(sem_t *sem, int *sval)	
1. sem:信号量地址
2. sval: 将信号量值放到该地址	0: 成功 -1: 失败
	
删除信号量	int sem_destroy(sem_t *sem)	sem:信号量地址	0: 成功  -1: 失败	
struct timespec 
{
               time_t tv_sec;      /* Seconds */
               long   tv_nsec;     /* Nanoseconds [0 .. 999999999] */
};

II. 异步机制 - 信号
线程的异步机制只有信号，类似于线程的信号。

线程信号具备以下特点

任何线程都可以向其它线程(同一进程下)发送信号；
每个线程都具备自己独立的信号屏蔽集，不影响其它线程；
线程创建时，不继承原线程的信号屏蔽集；
同进程下，所有线程共享对某信号的处理方式，即一个设置，所有有效；
多个线程的程序，向某一个线程发送终止信号，则整个进程终止
信号的基本操作如下：

功能	函数	参数	返回值	说明
安装信号	sighandler_t signal( int signum, sighandler_t handler)	1.signum:信号值
2.handler:信号操作	 	详情参见：
http://www.cnblogs.com/Jimmy1988/p/7575103.html

发送信号	int pthread_kill( pthread_t threadid,int signo)	
1.threadid: 目标线程id
2.signo:信号值	成功：0
失败：非零值	若signo=0,
检测该线程是否存在，不发送信号

设置屏蔽集	pthread_sigmask(int how,const sigset_t *set, sigset_t *oldset)	
1.how:如何更改信号掩码
2.newmask:新的信号屏蔽集
3.原信号屏蔽集	成功：0
失败：非零值	how值：
 1.SIG_BLOCK:添加新掩码
 2.SIG_UNBLOCK:删除新掩码
 3.SIG_SETMASK:设置新掩码完全替换旧值
#endif

#if 0
//同步机制：互斥锁，两个线程：读线程：从stdin中读取数据并存储
//写线程：从存储buffer中读取数据并显示
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define SIZE 128

pthread_mutex_t mutex;
int EXIT = 0;
char word[SIZE];

void *child(void*arg)
{
	while(1)
	{
		while(strlen(word) == 0)
		{
			usleep(100);
		}
		pthread_mutex_lock(&mutex);
		printf("The input words: %s\n",word);
		pthread_mutex_unlock(&mutex);
		if(strcmp("end\n",word) ==0)
		{
			printf("The process end\n");
			EXIT = 1;
			break;
		}
		memset(word,'\0',SIZE);
	}
	return NULL;
}

int main(int argc,char*argv[])
{
	pthread_mutex_init(&mutex,NULL);

	pthread_t tid;
	pthread_create(&tid,NULL,(void*)*child,NULL);

	while(0 == EXIT)
	{
		if(strlen(word) != 0)
		{
			usleep(100);
		}
		else
		{
			pthread_mutex_lock(&mutex);
			printf("Input words: ");
			fgets(word,SIZE,stdin);
			pthread_mutex_unlock(&mutex);
		}
	}

	pthread_join(tid,NULL);
	printf("The child has joined\n");
	pthread_mutex_destroy(&mutex);
	return 0;
}
#endif

#if 0
/*
2). 条件变量：
生产者和消费者问题：

生产者:
向仓库生产数据(大小可任意设定)，当满时，阻塞等待仓库有空闲(由消费者消费完后通知)
消费者：
从仓库读数据，若仓库为空，则阻塞等待，当生产者再次生产产品后通知
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define SIZE 2
int Data[SIZE];

typedef struct
{
	pthread_mutex_t lock;
	pthread_cond_t notFull;
	pthread_cond_t notEmpty;
	int read_point;
	int write_point;
}sCOND;

sCOND *pCondLock;

void init(void)
{
	//memset(pCondLock,0,sizeof(sCOND));
	pthread_mutex_init(&pCondLock->lock,NULL);
	pthread_cond_init(&pCondLock->notFull,NULL);
	pthread_cond_init(&pCondLock->notEmpty,NULL);
	pCondLock->read_point = 0;
	pCondLock->write_point = 0;
}

int put(int data)
{
	pthread_mutex_lock(&pCondLock->lock);
	while((pCondLock->write_point+1)%SIZE == pCondLock->read_point)
	{
		printf("%s The buf is full,waitting for not_full signal\n",__func__);
		pthread_cond_wait(&pCondLock->notFull,&pCondLock->lock);
	}

	Data[pCondLock->write_point] = data;
	pCondLock->write_point++;
	
	if(pCondLock->write_point == SIZE)
	{
		pCondLock->write_point = 0;
	}

	pthread_mutex_unlock(&pCondLock->lock);
	pthread_cond_signal(&pCondLock->notEmpty);

	return 0;
}

int get(int*data)
{
	pthread_mutex_lock(&pCondLock->lock);

	while(pCondLock->write_point == pCondLock->read_point)
	{
		printf("%s The buf is empty,waitting for not_empty signal\n",__func__);
		pthread_cond_wait(&pCondLock->notEmpty,&pCondLock->lock);
	}

	*data = Data[pCondLock->read_point];
	pCondLock->read_point++;
	if(pCondLock->read_point == SIZE)
	{
		pCondLock->read_point = 0;
	}

	pthread_cond_signal(&pCondLock->notFull);
	pthread_mutex_unlock(&pCondLock->lock);
	return *data;
}

void *produce(void*arg)
{
	int times = 0;

	for(times = 0;times<5;times++)
	{
		sleep(1);
		put(times+1);
		printf("%s Input data=%d\n",__func__,times+1);
	}

	for(times=5;times<10;times++)
	{
		sleep(3);
		put(times+1);
		printf("%s Input data=%d\n",__func__,times+1);
	}

	return NULL;
}

void *consume(void*arg)
{
	int times = 0;
	int data = 0;

	for(times=0;times<10;times++)
	{
		sleep(2);
		data = get(&data);
		printf("%s The data is %d\n",__func__,data);
	}
}

int main(int argc,char*argv[])
{
	pthread_t tid1,tid2;

	pCondLock = (sCOND*)malloc(sizeof(sCOND));
	memset(pCondLock,'\0',sizeof(sCOND));

	init();
	//2. start two threads
    	pthread_create(&tid1, NULL, produce, NULL);
    	pthread_create(&tid2, NULL, consume, NULL);
    	
    	pthread_join(tid1, NULL);
    	pthread_join(tid2, NULL);

    	free(pCondLock);
	
	return 0;
}
#endif


#if 0
/*
3). 读写锁：
四个线程：两读两写；

多进程可同时读，但此时不可写；
只有一个线程可写，其它线程等待该线程写完后执行响应的读/写操作
*/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

#define BUF_SIZE 128

char buf[BUF_SIZE];

pthread_rwlock_t rwlock;

int time_to_exit = 0;

void *read_first(void *arg);
void *read_second(void *arg);
void *write_first(void *arg);
void *write_second(void *arg);

int main()
{
    pthread_t tid_rd1, tid_rd2;
    pthread_t tid_wr1, tid_wr2;

    //1.create a read-write-lock
    int ret = pthread_rwlock_init(&rwlock, NULL);
    if(ret != 0)
    {
        perror("pthread_rwlock_init");
        exit(EXIT_FAILURE);
    }

    //2. Create the read and write threads
    ret = pthread_create(&tid_rd1, NULL, (void *)*read_first, NULL);
    if(ret != 0)
    {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    ret = pthread_create(&tid_rd2, NULL, (void *)*read_second, NULL);
    if(ret != 0)
    {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    ret = pthread_create(&tid_wr1, NULL, (void *)*write_first, NULL);
    if(ret != 0)
    {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    ret = pthread_create(&tid_wr2, NULL, (void *)*write_second, NULL);
    if(ret != 0)
    {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }


    //3. wait for the threads finish
    pthread_join(tid_rd1, NULL);
    pthread_join(tid_rd2, NULL);
    pthread_join(tid_wr1, NULL);
    pthread_join(tid_wr2, NULL);

    //4. delete the read-write-lock
    pthread_rwlock_destroy(&rwlock);

    return 0;
}

/***************************************************/
// Write threads
void *write_first(void *arg)
{
    while(!time_to_exit)
    {
        sleep(5);
        
        //1. get the read-lock    
        pthread_rwlock_wrlock(&rwlock);
        printf("\nThis is thread write_first!\n");

        printf("Pls input the string: ");
        fgets(buf, BUF_SIZE, stdin);

        pthread_rwlock_unlock(&rwlock);
    }

    printf("Exit the write_first!\n");
    pthread_exit(0);
}

void *write_second(void *arg)
{
    while(!time_to_exit)
    {
        sleep(10);
        
        //1. get the read-lock    
        pthread_rwlock_wrlock(&rwlock);
        printf("\nThis is thread write_second!\n");

        printf("Pls input the string: ");
        fgets(buf, BUF_SIZE, stdin);

        pthread_rwlock_unlock(&rwlock);
    }

    printf("Exit the write_second!\n");
    pthread_exit(0);
}


//-----2. read the threads
void *read_first(void *arg)
{
    while(1)
    {
        sleep(5);
        pthread_rwlock_rdlock(&rwlock);
        printf("\nThis is thread read_first\n");

        //if write an string of "end"
        if(!strncmp("end", buf, 3))
        {
            printf("Exit the read_first!\n");
            break;
        }
        
        //if nothing in the BUFFER
        while(strlen(buf) == 0)
        {
            pthread_rwlock_unlock(&rwlock);
            sleep(2);
            pthread_rwlock_rdlock(&rwlock);
        }
        
        //output the string in BUFFER
        printf("The string is: %s\n", buf);

        pthread_rwlock_unlock(&rwlock);
    }

    pthread_rwlock_unlock(&rwlock);

    //make the exit true
    time_to_exit = 1;

    pthread_exit(0);
}

void *read_second(void *arg)
{
    while(1)
    {
        sleep(4);

        pthread_rwlock_rdlock(&rwlock);
        printf("\nThis is thread read_second\n");

        //if write an string of "end"
        if(!strncmp("end", buf, 3))
        {
            printf("Exit the read_second!\n");
            break;
        }
        
        //if nothing in the BUFFER
        while(strlen(buf) == 0)
        {
            pthread_rwlock_unlock(&rwlock);
            sleep(2);
            pthread_rwlock_rdlock(&rwlock);
        }
        
        //output the string in BUFFER
        printf("The string is: %s\n", buf);

        pthread_rwlock_unlock(&rwlock);
    }

    pthread_rwlock_unlock(&rwlock);

    //make the exit true
    time_to_exit = 1;

    pthread_exit(0);
}
#endif

#if 1
/*
2. 异步机制 - 信号：
本程序包括两个线程：

线程1安装SIGUSR1,阻塞除SIGUSR2外的所有信号；
线程2安装SIGUSR2,不阻塞任何信号
操作流程：

1- 线程1、2安装信号；
2- 主线程发送SIGUSR1和SIGUSR2至线程1和线程2；
3- 线程1接收到除SIGUSR2之外的信号，阻塞不执行；当收到SIGUSR2后，执行对应操作；
4- 线程2接收到SIGUSR1和SIGUSR2后，分别执行对应操作
5- 主线程发送SIGKILL信号，结束整个进程
*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>

void *th_first(void *arg);

void *th_second(void *arg);

pthread_t tid1, tid2;

void handler(int signo)
{
    printf("In handler: tid_%s, signo=%d\n", ((pthread_self() == tid1)?"first":"second"), signo);
}

int main()
{
    int ret = 0;
    //1. create first thread
    ret = pthread_create(&tid1, NULL, (void *)*th_first, NULL);
    if(0 !=ret)
    {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }
    //2. create second thread
    ret = pthread_create(&tid2, NULL, (void *)*th_second, NULL);
    if(0 !=ret)
    {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    sleep(2);
    //3. send the signal of SIG_USER1 and SIG_USER2 to thread_first
    ret = pthread_kill(tid1, SIGUSR1);
    if(0 !=ret)
    {
        perror("pthread_kill");
        exit(EXIT_FAILURE);
    }

    ret = pthread_kill(tid1, SIGUSR2);
    if(0 !=ret)
    {
        perror("pthread_kill");
        exit(EXIT_FAILURE);
    }

    //4. send the signal of SIG_USER1 and SIG_USER2 to thread_second_
    sleep(1);
    ret = pthread_kill(tid2, SIGUSR1);
    if(0 !=ret)
    {
        perror("pthread_kill");
        exit(EXIT_FAILURE);
    }

    ret = pthread_kill(tid2, SIGUSR2);
    if(0 !=ret)
    {
        perror("pthread_kill");
        exit(EXIT_FAILURE);
    }

    sleep(1);
    //5. send SIGKILL to all threads
    ret = pthread_kill(tid1, SIGKILL);
    if(0 !=ret)
    {
        perror("pthread_kill");
        exit(EXIT_FAILURE);
    }

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    return 0;
}


void *th_first(void *arg)
{
    //1. Add SIGUSR1 signal
    signal(SIGUSR1, handler);
    
    //2. Set the sinagl set
    sigset_t set;
    sigfillset(&set);                           //init set to be full, include all signal
    sigdelset(&set, SIGUSR2);                   //delete the SIGUSR2 from the set variable
    pthread_sigmask(SIG_SETMASK, &set, NULL);   //set the current mask set to be defined set variable
    
    //3. Circular wait the signal
    int i;
    for(i=0; i<5; i++)
    {
        printf("\nThis is th_first, tid=%#x\n ", pthread_self());
        pause();
    }
}

void *th_second(void *arg)
{
    usleep(100);
    //1. Add the signal of SIGUSR2
    signal(SIGUSR2, handler);

    //2. Circular wait the signal
    int i;
    for(i=0; i<5; i++)
    {
        printf("\nThis is th_second, tid=%#x\n", pthread_self());
        pause();
    }
}
#endif
