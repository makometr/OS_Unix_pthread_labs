#include <stdio.h>
#include <unistd.h> 
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
    pid_t pid = fork();
    if (pid != 0){ // Родительский процесс
        printf("Parent process ID from prog_2 from parent: %d\n", getppid());
        printf("Program process ID from prog_2 from parent: %d\n", getpid());

        int status;  
        while (waitpid(-1, &status, WNOHANG) == 0){
            printf("Wait...\n");
            usleep(500 * 1000);
        }
        if (WIFEXITED(status))
            printf("Process successfully ended with status: %d\n", WEXITSTATUS(status));
        else
            printf("Process ended unsuccessfully!\n");
    }
    else { // дочерний процесс
        printf("Parent process ID from prog_2 from child: %d\n", getppid());
        printf("Program process ID from prog_2 from child: %d\n", getpid());
        if (argc == 1)
            execlp("./prog1", "prog1", NULL);
        if (argc == 2)
            execlp("./prog1", "prog1", argv[1],  NULL);
        if (argc == 3)
            execlp("./prog1", "prog1", argv[1], argv[2], NULL);
        if (argc == 4)
            execlp("./prog1", "prog1", argv[1], argv[2], argv[3], NULL);
        if (argc > 4){
            printf("Warning! Only 3 args out of %d are used!\n", argc-1);
            execlp("./prog1", "prog1", argv[1], argv[2], argv[3], NULL);
        }
    }
    return 0;
}