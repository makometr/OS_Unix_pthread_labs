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

int main(int argc, char* argv[]){
    srand(time(NULL));
    pthread_t id_thread;
    char fifoName[] = {"/home/fifo"};

    int fifo_create_status = mkfifo(fifoName, 0666);
    cout << fifo_create_status << endl;
    cout << strerror(errno) << endl;
    fd = open(fifoName, O_WRONLY);
    cout << "FD: " << fd << endl;


    pthread_create(&id_thread , NULL , thread_func , NULL);

    getchar();
    flag = 1;

    pthread_join(id_thread, NULL);
    close(fd);
    unlink(fifoName);
    return 0;
}