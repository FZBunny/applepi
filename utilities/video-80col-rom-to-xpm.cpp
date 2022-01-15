/*****************************************************************************

    File: "video-80col-rom-to-xpm.cpp"

    Copyright (C) 2021, Bruce Ward

    This file is part of applepi, an apple2 emulator for Linux,
    especially aimed at the Raspberry Pi operating system.

    applepi is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    applepi is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with applepi; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Or perhaps visit: http://www.gnu.org/licenses/licenses.html

*****************************************************************************/


/****************************************************

  Read an 80-column Apple II video charcter ROM, and
  create a '.xpm' representation of the data.

****************************************************/


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <getopt.h>
#include <string.h>


void usage (void)
{
    fprintf (stderr, "\nUsage:\n") ;
    fprintf (stderr, "   video-80col-rom-to-xpm [-r] [-i] <filename>\n") ;
    fprintf (stderr, "\nWhere:\n") ;
    fprintf (stderr, "   name   The name of the video ROM image file;\n") ;
    fprintf (stderr, "          the output XPM file will be written to stdout.\n") ;
    fprintf (stderr, "   -r     Reverse the bit-order of each scan line in the file. \n") ;
    fprintf (stderr, "   -i     Invert the video signal produced. (light becomes dark, etc.)\n") ;
    fprintf (stderr, "   \n") ;
    exit (1) ;
}


bool reverse = false ;
bool invert = false ;

void write1LineOfBits (u_char c, int charNo, char* line)
{
    u_char  bitsDown[8] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 } ;
    u_char  bitsUp[8]   = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 } ;
    u_char* bits ;

    char* light ;
    char* dark ;

    if (reverse) bits = bitsDown ;
    else         bits = bitsUp ;

    if (invert) {
        light = (char*)"-" ;
        dark  = (char*)"X" ;
    } else {
        light = (char*)"X" ;
        dark  = (char*)"-" ;
    }

    for (int bit=1; bit<8; bit++) {
        if (charNo < 0x80) {
            if (c & bits[bit]) strcat (line, dark) ;
            else               strcat (line, light) ;
        } else {
            if (c & bits[bit]) strcat (line, light) ;
            else               strcat (line, dark) ;
        }
    }
}


void write80ColChar (u_char* rom, int charNo)
{
    char line[80] ;
    if (charNo == 0x80) invert = ! invert ;
//fprintf (stderr, "charNo=%2.2x  invert=%i\n", charNo, invert) ;
    for (int outputLine=0; outputLine<8; outputLine++) {
        line[0] = 0 ;
        strcat (line, "    \"") ;
        u_char c = rom[outputLine+(charNo*8)] ;
        write1LineOfBits (c, charNo, line) ;
        if (invert) {
            strcat (line, "-\",") ;
        } else {
            strcat (line, "X\",") ;
        }
        if (outputLine == 0) {
            char charNoString[16] ;
            sprintf (charNoString, "    // %2.2X", charNo) ;
            strcat (line, charNoString) ;
        }

        puts (line) ;
        puts ("    \"----------------\",") ;
    }
    puts ("\n") ;
}


int main (int argc, char* argv[])
{
    int opt ;

    if (argc < 2) usage() ;

    while ((opt = getopt(argc, argv, "ir")) != -1) {
        switch (opt) {
            case 'i':
                invert = true ;
                break ;
            case 'r':
                reverse = true ;
                break;
            default:
                usage() ;
                break ;
        }
    }
    char* filename = argv[optind] ;

    FILE* fIn = fopen (filename, "r") ;
    if (fIn == NULL) {
        fprintf (stderr, "\n*** Can't open file \"%s\": %s\n\n", filename, strerror(errno)) ;
        exit (1) ;
    }

    fseek (fIn, 0, SEEK_END) ;
    int romLen = ftell (fIn) ;
    rewind (fIn) ;
    u_char* rom = (u_char*)malloc (romLen) ;
    int n = fread (rom, 1, romLen, fIn) ;
    if (n != romLen) {
        fprintf (stderr, "*** Error reading %s: %s\n\n", filename, strerror(errno)) ;
        exit(1) ;
    }
    fclose (fIn) ;

// --------------------------------------------

    puts   ("const char* xpm_80ColChars[] =") ;
    puts   ("{") ;
    printf ("    \"8 %i 2 1\",\n", romLen*2) ;
    puts   ("    \"X c #ffffff\",") ;
    puts   ("    \"- c #000000\",") ;
    puts   ("") ;

    int nChars = romLen/8 ;
    for (int charNo=0; charNo<nChars; charNo++) {
        write80ColChar (rom, charNo) ;
    }

    puts ("} ;") ;

}

