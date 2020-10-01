#include <pthread.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>

using namespace std;

void* func1(void * argFl){
    int *fl1 = (int*)argFl;

    while( *fl1 != 1 ){
        cout << "1" << flush;
        sleep (2);
    }

    int* codeReturn1 = new int(3); // Объявление переменной для вывода кода завершения и присваевываем ей значение по адресу
    pthread_exit(codeReturn1); // Завершение потока с кодом завершения
}

void* func2(void * argFl){
    int *fl2 = (int*)argFl;

    while( *fl2 != 1 ){
        cout << "2" << flush;
        sleep (2);
    }

    int* codeReturn2 = new int; // Объявление переменной для вывода кода завершения
    *codeReturn2 = 4;
    pthread_exit(codeReturn2); // Завершение потока с кодом завершения
}

int main(){
    pthread_t id1; // Объявление идентификатора для первого потока
    pthread_t id2; // Объявление идентификатора для второго потока

    int flag1 = 0; // объявление флагов завершения потока
    int flag2 = 0; // объявление флагов завершения потока

    pthread_create (&id1 , NULL , func1 , (void*)&flag1); // Создание первого потока
    sleep(1);
    pthread_create (&id2 , NULL , func2 , (void*)&flag2); // Создание второго потока

    getchar(); //Ожидание нажатия клавиши

    flag1 = 1; // Установка флага завершения для первого потока
    flag2 = 1; // Установка флага завершения для второго потока

    int* codeReturn1; // Объявление переменной для данных из первого потока
    int* codeReturn2; // Объявление переменной для данных из второго потока

    pthread_join(id1, (void**)&codeReturn1); // Ожидание завершения первого потока
    pthread_join(id2, (void**)&codeReturn2); // Ожидание завершения второго потока

    cout << *codeReturn1 << endl; // Вывод кодов завершения потоков
    cout << *codeReturn2 << endl; // Вывод кодов завершения потоков

    delete(codeReturn1);
    delete(codeReturn2);
}