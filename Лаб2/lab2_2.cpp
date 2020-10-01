#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <errno.h>

using namespace std;

int flag1 = 0; // объявление флагов завершения потока
int flag2 = 0; // объявление флагов завершения потока
sem_t semaphoreID; // объявляем идентификатор семафора

void* func1(void* array_ptr) {
    char* array = (char*)array_ptr;

    while (flag1 != 1){
        int block_value = sem_trywait(&semaphoreID);
        if (block_value == -1){
            cout << "Error from 1 thread: " << strerror(errno) << endl;
            sleep(1);
            continue;
        }
        for (int i = 0; i < strlen(array); i++){
            cout << array[i];
        }
        cout << endl;
        sem_post(&semaphoreID);
        sleep(1);
    }
    void* returned_value = NULL;
    return returned_value;
}

void* func2(void* array_ptr) {
    char* array = (char*)array_ptr;

    while (flag2 != 1){
        int block_value = sem_trywait(&semaphoreID);
        if (block_value == -1){
            cout << "Error from 2 thread: " << strerror(errno) << endl;
            sleep(1);
            continue;
        }
        for (int i = 0; i < strlen(array); i++){
            cout << array[i];
        }
        cout << endl;
        sem_post(&semaphoreID);
        sleep(1);
    }
    void* returned_value = NULL;
    return returned_value; 
}

int main(){
    pthread_t id1; // Объявление идентификатора для первого потока
    pthread_t id2; // Объявление идентификатора для второго потока

    sem_init(&semaphoreID, false, 1);

    char array_1[] = {"abcdeabcdeabcdeabcdeabcdeabcde\0"};
    char array_2[] = {"123451234512345123451234512345\0"};

    pthread_create (&id1 , NULL , func1 , &array_1); // Создание первого потока
    pthread_create (&id2 , NULL , func2 , &array_2); // Создание второго потока

    getchar();
    flag1 = 1; // Установка флага завершения для первого потока
    flag2 = 1; // Установка флага завершения для второго потока

    pthread_join(id1, NULL); // Ожидание завершения первого потока
    pthread_join(id2, NULL); // Ожидание завершения второго потока

    sem_destroy(&semaphoreID);
}