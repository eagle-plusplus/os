#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char **argv){

/*check if input file has been given*/
if(argc >= 2){
    int fd[2];
    int fdd[2];

        /*create pipes*/
        if (pipe(fd) < 0){
            perror("pipe error :");
            return 1;
        }

        if (pipe(fdd) < 0){
            perror("pipe error :");
            return 1;
        }

        int pid1;
        if ( (pid1 = fork()) < 0){
            perror("fork error : ");
            return 1;
        }

        if (pid1 == 0){  /* child1: read from file */
            dup2(fd[1], STDOUT_FILENO);
            close(fd[0]);
            close(fd[1]);
            close(fdd[0]);
            close(fdd[1]);
            execlp("cat", "cat", argv[1], NULL);
            printf("Error while trying to read from file");
            return 1;
        }

        int pid2;
        if ( (pid2 = fork()) < 0){
            perror("fork error : ");
            return 1;
        }

        if (pid2 == 0){  /* child2: make lowercase to uppercase */
            dup2(fd[0], STDIN_FILENO);
            dup2(fdd[1], STDOUT_FILENO);
            close(fd[0]);
            close(fd[1]);
            close(fdd[0]);
            close(fdd[1]);
            execlp("tr", "tr", "[:lower:]", "[:upper:]", NULL);
            printf("Error while trying to make lowercase to uppercase");
            return 1;
        }

        int pid3;
        if ( (pid3 = fork()) < 0){
            perror("fork error : ");
            return 1;
        }

        if (pid3 == 0){  /* child3: put "Data received through pipe " message before printing to terminal */
            dup2(fdd[0], STDIN_FILENO);
            close(fd[0]);
            close(fd[1]);
            close(fdd[0]);
            close(fdd[1]);
            execlp("sed", "sed", "s/^/Data received through pipe /", STDIN_FILENO, NULL);
            printf("Error while trying to put 'Data received through pipe ' message before printing to terminal");
            return 1;
        }

        if (pid1 != 0){  /* parent */
            close(fd[0]);
            close(fd[1]);
            close(fdd[0]);
            close(fdd[1]);

            /*wait for children to finish*/
            waitpid(pid1, NULL, 0);
            waitpid(pid2, NULL, 0);
            waitpid(pid3, NULL, 0);
        }
    }else{
        printf("Usage: ./askisi2 infile\n");
    }
}