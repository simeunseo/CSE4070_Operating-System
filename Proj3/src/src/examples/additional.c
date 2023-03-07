#include "userprog/syscall.h"
#include <stdio.h>

int
main (int argc, char *argv[]) 
{
    char* a = argv[1];
    char* b = argv[2];
    char* c = argv[3];
    char* d = argv[4];
    printf("%d %d\n",fibonacci(atoi(a)),max_of_four_int(atoi(a),atoi(b),atoi(c),atoi(d)));

    return EXIT_SUCCESS;
}