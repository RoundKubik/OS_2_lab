#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include "shmem.h"
Message* msgptr;
int shmid, semid;
void hndlr(int sig) 
{
signal(SIGINT, hndlr);

if(shmdt(msgptr) < 0)
{
printf("Server: error\n");
exit(-1);
}

if(shmctl(shmid, IPC_RMID, 0) < 0)
{
printf("Server : can't delete area\n");
exit(-1);
}
printf("Server: area is deleted\n");
if(semctl(semid, 0, IPC_RMID) < 0)
{
printf("Server : can't delete semaphore\n");
exit(-1);
}
printf("Server: semaphores are deleted\n");
}
void main(void)
{
key_t key;
signal(SIGINT, hndlr);

if((key = ftok("key.txt", 'A')) < 0)
{
printf("Server: can't get a key\n");
exit(-1);
}

if((shmid = shmget(key, sizeof(Message), PERM | IPC_CREAT))< 0)
{
printf("Server: can't create an area\n");
exit(-1);
}
printf("Server: area is created\n");

if((msgptr = (Message*)shmat(shmid, 0, 0)) < 0)
{
printf("Server: error of joining\n");
exit(-1);
}
printf("Server: area is joined\n");

if((semid = semget(key, 2, PERM | IPC_CREAT)) < 0)
{
printf("Server: can't create a semaphore\n");
exit(-1);
}
printf("Server: semaphores are created\n");
while(1)
{

if(semop(semid, &proc_wait[0], 1) < 0)
{
printf("Server: execution complete\n");
exit(-1);
}

if(semop(semid, &mem_lock[0], 2) < 0)
{
printf("Server: can't execute a operation\n");
exit(-1);
}

printf("Server: read message\n%s", msgptr->buff);
sprintf(msgptr->buff,"Message from server with PID =%d\n", getpid());

if(semop(semid, &mem_unlock[0], 1) < 0)
{
printf("Server: can't execute a operation\n");
exit(-1);
}
}
}
