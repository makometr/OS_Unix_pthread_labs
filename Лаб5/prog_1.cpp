#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h> 
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>

int flag = 0;
sem_t* semaphoreID;
FILE* shared_file;

void* thread_func(void* array_ptr) {
    char* array = (char*)array_ptr;

    while (flag != 1){
        sem_wait(semaphoreID);
        for (int i = 0; i < strlen(array); i++){
            printf("%c", array[i]);
            fprintf(shared_file, "%c", array[i]);
        }
        fflush(stdout);
        fflush(shared_file);
        sem_post(semaphoreID);
        sleep(1);
    }
    return NULL;
}

int main(int argc, char* argv[]){
    printf("Non blocking\n");
    pthread_t id_thread;

    char semaphoreName[] = {"/alisa"};

    semaphoreID = sem_open(semaphoreName, O_CREAT, 0644, 1);
    if (semaphoreID == NULL){
        printf("%s\n", strerror(errno));
        return 0;
    }
    shared_file = fopen("data.txt", "a+");

    char data_for_thread[] = {"aaaaa"};

    pthread_create(&id_thread , NULL , thread_func , &data_for_thread);

    getchar();
    flag = 1;

    pthread_join(id_thread, NULL);
    sem_unlink(semaphoreName);
    return 0;
}