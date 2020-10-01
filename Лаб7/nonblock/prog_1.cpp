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
#include <signal.h>

using namespace std;
constexpr size_t BUFF_SIZE = 32;
char fifoName[] = {"/home/fifo"};
int flag = 0;
size_t fd;
pthread_t id_thread_open;
pthread_t id_thread_exec;

void* thread_func(void* array_ptr) {
    string buffer;
    int divisionNum = 1;
    
    while (flag != 1){
        buffer.clear();
        buffer += "We need " + to_string(divisionNum) + " divisions more!";
        write(fd, buffer.c_str(), strlen(buffer.c_str())+1);
        if (strlen(buffer.c_str()) >= BUFF_SIZE)
            cerr << "Msg is bigger than buffer! Memory may be corrupted!\n";
        divisionNum++;
        sleep(1);
    }

    return NULL;
}

void* open_fifo_thread(void* nnull){
    while (flag != 1){
        fd = open(fifoName, O_WRONLY | O_NONBLOCK);
        if (fd == -1){
            cout << strerror(errno) << endl;
            cout << "FD: " << fd << endl;
            sleep(1);
        }
        else {
            pthread_create(&id_thread_exec , NULL , thread_func , NULL);
            return NULL;
        }   
    }
    return NULL;
}

void sig_handler(int signo){
    cout << "SIGPIPE is gotten!\n";
}

int main(int argc, char* argv[]){
    signal(SIGPIPE, sig_handler);
    srand(time(NULL));

    int fifo_create_status = mkfifo(fifoName, 0666);
    cout << fifo_create_status << endl;


    pthread_create(&id_thread_open , NULL , open_fifo_thread , NULL);
    std::cout << "D!\n";

    getchar();
    flag = 1;

    pthread_join(id_thread_open, NULL);
    pthread_join(id_thread_exec, NULL);
    
    close(fd);
    unlink(fifoName);
    return 0;
}