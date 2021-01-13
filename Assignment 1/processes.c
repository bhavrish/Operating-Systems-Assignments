#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>

int main(int argc, char *argv[]) {
    pid_t pid;
    pid_t pid2;
    int fd[2]; // 0th posistion is read-end of pipe, 1st position is write-end of pipe

    if (pipe(fd) == -1) {
        printf("Could not open pipe.");
        return 1;
    }

    pid = fork();
    pid2 = fork();

    if (pid < 0 || pid2 < 0) {
        fprintf(stderr, "Fork Failed");
        return 1;
    }
    else if (pid == 0 && pid2 > 0) { // child process 1
        close(fd[0]); // close read end of pipe since we are not currently reading from it
        dup2(fd[1], STDOUT_FILENO); // redirect writing from stdout to fd[1] (write end of pipe)
        execlp("ls", "ls", "-F", NULL);
        exit(0);
    }
    else if (pid2 == 0 && pid > 0) { // child process 2
        close(fd[1]); // close write end of pipe since we are no longer writing to it 
        dup2(fd[0], STDIN_FILENO); // redirect reading from stdin to fd[0] (read end of pipe) 
        execlp("nl", "nl", NULL);
        exit(0);
    }
    else {
        close(fd[0]); // close read end of pipe in parent
        close(fd[1]); // close write end of pipe in parent
        waitpid(pid); // parent waits for child 1 to complete
        waitpid(pid2); // parent waits for child 2 to complete
    }
    
    return 0;
}