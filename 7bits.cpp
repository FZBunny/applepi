
#include <stdio.h>
#include <stdint.h>
#include <errno.h>

int main (int argc, char* argv[])
{
    int c ;
 
    while (errno == 0) {
        c = getchar() ;
        if (c == EOF) break ;
        c &= 0x7f ;
        putchar (c) ;
    }
}

