/**
 * The following program demonstrates the use of semaphores to handle critical sessions
 * for alarm triggering purpose, we have two common variables alarmset and alarmTime which can be given as
 * sensor inputs. We initialize it ourselves for now. The alarma is triggered when the comparison is done inside the threads
 * Thread 1 checks the data and does clean exit. Thread 2 check and triggers the alarm
 */
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

sem_t mutex;
pthread_t tid[2];
int alarmset, alarmTime;

struct thread_data
{
    int thread_id;
};

void *thread_alarm(void *arg)
{
    //wait
    sem_wait(&mutex);
    printf("\nEntered.. %d \n", ((struct thread_data *)arg)->thread_id);

    //critical section
    sleep(4);
    if (alarmTime > alarmset)
    {
        printf("Trigger LED Alarm...");
    }

    // Uncomment below two lines for inc values via thread 1
    // alarmTime++;
    // alarmTime++;
    printf("\nJust Exiting...%d \n", ((struct thread_data *)arg)->thread_id);
    sem_post(&mutex);
}

int main(void)
{
    int i = 0;
    sem_init(&mutex, 0, 1);
    int error;
    alarmset = 1;
    alarmTime = 0;
    while (i < 2)
    {
        struct thread_data *thread_info = (struct thread_data *)malloc(sizeof(struct thread_data));
        thread_info->thread_id = i;
        error = pthread_create(&(tid[i]), NULL, &thread_alarm, (void *)thread_info);
        if (error != 0)
            printf("\nThread can't be created : [%s]", strerror(error));
        i++;
    }

    pthread_join(tid[0], NULL);
    // Below two lines are used to inc values globally for triggering thread 2 alarm.
    // comment the below line and uncomment the lines in function to achieve via thread 1
    alarmTime++;
    alarmTime++;
    pthread_join(tid[1], NULL);
    sem_destroy(&mutex);
    return 0;
}