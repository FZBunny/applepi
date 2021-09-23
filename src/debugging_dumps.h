/*****************************************************************************

    File: "debugging_dumps.h"

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



#ifndef DDUMPS_H
#define DDUMPS_H 1

typedef unsigned char      uint8 ;
typedef unsigned short     uint16 ;
typedef unsigned int       uint32 ;
typedef unsigned long long uint64 ;
typedef long long          int64 ;


uint8 hex2byte (char *p) ;

char *btohex (char *p, char c) ;
char *itohex (char *p, int n) ;
void formatDumpLine16 (uint8 *buffer, uint8 *data, int lineAddress) ;
void xdump (uint8 *data, int len, int fakeAddress) ;


#endif  /* DDUMPS_H */

