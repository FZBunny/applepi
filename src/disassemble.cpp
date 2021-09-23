/*****************************************************************************

    File: "disassemble6502.cpp"

    Copyright (C) 2003,2019 Bruce Ward

    This file is part of ApplePi, the Apple2 emulator for the Raspberry Pi.

    ApplePi is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    ApplePi is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ApplePi; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Or perhaps visit: http://www.gnu.org/licenses/licenses.html

*****************************************************************************/

//
// disassemble a rom containing 6502 instructions.
//
//    ./disassemble6502 romimagefile  startaddress
//


#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#ifndef __WXMSW__
  #include <unistd.h>
  #include <getopt.h>
#endif
#include <fcntl.h>
#include <errno.h>

#include <QTextStream>
#include <QFile>
#include <QFileInfo>

#include "defs.h"
#include "disassembler.h"



void usage(void)
{
    printf ("\nUsage:\n") ;
    printf ("   disassemble [-a address] [-s n] [-l length] filename\n") ;
    printf ("Where:\n") ;
    printf ("  -a address   Use this as the address of the first disassembled byte.\n") ;
    printf ("  -s n         Skip this many bytes before beginning disassembly.\n") ;
    printf ("  -l length    Disassemble this many bytes.\n\n") ;
}


void disassemble (quint8* memory, int startAddress, int endAddress)  // XXXXXXXXXXXXXXXXXXXX  RECODE!  CONFUSED LOGIC! XXXXXXXXXXXXXXXXXXXXXXXXX
{
    Disassembler* dis = new Disassembler (memory, false) ;
    char buffer[80] ;

   // int p = startAddress ;
    int p = 0 ;
    int lastP = endAddress - startAddress + 1 ;

    while (p <= lastP) {
        quint8 opcode = memory[p] ;
        quint8 c1 = memory[p+1] ;
        quint8 c2 = memory[p+2] ;
        p = dis->sprint_6502_instruction (startAddress+p, opcode, c1, c2, buffer) ;
        if (p == 0) break ;
        p -= startAddress ;
 //       qStdOut() << buffer << endl ;
        printf ("%s\n", buffer) ;
        if ((opcode==0x40) || (opcode==0x60)) printf ("\n") ;
    }

}


int main (int argc, char *argv[])
{
    if (argc <2) {
        usage() ;
        exit(1) ;
    }

    bool    badOption = false ;
    int     opt ;
    quint32 startAddress = 0 ;
    int     skip = 0 ;
    int     cmdLen = 0 ;

    while (true) {
        opt = getopt(argc, argv, "a:l:s:") ;
        if (opt == -1) break ;
        switch (opt) {
            case 'a':
               startAddress = strtoul (optarg, NULL, 0) ;
               break ;
            case 'l':
               cmdLen = strtoul (optarg, NULL, 0) ;
               break ;
            case 's':
               skip = strtoul (optarg, NULL, 0) ;
               break ;
            default:
               badOption = true ;
               break ;
        }
    }

    if (badOption) {
        usage() ;
        exit(1) ;
    }

    char* name = argv[optind] ;
    if (name == NULL) {
        printf ("\n** missing filename\n") ;
        usage() ;
        exit(1) ;
    }

// -----------------------------------------------------

    QString fileName (name) ;

    QFile file (fileName) ;
    if (file.isOpen() == false) exit(1) ;
    QFileInfo fInfo (fileName) ;
    int fileLength = fInfo.size() ;

    const int BUFFERSIZE = 0x10000 ;        // (This is all that a 16-bit address is good for...)
    quint8 buffer[BUFFERSIZE] ;

    int disassembleLen = BUFFERSIZE ;
    if (cmdLen && (disassembleLen > cmdLen)) disassembleLen = cmdLen ;
    if (disassembleLen > (fileLength - skip)) disassembleLen = fileLength - skip ;
    uint endAddress = startAddress + disassembleLen - 1 ;

    if (skip) file.seek (skip) ;

    int readLen = fileLength - skip ;
    if (readLen > BUFFERSIZE) readLen = BUFFERSIZE ;

    int n = file.read ((char*)buffer, readLen) ;
    if (n == -1) {
        printf ("Error: Can't read file \"%s\": %s\n", name, strerror(errno)) ;
        usage() ;
        exit(1) ;
    }
    file.close() ;

    disassemble (buffer, startAddress, endAddress) ;

    exit(0);

}
