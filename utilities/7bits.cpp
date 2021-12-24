
/*   This does nothing more than take characters from stdin, strip  */
/*   the uppermost bit, and write the remaining 7 bits to stdout.   */

#include <stdio.h>
#include <stdint.h>
#include <errno.h>

int main (int argc, char* argv[])
{
    int c ;
    errno = 0 ;

    while (errno == 0) {
        c = getchar() ;
        if (c == EOF) break ;
        c &= 0x7f ;
        putchar (c) ;
    }
}

