#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h> 
#include <errno.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <sys/types.h>
#include <fcntl.h>

using namespace std;
constexpr size_t BUFF_SIZE = 32;
int flag = 0;
size_t fd;

void* thread_func(void* array_ptr) {
    char buffer[BUFF_SIZE] = {'\0'};

    while (flag != 1){
        memset(buffer, 0, BUFF_SIZE);
        int bytesReaded = read(fd, buffer, BUFF_SIZE);
        cout << buffer << endl;
        cout << "Size of msg: " << bytesReaded-1 << endl;
        sleep(1);
    }
    return NULL;
}

int main(int argc, char* argv[]){
    srand(time(NULL));
    pthread_t id_thread;
    char fifoName[] = {"/home/fifo"};

    int fifo_create_status = mkfifo(fifoName, 0644);
    fd = open(fifoName, O_RDONLY | O_NONBLOCK);
    cout << "FD: " << fd << endl;


    pthread_create(&id_thread , NULL , thread_func , NULL);

    getchar();
    flag = 1;

    pthread_join(id_thread, NULL);
    close(fd);
    unlink(fifoName);
    return 0;
}