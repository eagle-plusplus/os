#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "libs.h"

//Συνάρτηση για αντιγραφή με buffer ορισμένο από τον χρήστη
void descriptors_cp(char **argv){
    int bytes;
    char option[50] = "";

    strncpy(option, argv[1], 50);

    //Έλεγχος για το εαν η επιλογή -b'buffer_size' είνα σωστά ορισμένη 
    if (option[0] == '-' && option[1] == 'b'){
        option[0] = '0';
        option[1] = '0';
        bytes = atoi(option);
        if (bytes <= 0){
            printf("Invalid buffer size");
            exit(13);
        }
    }else{
        printf("Invalid option");
        exit(14);
    }

    //Άνοιγμα και έλεγχος για σφάλμα του φακέλου ανάγνωσης 
    int source_fd;
    if ((source_fd = open(argv[2], O_RDONLY)) < 0) {
        perror("Open source failed");
        exit(2);
    }
    
    //Άνοιγμα και έλεγχος για σφάλμα του φακέλου εγγραφής
    int dest_fd;
    if ((dest_fd = open(argv[3], O_WRONLY | O_CREAT, 0600)) < 0) {
        perror("Destination open failed");
        int rc;
        if ((rc = close(source_fd)) < 0) {
            perror("close");
            exit(10);
        }
        exit(3);
    }

    /*Διατηρούμε το ρεύμα εξόδου με file descriptor = 1 ώστε να το 
    επαναφέρουμε πιο κάτω. Aλλάζουμε το STDOUT με τον φάκελο έγγραφης 
    που άνοιξε παραπάνω και πλέον έχει αυτός file descriptor = 1. */
    int saved_dup = dup(1);
    dup2(dest_fd, STDOUT_FILENO);   

    char buf[bytes];
    int chars_read;

    //Έναρξη χρονομέτρησης
    int before = clock();
    
    //Εκκίνηση ανάγνωσης από τον πρώτο φάκελο και εγγραφής στον δεύτερο.
    chars_read = read(source_fd, buf, sizeof(buf));
    while (chars_read > 0) {
        //Έλεγχος για σφάλμα κατά την εγγραφή.
        if (write(STDOUT_FILENO, buf, chars_read) < chars_read){
            perror("Write problems:");
            exit(4);
        }
        chars_read = read(source_fd, buf, sizeof(buf));
    }

    //Τερματισμός χρονομέτρησης
    int after = clock();

    //Έλεγχος για σφάλμα κατά την ανάγνωση
    if (chars_read < 0) {
        perror("Reading error:");
        exit(5);
    }

    //Επαναφορά file descriptor = 1 στο STDOUT
    dup2(saved_dup, STDOUT_FILENO);

    //Κλείσιμο φακέλων
    close(source_fd);
    close(dest_fd);

    //Υπολογισμός και εκτύπωση χρόνου ανάμονής για την αντιγραφή
    double elapsed = (double)(after-before)/CLOCKS_PER_SEC;
    printf("Time elapsed :%lf seconds for buffer size %d \n", elapsed, bytes);
}
