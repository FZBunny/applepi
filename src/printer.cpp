/*****************************************************************************

    File: "printer.cpp"

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

#include <QtGlobal>
#include <stdio.h>

#include "defs.h"
#include "machine.h"
#include "printer.h"




Printer::Printer (Machine* mac)
{
    m_parent = mac ;
    m_out = new QFile() ;
    m_out->open (1, QIODevice::WriteOnly) ;
    m_writingToFile = false ;
}


bool Printer::open (QString path)
{
    if (m_out) delete m_out ;
    m_out = new QFile (path) ;
    bool ok = m_out->open (QIODevice::WriteOnly) ;
    if (ok) m_writingToFile = true ;
    return ok ;
}


QString Printer::error()
{
    return m_out->errorString() ;
}

void Printer::close (void)
{
    m_out->close() ;
    delete m_out ;
    m_out = new QFile() ;
    m_out->open (1, QIODevice::WriteOnly) ;
    m_writingToFile = false ;
    MAC->m_ram[0x36] = 0xf0 ;  // restore the char. output vector to COUT1 ($FDF0)
    MAC->m_ram[0x37] = 0xfd ;
}


bool Printer::haveOpenFile (void)
{
    return m_writingToFile ;
}


//
//  This is called by Machine::fetch when fetching a byte from our fictitious printer ROM.
//  This usually occurs when fetching 3 butes for a JSR $C100 (When a "PR#1" is entered),
//  or 3 bytes when a JSR $C110 is executed to print a single character.
//  ProDOS may also fetch single bytes when attempting to identify the device in slot 1. 

quint8 Printer::fetch_Printer_ROM (int slotNumber, quint8 p)
{                                      
    static const quint8 RTS = 0x60 ;
    
    quint16 calledFrom = MAC->savedPC() ;
    quint16 slotAddr = 0xc000 | (slotNumber << 8) ;
    quint16 entryPoint = slotAddr + 0x10 ;
    ProcessorState* ps = MAC->processorState() ;
    quint8 c ;

    // Ignore 2 fetches after $Cn00 or $Cn10 entry; just a side-effect
    // of last few lines of fn. "Machine::run" saving history data.
    if ((calledFrom==slotAddr)   && ((p==0x01) || (p==0x02))) return 0 ;
    if ((calledFrom==entryPoint) && ((p==0x11) || (p==0x12))) return 0 ;

    if ((p==0) && (calledFrom==slotAddr)) {                        // Did someone say "PR#1" ?
        MAC->m_ram[0x36] = 0x10 ;           // Set the character output vector
        MAC->m_ram[0x37] = slotAddr >> 8 ;  // to point to us
        ps->Areg = 0 ;
        ps->Pstat &= C ^ 0xff ;
        c = RTS ;
    } else if ((p==0x10) && (calledFrom==entryPoint)) {            // Print a single character.
//printf (" %2.2x:", Areg) ;
        m_out->putChar (ps->Areg&0x7f) ;
        if ((ps->Areg&0x7f) == '\r') m_out->putChar ('\n') ;
        m_out->flush() ;
        if (MAC->m_ss[0] == 0x83) {    // If a ctrl-C was entered at the keyboard,
            MAC->m_ram[0x36] = 0xf0 ;  // restore the char. output vector to COUT1 ($FDF0),
            MAC->m_ram[0x37] = 0xfd ;  // and close the output file.
            close() ;
        }
        c = RTS ;
    } else {                                                       // PC isn't Cn00 or Cn10; just return a byte from ROM.
        if (p > 0x4f) p = 0 ;
        c = epson_ROM_fragment[p] ;
    }

    return c ;
}

