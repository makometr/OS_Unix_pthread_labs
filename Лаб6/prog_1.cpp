#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h> 
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <time.h>
#include <stdlib.h>

int flag = 0;
sem_t* semaphoreID_access;
sem_t* semaphoreID_ready;
FILE* shared_file;
int* sharedMemPtr;

void* thread_func(void* array_ptr) {
    int count = 0;
    while (flag != 1){
        int sem_status_ready;
        sem_getvalue(semaphoreID_ready, &sem_status_ready);
        if (sem_status_ready == 0){ // Если чебурек куплен -> делаем новый.
            count = 0;
            int number = random() % 100;
            sem_wait(semaphoreID_access); // Закрывем доступ к чебуреку.
            memcpy(sharedMemPtr, &number, sizeof(int));
            printf("Random write: %d\n", *sharedMemPtr);
            fflush(stdout);
            sem_post(semaphoreID_access); // Открывыем доступ к чебуреку.
            sem_post(semaphoreID_ready); // Говорим, что чебурек приготовлен.
        }
        else if (sem_status_ready == 1){ // Если чебурек всё ещё новый, то считаем до 5.
            count++;
            if (count == 5){ // Чебуреки никто не покупает, закрываемся.
                printf("Reader is dead!\nPress any key to close the program.\n");
                return NULL;
            }
        }
        sleep(1);
    }
    return NULL;
}

int main(int argc, char* argv[]){
    srand(time(NULL));
    pthread_t id_thread;
    char semaphoreName_1[] = {"/access"};
    char semaphoreName_2[] = {"/ready"};
    char shmPath[] = {"/cheburek"};

    int shm = shm_open(shmPath, O_CREAT|O_RDWR, 0644);
    ftruncate(shm, sizeof(int));
    sharedMemPtr = (int*)mmap(0, sizeof(int), PROT_WRITE|PROT_READ, MAP_SHARED, shm, 0);

    semaphoreID_access = sem_open(semaphoreName_1, O_CREAT, 0644, 1);
    semaphoreID_ready = sem_open(semaphoreName_2, O_CREAT, 0644, 1);
    
    int sem_status_ready;
    sem_getvalue(semaphoreID_ready, &sem_status_ready);
    if (sem_status_ready == 1)
        sem_wait(semaphoreID_ready); // Как только палатку открыли - чебурек ещё не готов.

    pthread_create(&id_thread , NULL , thread_func , NULL);

    getchar();
    flag = 1;

    pthread_join(id_thread, NULL);
    sem_unlink(semaphoreName_1);
    sem_unlink(semaphoreName_2);
    munmap(sharedMemPtr, sizeof(int));
    close(shm);
    shm_unlink(shmPath);
    return 0;
}