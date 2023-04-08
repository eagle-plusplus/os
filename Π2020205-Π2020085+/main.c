#include <stdio.h>
#include <stdlib.h>
#include "libs.h"

int main(int argc, char **argv) {

    /*Εαν ο χρήστης έχει δώσει ένα ή δύο ορίσματα τότε απλώς εκτυπώνετε
    η σωστή χρήση της έντολής*/
    if (argc <= 2) {
        printf("Usage: mycopy source_file [destination_file]\n");
        printf("Use -b option to choose size of buffer\n");
        exit(1);
    }

    if (argc == 4){
        //Καλείτε η συνάρτηση για αντιγραφή με buffer ορισμένο από τον χρήστη
        descriptors_cp(argv);
    }else{
        //Καλείτε η συνάρτηση για αντιγραφή που χρησιμοποιεί standard buffered I/O
        pointers_cp(argv);
    }
return 0;
}
