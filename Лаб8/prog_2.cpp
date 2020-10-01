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
#include <mqueue.h>

using namespace std;
constexpr size_t BUFF_SIZE = 32;
char queue_name[] = {"/test_queue"};
int flag = 0;
mqd_t qd;

void* thread_func(void* array_ptr) {
    char buffer[BUFF_SIZE] = {'\0'};

    while (flag != 1){
        memset(buffer, 0, BUFF_SIZE);
        int bytesReaded = mq_receive(qd, buffer, BUFF_SIZE, NULL);
        if (bytesReaded > 0){
            cout << buffer << endl;
            cout << "Size of msg: " << bytesReaded-1 << endl;
        }
        else {
            cout << "Error No.: " << errno << "\nError Descr: " << strerror(errno) << endl;
            sleep(1);
        }
    }
    return NULL;
}

int main(int argc, char* argv[]){
    srand(time(NULL));

    pthread_t id_thread_open;
    qd = mq_open(queue_name, O_CREAT | O_RDONLY | O_NONBLOCK, 0644, NULL);
    cout << "Queue id: " << qd << endl;
    if (qd == -1){
        cout << "Error No.: " << errno << "\nError Descr: " << strerror(errno) << endl;
        return 0;
    }

    pthread_create(&id_thread_open , NULL , thread_func , NULL);

    getchar();
    flag = 1;

    pthread_join(id_thread_open, NULL);
    
    close(qd);
    mq_close(qd);
    mq_unlink(queue_name);
    return 0;
}