#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <fcntl.h>

//Συνάρτηση εκτύπωσης
void childPrint(char **argv, int num, int x) {
    for(int i = 0; i < num; i++){
    //Άνοιγμα και έλεγχος για σφάλμα του φακέλου ανάγνωσης 
    int source_fd;
    if ((source_fd = open(argv[x], O_RDONLY)) < 0) {
        perror("Open source failed");
        exit(2);
    }

    char buf[128];
    int chars_read; 

    //Εκκίνηση ανάγνωσης από τον πρώτο φάκελο και εγγραφής στο τερματικό.
    chars_read = read(source_fd, buf, sizeof(buf));
    while (chars_read > 0) {
        //Έλεγχος για σφάλμα κατά την εγγραφή.
        if (write(STDOUT_FILENO, buf, chars_read) < chars_read){
            perror("Write problems:");
            exit(4);
        }
        chars_read = read(source_fd, buf, sizeof(buf));
    }
    sleep(4);
    }
}

int main(int argc, char **argv) {
    /*Εαν ο χρήστης έχει δώσει λάθος αριθμό ορισμάτων τότε απλώς εκτυπώνετε
    η σωστή χρήση της έντολής*/
    if(argc = 5){
        //Έλεγχος για το εαν η επιλογή του αριθμού επαναλήψεων είναι σωστά ορισμένη
        int times;
        times = atoi(argv[1]);
        if (times <= 0){
            printf("Invalid data type. Yoy must use unsigned INT");
            exit(13);
        }
        
        int Num = times;

        int Numb = Num - 1;
        int Numc = Num - 2;

        int pid1;
        if ( (pid1 = fork()) < 0){ /*child 1*/
            perror("fork error : ");
            return 1;
        }
        if (pid1 == 0) {
            childPrint(argv, Num, 2);
            
        } else {
            int pid2;
            if ( (pid2 = fork()) < 0){
                perror("fork error : ");
                return 1;
            }
            if (pid2 == 0) { /*child 2*/
                
                childPrint(argv, Numb, 3);
                
            } else {
                int pid3;
                if ( (pid3 = fork()) < 0){
                    perror("fork error : ");
                    return 1;
                }
                if (pid3 == 0) { /*child 3*/
                    
                    childPrint(argv, Numc, 4);
                } else {
                   
                    waitpid(pid1, NULL, 0);  /*wait for children to finish*/
                    waitpid(pid1, NULL, 0);
                    waitpid(pid3, NULL, 0);
                }
            }
        }
        return 0;

    }else{
        printf("Usage: ./askisi4 maxprint infile1 infile3 infile3\n");
        return 1;
    }
}