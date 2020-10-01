#include <stdio.h>
#include <unistd.h> 
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sched.h>
#include <stdlib.h>
#include <errno.h>

static int func_to_execute_in_clone(void* arg){
    printf("Parent process ID from prog_2 from child: %d\n", getppid());
    printf("Program process ID from prog_2 from child: %d\n", getpid());

    char* arg_1_ptr = (char*)arg;
    if (arg != NULL){
        char* arg_2_ptr = arg_1_ptr + strlen(arg_1_ptr) + 1;
        char* arg_3_ptr = arg_2_ptr + strlen(arg_2_ptr) + 1;
        execlp("./prog1", "prog1", arg_1_ptr, arg_2_ptr, arg_3_ptr, NULL);
    }
    else {
        execlp("./prog1", "prog1", NULL);
    }
    return 0;
}

int main(int argc, char* argv[]){
    printf("Parent process ID from prog_2 from parent: %d\n", getppid());
    printf("Program process ID from prog_2 from parent: %d\n", getpid());

    const int STACK_SIZE = 65536;
    void* stack = malloc(STACK_SIZE);
    if (argc < 3+1)
        printf("\nWarning! Few arguments! Env vars can be displayed!\n\n");
    if (argc > 3+1)
        printf("\nWarning! Only 3 args out of %d are used!\n\n", argc-1);

    pid_t code = clone(func_to_execute_in_clone, (char*)stack+STACK_SIZE, SIGCHLD, argv[1]);
    
    if (code == -1){
        printf("New process did not created! Error: %s\n", strerror(errno));
        return 0;
    }

    int status;
    while (waitpid(-1, &status, WNOHANG) == 0){
        printf("Wait...\n");
        usleep(500 * 1000);
    }
    if (WIFEXITED(status))
        printf("Process successfully ended with status: %d\n", WEXITSTATUS(status));
    else
        printf("Process ended unsuccessfully!\n");
    return 0;
}