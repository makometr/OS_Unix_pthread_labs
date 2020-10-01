#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[]){
    printf("Parent process ID from prog_1: %d\n", getppid());
    printf("Program process ID from prog_1: %d\n", getpid());
    if (argc == 1){
        printf("No args!\n");
        return 1;
    }
    else {
        for (int i = 1; i < argc; i++){
            sleep(1);
            printf("%s \n", argv[i]);
        }
        printf("\n");
    }
    return 15;
}