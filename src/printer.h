/*****************************************************************************

    File: "printer.h"

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


#ifndef PRINTER_H
#define PRINTER_H  1

#include <stdio.h>
#include <QtGlobal>
#include <QString>

#include "defs.h"
#include "machine.h"



class Printer
{
public:

    Printer (Machine* machine) ;
    ~Printer (void) {} ;

    int    open (QString& name) ;
    void   close (void) ;
    quint8 fetch_Printer_ROM (int slotNumber, quint8 loByte) ;

private:

    Machine* m_mac ;
    FILE*    m_out ;

    // 'epson_ROM_fragment' exists only to let ProDOS identify slot 1 as a parallel (printer) card.
    const quint8 epson_ROM_fragment[0x50] = { 
        0x18, 0xb0, 0x38, 0x48, 0x8a, 0x48, 0x98, 0x48, 0xa5, 0x00, 0x48, 0xa5, 0x01, 0x48, 0xba, 0xe8,
        0xe8, 0xe8, 0xe8, 0xe8, 0xbd, 0x00, 0x01, 0x48, 0x20, 0x58, 0xff, 0xba, 0xbd, 0x00, 0x01, 0x29,
        0x0f, 0xaa, 0xb0, 0x2b, 0xa9, 0x89, 0x9d, 0x78, 0x05, 0xa9, 0x00, 0x9d, 0xf8, 0x05, 0x8d, 0x78,
        0x05, 0x9d, 0xf8, 0x04, 0x9d, 0xf8, 0x06, 0xa5, 0x24, 0x9d, 0xf8, 0x07, 0xa5, 0x21, 0x9d, 0x78,
        0x06, 0xa9, 0x01, 0x9d, 0x78, 0x07, 0xa9, 0x4b, 0x9d, 0x78, 0x04, 0xa9, 0x02, 0x85, 0x36, 0x68  
    } ;
} ;

#endif
