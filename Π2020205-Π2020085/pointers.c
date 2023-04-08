#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "libs.h"

//Συνάρτηση για αντιγραφή που χρησιμοποιεί standard buffered I/O
void pointers_cp(char **argv){

    //Άνοιγμα και έλεγχος για σφάλμα του φακέλου ανάγνωσης 
    FILE *source_fp = fopen(argv[1], "rb");
    if (source_fp == NULL){
        perror("Open source failed");
        exit(2);
    }

    //Άνοιγμα και έλεγχος για σφάλμα του φακέλου εγγραφής
    FILE *dest_fp = fopen(argv[2], "wb");
    if(dest_fp == NULL){
        fclose(source_fp);
        perror("Destination open failed");
        exit(3);
    }

    //Έναρξη χρονομέτρησης
    int before = clock();

    //Εκκίνηση ανάγνωσης από τον πρώτο φάκελο και εγγραφής στον δεύτερο.
    char c;
    do{
        //Διάβασμα έναν χαρακτήρα την φορά
        c = fgetc(source_fp);
        //Έλεγχος για σφάλμα κατά την ανάγνωση
        if (ferror(source_fp) != 0){
            perror("Reading error:");
            exit(5);
        }
        //Έλεγχος για το τέλος του αρχείου
        if (feof(source_fp)){
            break ;
        }
        fputc(c, dest_fp);
        //Έλεγχος για σφάλμα κατά την εγγραφή
        if (ferror(dest_fp) != 0){
            perror("Write problems:");
            exit(4);
        }
    }  while(1);

    //Τερματισμός χρονομέτρησης
    int after = clock();

    //Κλείσιμο φακέλων
    fclose(source_fp);
    fclose(dest_fp);

    //Υπολογισμός και εκτύπωση χρόνου ανάμονής για την αντιγραφή
    double elapsed = (double)(after-before)/CLOCKS_PER_SEC;
    printf("Time elapsed :%lf seconds with Standard buffered I/O \n", elapsed);   
}
