
// gcc -o create-printer-pins-xpm   create-printer-pins-xpm.cpp


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <getopt.h>
#include <string.h>


int main (int argc, char* argv[])
{
  char* header =  (char*)
                    "const char*  xpm_printerPins[] =\n"
                    "{\n"
                    "    1 2048 2 1\n"
                    "    X c #ffffff\n"
                    "    . c #000000\n\n" ;

    if (argc < 2) {
        fprintf (stderr, "\n* We need a name for the output file.\n\n") ;
        exit (1) ;
    }

    FILE* out = fopen (argv[1], "w") ;
    if (out < 0) {
        fprintf (stderr, "Can't open file %s for writing: %s\n", argv[1], strerror(errno)) ;
        exit (1) ;
    }

    fprintf (out, "%s", header) ;

    for (int pinPattern=0; pinPattern<256; pinPattern++) {
        for (int mask=0x80; mask&0xff; mask=mask>>1) {
            unsigned char c = '.' ;
            if (pinPattern & mask) c = 'X' ;
            fprintf (out, "    \"%c\"", c) ;
            if (pinPattern != 255) fputc (',', out) ;
            else    if (mask != 1) fputc (',', out) ;
            if (mask == 0x80) fprintf (out, "  // %2.2x",pinPattern) ;
            fputc ('\n', out) ;
        }
        fputc ('\n', out) ;
    }

    fprintf (out, "} ;\n\n") ;
    fclose (out) ;
}

