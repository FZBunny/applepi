
/*****************************************************************************

    File: "debugging_dumps.cpp"

    Copyright (C) 2003,2019 Bruce Ward

    This file is part of ApplePi, an Apple2 emulator.
    (Apple2 is a trademark of Apple, Inc.)

    "ApplePi" is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    "ApplePi" is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You may obtain a copy of the GNU General Public License by writing to:

    The Free Software Foundation, Inc.
    51 Franklin Street, Fifth Floor
    Boston, MA 02110-1335 USA

    Or simply visit: https://www.gnu.org/licenses/gpl-3.0.html

*****************************************************************************/


#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include "debugging_dumps.h"



uint8 hex2byte (char *p)
{
    static uint8 n[256] = {
	    0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,
	    0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,
	    0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,
	    0, 1, 2, 3,  4, 5, 6, 7,  8, 9, 0, 0,  0, 0, 0, 0,

	    0,10,11,12, 13,14,15, 0,  0, 0, 0, 0,  0, 0, 0, 0,
	    0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,
	    0,10,11,12, 13,14,15, 0,  0, 0, 0, 0,  0, 0, 0, 0,
	    0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,

	    0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,
	    0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,
	    0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,
	    0, 1, 2, 3,  4, 5, 6, 7,  8, 9, 0, 0,  0, 0, 0, 0,

	    0,10,11,12, 13,14,15, 0,  0, 0, 0, 0,  0, 0, 0, 0,
	    0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,
	    0,10,11,12, 13,14,15, 0,  0, 0, 0, 0,  0, 0, 0, 0,
	    0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0
	} ;


	return  (n[(int)*p]<<4) | (n[(int)*(p+1)]) ;
}



static char *hexchar = (char*)"0123456789abcdef" ;

uint8 *btohex (uint8 *p, uint8 c)
{
    *p++ = hexchar[(c>>4)&0x0f] ;
    *p++ = hexchar[c&0x0f] ;

    return p ;
}


uint8 *itohex (uint8 *p, int n)
{
    *p++ = hexchar[(n>>28)&0x0f] ;
    *p++ = hexchar[(n>>24)&0x0f] ;
    *p++ = hexchar[(n>>20)&0x0f] ;
    *p++ = hexchar[(n>>16)&0x0f] ;
    *p++ = hexchar[(n>>12)&0x0f] ;
    *p++ = hexchar[(n>>8)&0x0f] ;
    *p++ = hexchar[(n>>4)&0x0f] ;
    *p++ = hexchar[n&0x0f] ;

    return p ;
}



uint8 *stohex (uint8 *p, int n)
{
    *p++ = hexchar[(n>>12)&0x0f] ;
    *p++ = hexchar[(n>>8)&0x0f] ;
    *p++ = hexchar[(n>>4)&0x0f] ;
    *p++ = hexchar[n&0x0f] ;

    return p ;
}


void xdump (uint8 *data, int len, int fakeAddress)
{
    int      i, last ;
    int      line ;
    uint8   buffer[80] ;
    uint8   c ;
    uint8   *p ;

    for (line=0; line<len; line += 16) {
        memset (buffer, 0x20, 80) ;
        p = itohex (buffer, (int64)fakeAddress+line) ;
        *p++ = ' ' ;
        *p++ = ':' ;
        last = line + 16 ;
        if (last > len) last = len ;
        for (i=line; i<last; i++) {
            if ((i %4) == 0) *p++ = ' ' ;
            p = btohex (p, *(data+i)) ;
            *p++ = ' ' ;
        }
        p = buffer + 63 ;
        for  (i=line; i<last; i++) {
            c = *(data+i) & 0x7f ;
            if (c < 0x20)  c = '.' ;
            if (c == 0x7f) c = '.' ;
            *p++ = c ;
        }
        *p = 0 ;
	    puts ((char*)buffer) ;
    }

    fflush (stdout) ;
}


void formatDumpLine16  (uint8 *buffer, uint8 *data, int lineAddress)
{
    uint8   c ;
    memset (buffer, 0x20, 80) ;
    uint8* p = stohex (buffer, (int64)lineAddress) ;
    *p++ = ' ' ;
    *p++ = ':' ;
    for (int i=0; i<16; i++) {
        if ((i %4) == 0) *p++ = ' ' ;
        p = btohex (p, *(data+i)) ;
        *p++ = ' ' ;
    }

    for  (int i=0; i<16; i++) {
        if (i%4 == 0) *p++ = ' ' ;
        c = *(data+i) & 0x7f ;
        if (c < 0x20)  c = '.' ;
        if (c == 0x7f) c = '.' ;
        *p++ = c ;
    }
    *p = 0 ;
}

