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
        if (sem_status_ready == 1){ // Если чеберек готов, то съедаем его.
            count = 0;
            int readedNum;
            sem_wait(semaphoreID_access); // Блокируем доступ.
            memcpy(&readedNum, sharedMemPtr, sizeof(int)); // Кушаем.
            printf("Random read: %d\n", readedNum);
            fflush(stdout);
            sem_wait(semaphoreID_ready); // Говорим, что чебурек съеден.
            sem_post(semaphoreID_access); // Открываем доступ.
        }
        else if (sem_status_ready == 0){ // Если чебурек не готов, то считаем до 5.
            count++;
            if (count == 5){ // Чебуреки никто не делает, видимо, палатка закрылась, уходим.
                printf("Writer is dead!\nPress any key to close the program.\n"); 
                return NULL;
            }
        }
        sleep(1);
    }
    return NULL;
}

int main(int argc, char* argv[]){
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
        sem_wait(semaphoreID_ready); // Если мы только пришли, а чебурек уже готов, то он чужой - выкидываем его.

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