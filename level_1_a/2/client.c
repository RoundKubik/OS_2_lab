#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include "shmem.h"
void main(void)
{
Message* msgptr;
key_t key;
int shmid, semid;

if((key = ftok("key.txt", 'A')) < 0)
{
printf("Client: can't get a key\n");
exit(-1);
}

if((shmid = shmget(key, sizeof(Message), 0)) < 0)
{
printf("Client: access denied\n");
exit(-1);
}

if((msgptr = (Message*)shmat(shmid, 0, 0)) < 0)
{
printf("Client : error of joining\n");

exit(-1);
}

if((semid = semget(key, 2, PERM)) < 0)
{
printf("Client: access denied\n");
exit(-1);
}

if(semop(semid, &mem_lock[0], 2) < 0)
{
printf("Client : can't execute a operation\n");
exit(-1);
}

if(semop(semid, &proc_start[0], 1) < 0)
{
printf("Client : can't execute a operation\n");
exit(-1);
}

sprintf(msgptr->buff,"Message from client with PID = %d\n", getpid());

if(semop(semid, &mem_unlock[0], 1) < 0)
{
printf("Client: can't execute a operation\n");
exit(-1);
}


if(semop(semid, &mem_lock[0], 2) < 0)
{
printf("Client: can't execute a operation\n");
exit(-1);
}

printf("Client: read message\n%s", msgptr->buff);

if(semop(semid, &mem_unlock[0], 1) < 0)
{
printf("Client: can't execute a operation\n");
exit(-1);
}

if(shmdt(msgptr) < 0)
{
printf("Client: error\n");
exit(-1);
}
}
