#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>

#define NBUFF 6

struct
{
    int buff[NBUFF];
    sem_t mutex, nempty, nstored;
} shared;

void *produce(void *), *consume(void *);


int main(int argc, char **argv)
{
    pthread_t tid_produce, tid_consume;
// создание трех семафоров
    sem_init(&shared.mutex, 0, 1);
    sem_init(&shared.nempty, 0, NBUFF);
    sem_init(&shared.nstored, 0, 0);
    pthread_create(&tid_produce, NULL, produce, NULL);
    pthread_create(&tid_consume, NULL, consume, NULL);
    pthread_join(tid_produce, NULL);
    pthread_join(tid_consume, NULL);
    
    sem_unlink(&shared.mutex);
    sem_unlink(&shared.nempty);
    sem_unlink(&shared.nstored);
    
    exit(0);
}
void *produce(void *arg)
{
    for(int i = 0; i < NBUFF; i++)
    {
        printf("\n");
        if(i == NBUFF - 1)
        {
            i = 0;
        }
        printf("Producer: уменьшаем количество пустых ячеек\n");
        sem_wait(&shared.nempty);
        printf("Producer: переводим мютекс в режим производителя\n");
        sem_wait(&shared.mutex);

        printf("Producer: записал %d в буфер.\n", i);
        shared.buff[i] = i;
// помещаем i в циклический буфер
        printf("Producer: возвращаю мютекс обратно.\n");
        sem_post(&shared.mutex);
        printf("Producer: увеличиваю количество заполненных ячеек.\n");
        sem_post(&shared.nstored);
        sleep(1);
    }

    return(NULL);
}
void *consume(void *arg)
{
    for(int i = 0; i < NBUFF; i++)
    {
        printf("\n");
        if(i == NBUFF - 1)
        {
            i = 0;
        }
        printf("Consumer: уменьшаю количество заполненых ячеек\n");
        sem_wait(&shared.nstored);
        printf("Consumer: переводим мютекс в режим производителя\n");
        sem_wait(&shared.mutex);

        i = shared.buff[i];
        printf("Consumer: получил %d из буфера.\n", i);
// помещаем i в циклический буфер
        printf("Consumer: возвращаю мютекс обратно.\n");
        sem_post(&shared.mutex);
        printf("Consumer: увеличиваю количество пустых ячеек.\n");
        sem_post(&shared.nempty);
        sleep(1);
    }

    return(NULL);
}
