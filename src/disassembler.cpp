/*****************************************************************************

    File: "disassembler.cpp"

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



#include <stdio.h>
#include <string.h>

#ifndef __WXMSW__
#  include <unistd.h>
#endif


#include "defs.h"
#include "disassembler.h"
#include "debugging_dumps.h"


Disassembler::Disassembler (QTextStream* out, quint8* mem, bool exitOnError)
{
    m_baseAddress = 0 ;
    m_memory = mem ;
    m_out    = out ;
    m_exitOnError = exitOnError ;
}


Disassembler::Disassembler (quint8* mem, bool exitOnError)
{
    m_baseAddress = 0 ;
    m_memory = mem ;
    m_out    = new QTextStream (stdout) ;
    m_exitOnError = exitOnError ;
}


void Disassembler::disassemble6502 (quint16 start, quint16 end)
{
    quint16  p = start ;
    quint8   opcode, c1, c2 ;

    while (p <= end) {
        opcode = m_memory[p] ;
        c1 = m_memory[p+1] ;
        c2 = m_memory[p+2] ;
        p = sprint_6502_instruction (p, opcode, c1, c2, m_buffer) ;
        printf ("%s\n", m_buffer) ;
        if ((opcode==0x40) || (opcode==0x60)) putchar ('\n') ;  // Put a blank line after RTS of JMP opcodes
    }
}


void Disassembler::setBaseAddress (quint16 address)
{
    m_baseAddress = address ;
}


quint16 Disassembler::sprint_6502_instruction (quint16 pc, quint8 opcode, quint8 c1, quint8 c2, char* buffer)
{
    quint16  offset ;

    enum modenames {
        SI, // single byte instruction
        In, // indirect (JMP only)   (Must have lower-case 'n' because *Microsoft*.)
        IM, // immediate
        ZP, // zero page
        AB, // absolute
        IZ, // (indirect, zero page)
        IY, // (indirect, zero page),Y
        IX, // (indirect, zero page, X)
        IA, // (indirect, absolute, X) (JMP only)
        AY, // absolute,Y
        AX, // absolute,X
        ZY, // zero page,Y
        ZX, // zero page,X
        RE,  // relative (branches)
        BB  // branch-on-bit   (BBS & BBR)
    } ;

    static const char* mnemonic[256] =
    { //    0      1      2      3      4      5      6      7      8      9      A      B      C      D      E      F
/* 0 */   "BRK ","ORA "," ?  "," ?  ","TSB ","ORA ","ASL ","RMB0","PHP ","ORA ","ASL "," ?  ","TSB ","ORA ","ASL ","BBR0",
/* 1 */   "BPL ","ORA ","ORA "," ?  ","TRB ","ORA ","ASL ","RMB1","CLC ","ORA ","INC "," ?  ","TRB ","ORA ","ASL ","BBR1",
/* 2 */   "JSR ","AND "," ?  "," ?  ","BIT ","AND ","ROL ","RMB2","PLP ","AND ","ROL "," ?  ","BIT ","AND ","ROL ","BBR2",
/* 3 */   "BMI ","AND ","AND "," ?  ","BIT ","AND ","ROL ","RMB3","SEC ","AND ","DEC "," ?  ","BIT ","AND ","ROL ","BBR3",
/* 4 */   "RTI ","EOR "," ?  "," ?  "," ?  ","EOR ","LSR ","RMB4","PHA ","EOR ","LSR "," ?  ","JMP ","EOR ","LSR ","BBR4",
/* 5 */   "BVC ","EOR ","EOR "," ?  "," ?  ","EOR ","LSR ","RMB5","CLI ","EOR ","PHY "," ?  "," ?  ","EOR ","LSR ","BBR5",
/* 6 */   "RTS ","ADC "," ?  "," ?  ","STZ ","ADC ","ROR ","RMB6","PLA ","ADC ","ROR "," ?  ","JMP ","ADC ","ROR ","BBR6",
/* 7 */   "BVS ","ADC ","ADC "," ?  ","STZ ","ADC ","ROR ","RMB7","SEI ","ADC ","PLY "," ?  ","JMP ","ADC ","ROR ","BBR7",
/* 8 */   "BRA ","STA "," ?  "," ?  ","STY ","STA ","STX ","SMB0","DEY ","BIT ","TXA "," ?  ","STY ","STA ","STX ","BBS0",
/* 9 */   "BCC ","STA ","STA "," ?  ","STY ","STA ","STX ","SMB1","TYA ","STA ","TXS "," ?  ","STZ ","STA ","STZ ","BBS1",
/* A */   "LDY ","LDA ","LDX "," ?  ","LDY ","LDA ","LDX ","SMB2","TAY ","LDA ","TAX "," ?  ","LDY ","LDA ","LDX ","BBS2",
/* B */   "BCS ","LDA ","LDA "," ?  ","LDY ","LDA ","LDX ","SMB3","CLV ","LDA ","TSX "," ?  ","LDY ","LDA ","LDX ","BBS3",
/* C */   "CPY ","CMP "," ?  "," ?  ","CPY ","CMP ","DEC ","SMB4","INY ","CMP ","DEX ","WAI ","CPY ","CMP ","DEC ","BBS4",
/* D */   "BNE ","CMP ","CMP "," ?  "," ?  ","CMP ","DEC ","SMB5","CLD ","CMP ","PHX ","STP "," ?  ","CMP ","DEC ","BBS5",
/* E */   "CPX ","SBC "," ?  "," ?  ","CPX ","SBC ","INC ","SMB6","INX ","SBC ","NOP "," ?  ","CPX ","SBC ","INC ","BBS6",
/* F */   "BEQ ","SBC ","SBC "," ?  "," ?  ","SBC ","INC ","SMB7","SED ","SBC ","PLX "," ?  "," ?  ","SBC ","INC ","BBS7"
    } ;

    static char modes[256] =
    {// 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
/* 0 */ SI,IX,SI,SI,ZP,ZP,ZP,ZP,SI,IM,SI,SI,AB,AB,AB,BB,
/* 1 */ RE,IY,IZ,SI,ZP,ZX,ZX,ZP,SI,AY,SI,SI,AB,AX,AX,BB,
/* 2 */ AB,IX,SI,SI,ZP,ZP,ZP,ZP,SI,IM,SI,SI,AB,AB,AB,BB,
/* 3 */ RE,IY,IZ,SI,ZX,ZX,ZX,ZP,SI,AY,SI,SI,AX,AX,AX,BB,
/* 4 */ SI,IX,SI,SI,SI,ZP,ZP,ZP,SI,IM,SI,SI,AB,AB,AB,BB,
/* 5 */ RE,IY,IZ,SI,SI,ZX,ZX,ZP,SI,AY,SI,SI,SI,AX,AX,BB,
/* 6 */ SI,IX,SI,SI,ZP,ZP,ZP,ZP,SI,IM,SI,SI,In,AB,AB,BB,
/* 7 */ RE,IY,IZ,SI,ZX,ZX,ZX,ZP,SI,AY,SI,SI,IA,AX,AX,BB,
/* 8 */ RE,IX,SI,SI,ZP,ZP,ZP,ZP,SI,IM,SI,SI,AB,AB,AB,BB,
/* 9 */ RE,IY,IZ,SI,ZX,ZX,ZY,ZP,SI,AY,SI,SI,AB,AX,AX,BB,
/* A */ IM,IX,IM,SI,ZP,ZP,ZP,ZP,SI,IM,SI,SI,AB,AB,AB,BB,
/* B */ RE,IY,IZ,SI,ZX,ZX,ZY,ZP,SI,AY,SI,SI,AX,AX,AY,BB,
/* C */ IM,IX,SI,SI,ZP,ZP,ZP,ZP,SI,IM,SI,SI,AB,AB,AB,BB,
/* D */ RE,IY,IZ,SI,SI,ZX,ZX,ZP,SI,AY,SI,SI,SI,AX,AX,BB,
/* E */ IM,IX,SI,SI,ZP,ZP,ZP,ZP,SI,IM,SI,SI,AB,AB,AB,BB,
/* F */ RE,IY,IZ,SI,SI,ZX,ZX,ZP,SI,AY,SI,SI,SI,AX,AX,BB
    } ;

    quint8  mode = modes[opcode] ;

    quint16 displayAddress = m_baseAddress + pc ;

    switch (mode) {
        case SI:
            sprintf (buffer, "%4.4X  %2.2X         %s             ",
                     displayAddress, opcode, mnemonic[opcode]) ;
            pc += 1 ;
            break ;
        case In:
            sprintf (buffer, "%4.4X  %2.2X %2.2X %2.2X   %s ($%2.2X%2.2X)     ",
                     displayAddress, opcode, c1, c2, mnemonic[opcode], c2, c1) ;
            pc += 3 ;
            break ;
        case IM:
            sprintf (buffer, "%4.4X  %2.2X %2.2X      %s #$%2.2X        ",
                     displayAddress, opcode, c1, mnemonic[opcode], c1) ;
            pc += 2 ;
            break ;
        case ZP:
            sprintf (buffer, "%4.4X  %2.2X %2.2X      %s $%2.2X         ",
                     displayAddress, opcode, c1, mnemonic[opcode], c1) ;
            pc += 2 ;
            break ;
        case AB:
            sprintf (buffer, "%4.4X  %2.2X %2.2X %2.2X   %s $%2.2X%2.2X       ",
                    displayAddress, opcode, c1, c2, mnemonic[opcode], c2, c1) ;
            pc += 3 ;
            break ;
        case IZ:
            sprintf (buffer, "%4.4X  %2.2X %2.2X      %s ($%2.2X)       ",
                     displayAddress, opcode, c1, mnemonic[opcode], c1) ;
            pc += 2 ;
            break ;
        case IY:
            sprintf (buffer, "%4.4X  %2.2X %2.2X      %s ($%2.2X),Y     ",
                     displayAddress, opcode, c1, mnemonic[opcode], c1) ;
            pc += 2 ;
            break ;
        case IX:
            sprintf (buffer, "%4.4X  %2.2X %2.2X      %s ($%2.2X,X)     ",
                     displayAddress, opcode, c1, mnemonic[opcode], c1) ;
            pc += 2 ;
            break ;
        case IA:
            sprintf (buffer, "%4.4X  %2.2X %2.2X %2.2X   %s ($%2.2X%2.2X,X)   ",
                     displayAddress, opcode, c1, c2, mnemonic[opcode], c2, c1) ;
            pc += 2 ;
            break ;
        case AY:
            sprintf (buffer, "%4.4X  %2.2X %2.2X %2.2X   %s $%2.2X%2.2X,Y     ",
                   displayAddress, opcode, c1, c2, mnemonic[opcode], c2, c1) ;
            pc += 3 ;
            break ;
        case AX:
            sprintf (buffer, "%4.4X  %2.2X %2.2X %2.2X   %s $%2.2X%2.2X,X     ",
                   displayAddress, opcode, c1, c2, mnemonic[opcode], c2, c1) ;
            pc += 3 ;
            break ;
        case ZY:
            sprintf (buffer, "%4.4X  %2.2X %2.2X      %s $%2.2X,Y       ",
                     displayAddress, opcode, c1, mnemonic[opcode], c1) ;
            pc += 2 ;
            break ;
        case ZX:
            sprintf (buffer, "%4.4X  %2.2X %2.2X      %s $%2.2X,X       ",
                     displayAddress, opcode, c1, mnemonic[opcode], c1) ;
            pc += 2 ;
            break ;
        case RE:
            offset = c1 ;
            if (offset & 0x80) offset |= 0xff00 ;
            offset += pc + 2 ;
            sprintf (buffer, "%4.4X  %2.2X %2.2X      %s $%4.4X       ",
                    displayAddress, opcode, c1, mnemonic[opcode], offset) ;
            pc += 2 ;
            break ;
        case BB:
            offset = c2 ;
            if (offset & 0x80) offset |= 0xff00 ;
            offset += pc + 3 ;
            sprintf (buffer, "%4.4X  %2.2X %2.2X %2.2X   %s $%2.2X $%4.4X   ",
                     displayAddress, opcode, c1, c2, mnemonic[opcode], c1, offset) ;
            pc += 3 ;
            break ;
        default:
            sprintf (buffer, "*** internal error in 'sprint_6502_instruction' ***") ;
            pc += 1 ;
            break ;
    }

 //   puts (buffer) ;

    return pc ;
}





