/*****************************************************************************

    File: "debugging_dumps.h"

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

