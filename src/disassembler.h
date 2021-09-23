
/*****************************************************************************

    File: "disassembler.h"

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


#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include <QTextStream>
#include <stdio.h>

#include "defs.h"



class Disassembler
{
public:

    Disassembler (QTextStream* out, quint8* mem, bool exitOnError) ;
    Disassembler (quint8* mem, bool exitOnError) ;

    void     setBaseAddress (quint16 address) ;
    void     disassemble6502 (quint16 start, quint16 end) ;
    quint16  print_6502_instruction  (quint8 opcode, quint8 n1, quint8 n2) ;
    quint16  sprint_6502_instruction (quint16 pc, quint8 opcode, quint8 c1, quint8 c2, char* buffer) ;

private:

    QTextStream* m_out ;
    char         m_buffer[128] ;
    bool         m_exitOnError ;
    quint8*      m_memory ;
    quint16      m_baseAddress ;
};



#endif // DISASSEMBLER_H
