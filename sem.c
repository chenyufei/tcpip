#include <stdio.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
void main()
{
	key_t unique_key;
	int id;
	struct sembuf lock_it;
	union semun options;
	int i;
	unique_key = ftok(".", 'a');
	id = semget(unique_key, 1, IPC_CREAT | IPC_EXCL | 0666);
	printf("semaphore id=%d\n", id);
	options.val = 1; 
	semctl(id, 0, SETVAL, options); 
	i = semctl(id, 0, GETVAL, 0);
	printf("value of semaphore at index 0 is %d\n", i);
	lock_it.sem_num = 0; 
	lock_it.sem_op = -1; 
	lock_it.sem_flg = IPC_NOWAIT; 
	if (semop(id, &lock_it, 1) == -1) {
		printf("can not lock semaphore.\n");
		exit(1);
	}
	i = semctl(id, 0, GETVAL, 0);
	printf("value of semaphore at index 0 is %d\n", i);
	semctl(id, 0, IPC_RMID, 0);
}
