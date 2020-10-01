#include <iostream>
#include <unistd.h>
#include <stdio.h>

#include <pthread.h>
#include <string>
#include <string.h>

using namespace std;

constexpr size_t BUFF_SIZE = 32;

int flag1 = 0; // объявление флагов завершения потока
int flag2 = 0; // объявление флагов завершения потока
int field[2];

void* func1(void* nothing) {
    string buffer;
    int divisionNum = 1;
    
    while (flag1 != 1){
        buffer.clear();
        buffer += "We need " + to_string(divisionNum) + " divisions more!";
        write(field[1], buffer.c_str(), strlen(buffer.c_str())+1);
        if (strlen(buffer.c_str()) >= BUFF_SIZE)
            cerr << "Msg is bigger than buffer! Memory may be corrupted!\n";
        divisionNum++;
        sleep(1);
    }

    return NULL;
}

void* func2(void* nothing) {
    char buffer[BUFF_SIZE] = {'\0'};

    while (flag2 != 1){
        memset(buffer, 0, BUFF_SIZE);
        int bytesReaded = read(field[0], buffer, BUFF_SIZE);
        cout << buffer << endl;
        cout << "Size of msg: " << bytesReaded-1 << endl;
        sleep(1);
    }
    return NULL;
}

int main(){
    pthread_t id1; // Объявление идентификатора для первого потока
    pthread_t id2; // Объявление идентификатора для второго потока

    pipe(field); // Создаем канал

    pthread_create (&id1 , NULL , func1 , NULL); // Создание первого потока
    pthread_create (&id2 , NULL , func2 , NULL); // Создание второго потока

    getchar();
    
    flag1 = 1; // Установка флага завершения для первого потока
    flag2 = 1; // Установка флага завершения для второго потока

    pthread_join(id1, NULL); // Ожидание завершения первого потока
    pthread_join(id2, NULL); // Ожидание завершения второго потока

    close(field[0]);
    close(field[1]);
}