
/*****************************************************************************

    File: "store.cpp"

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


#include "machine.h"
#include "floppy_controller.h"

/*
#ifdef __WXMSW__
#  include "win-speaker.h"
#else
#  include "speaker.h"
#endif
*/
#define DBUG(bits) if(m_debugFlags & bits)
#define PBUG(bits) if(m_debugFlags & bits)printf


// See "AppleIIe Technical Reference Manual" for information on soft switches (C000..C0FF) & memory management
// Also "Inside The Apple//e" by Gary B. Little  ISBN 0-89303-551-3
// and "Understanding the Apple IIe" by Jim Sather ISBN0-a351-fl0n-7

//  ---------------------------------------------------------------------
//
//                    S t o r e     F u n c t i o n s
//
//  ---------------------------------------------------------------------



void Machine::store (quint8 c, quint16 p)
{

// m_rom[p] = c ;   // =============== Uncomment to run CPU-test ROMS ====================
// return ;         // =============== Uncomment to run CPU-test ROMS ====================
// Go to  https://github.com/Klaus2m5/6502_65C02_functional_tests/blob/master/bin_files/65C02_extended_opcodes_test.lst
// for test-rom image.

    if (p < 0x200) {                  // 0000 - 01ff   page 0 & stack
        if (RdALTZP) m_aux[p] = c ;
        else         m_ram[p] = c ;
        return ;
    }

    if (p < 0xc000) {                 // 0400 - bfff   Lower 48k RAM (excluding pages 0 & 1)
        *lower48k (p, true) = c ;     // ('lower48k' is in machine.cpp; shared by fetch.cpp & store.cpp)
        return ;
    }

    if (p < 0xd000) {                 // c000 - cfff   I/O & related address space
        store_ioSpace (c, p) ;
        return ;
    }

    store_highMem (c, p) ;            // d000 - ffff   Bank-switched area

}


void Machine::store_highMem (quint8 c, quint16 p)
{
    quint8*      memory = m_ram ;
    if (RdALTZP)  memory = m_aux ;

    if (m_highRamWrite) {
        if (p < 0xE000) {
            if (RdBNK2) {
                memory[p - 0x1000] = c ;
            } else {
                memory[p] = c ;
            }
        } else {
            memory[p] = c ;
        }
    }

}


//
//   Store to soft-switches $C080-$C08F
//   This is rather poorly documented; see
//   "Understanding the Apple IIe" pp. 5-23 & 5-24
//

void Machine::storeToBankSwitches (quint16 p)
{
    quint16  pLoBits = p & 0x0003 ;   // Mask for bits 0 & 1; note that we ignore bit 2.../B

    if (p & 0x0008) RdBNK2 = OFF ;   // Use bank1 in $D000-$DFFF range
    else            RdBNK2 = ON ;    // Use bank2 in $D000-$DFFF range

    switch (pLoBits) {
        case 0:                    // Read RAM, no write
            RdLCRAM = ON ;
            break ;
        case 1:                    // Read ROM, write RAM
            if (m_highRamWrite)  RdLCRAM = OFF ;
            break ;
        case 2:                    // Read ROM, no write
            RdLCRAM = OFF ;
            break ;
        case 3:                    // Read RAM, write RAM
            if (m_highRamWrite)  RdLCRAM = ON ;
            break ;
    }
}


void Machine::storeFloppy_5_25Inch (quint8 offset, quint8 c)
{
//printf ("storeFloppy_5_25Inch offset = %2.2X\n", offset) ;
    switch (offset) {
        case 0x0:
            m_floppy->phase0_off() ;
            break ;
        case 0x1:
            m_floppy->phase0_on() ;
            break ;
        case 0x2:
            m_floppy->phase1_off() ;
            break ;
        case 0x3:
            m_floppy->phase1_on() ;
            break ;
        case 0x4:
            m_floppy->phase2_off() ;
            break ;
        case 0x5:
            m_floppy->phase2_on() ;
            break ;
        case 0x6:
            m_floppy->phase3_off() ;
            break ;
        case 0x7:
            m_floppy->phase3_on() ;
            break ;
        case 0x8:
            m_floppy->motorOff() ;
            break ;
        case 0x9:
            m_floppy->motorOn() ;
            break ;
        case 0xa:
            m_floppy->drive0_enable() ;
            break ;
        case 0xb:
            m_floppy->drive1_enable() ;
            break ;
        case 0xc:
            break ;
        case 0xd:
            m_floppy->Q6H(c) ;
            break ;
        case 0xe:
            break ;
        case 0xf:
            m_floppy->Q7H() ;
            break ;
    }

}


void Machine::store_ioSpace (quint8 c, quint16 p)
{
    if (p < 0xc100) {
        store_sspage (c,p) ;
    } else {
        if (p == 0xcfff) {         // (Reference of 0xCFFF turns off all peripheral ROM in range 0xC800-0xCFFE)
//printf ("0xCFFF stored to at %4.4X\n", m_savedPC) ;
            m_romSlot = 0 ;
            RdSLOTCXROM = OFF ;
        } else {
            printf ("PC=%4.4X  : ** ILLEGAL store of c=%2.2X to ROM near %4.4X\n", PC, c, p) ;
        }
    }
}


// store a byte to the I/O & "Soft Switch" page, C000 - C0FF

void Machine::store_sspage (quint8 c, quint16 p)
{
    if ((p < 0xc000) || (p > 0xc0ff)) {
        printf ("Bad call to store_sspage near PC=0x%4.4X, to loc=0x%4.4X\n", PC, p) ;
        return ;
    }

    if (m_snoopSSStores) ss_store_snoop (c, p) ;

    int hiNibble = (p & 0x00f0) >> 4 ;
    int loNibble = p & 0x000f ;

    switch (hiNibble) {
        case 0:                                // C000 .. C00F :        These switches are write-only.
            switch (loNibble) {
                case 0:                        // C000  Rd80STORE off (enables $C002-$C005 for aux ram)
                    Rd80STORE = OFF ;
                    break ;
                case 1:                        // C001  Rd80STORE on  (enables PAGE2 for aux. ram)
                    Rd80STORE = ON ;
                    break ;
                case 2:                        // C002  RAMRD off     (read main ram $0200-$BFFF)
                    RdRAMRD = OFF ;
                    break ;
                case 3:                        // C003  RAMRD on      (read aux. ram $0200-$BFFF)
                    RdRAMRD = ON ;
                    break ;
                case 4:                        // C004  RAMWRT off    (write main ram $0200-$BFFF)
                    RdRAMWRT = OFF ;
                    break ;
                case 5:                        // C005  RAMWRT on     (write aux. ram $0200-$BFFF)
                    RdRAMWRT = ON ;
                    break ;
                case 6:                        // C006  SETSLOTCXROM  (read peripheral rom $C800-$CFFF)
                    RdSLOTCXROM = OFF ;
//printf ("X on  pc=%4.4X\n", m_savedPC) ;
                    break ;
                case 7:                        // C007  SETINTCXROM  (read internal rom $C100-$CFFF)
                    RdSLOTCXROM = ON ;
//printf ("X off pc=%4.4X\n", m_savedPC) ;
                    break ;
                case 8:                        // C008  ALTZP off     (main stack and zero page)
                    RdALTZP = OFF ;
                    break ;
                case 9:                        // C009  ALTZP on      (aux. stack and zero page)
                    RdALTZP = ON ;
                    break ;
                case 0xa:                      // C00A  RdC3ROM off  (rom in slot 3)
//printf ("set C3 off pc=%4.4x\n", m_savedPC) ;
                    RdC3ROM = OFF ;
                    break ;
                case 0xb:                      // C00B  RdC3ROM on   (internal rom)
//printf ("set C3 on  pc=%4.4x\n", m_savedPC) ;
                    RdC3ROM = ON ;
                    break ;
                case 0xc:                      // C00C  Rd80COL off  (40 column display)
                    Rd80COL = OFF ;
                    break ;
                case 0xd:                      // C00D  Rd80COL on   (80 column display)
                    Rd80COL = ON ;
                    break ;
                case 0xe:                      // C00E  RdALTCHAR off (primary character set)
                    RdALTCHAR = OFF ;
                    break ;
                case 0xf:                      // C00F  RdALTCHAR on  (alternate character set)
                    RdALTCHAR = ON ;
                    break ;
            }
            break ;
        case 1:                                // C010 .. C01F   (Except for $C010, these switches are)
            if (loNibble == 0) {               //                (read-only, usually to test for bit 7)
                m_ss[0] &= 0x7f ;              // C010: Write: clear keyboard strobe; Read: get ASCII from keyboard)
            }
            break ;
        case 2:                                // C020 .. C02F
            break ;
        case 3:                                // C030 .. C03F  ; Toggle speaker (by reading this location)
            m_parent->speaker()->toggleSpeaker () ;
            break ;
        case 4:                                // C040 .. C04F
            break ;
        case 5:                                // C050 .. C05F
            switch (loNibble) {
                case 0:                        // C050  TEXT off / graphics on
                    RdTEXT = OFF ;
                    break ;
                case 1:                        // C051  TEXT on / graphics off
                    RdTEXT = ON ;
                    break ;
                case 2:                        // C052  MIXED text/graphics off
                    RdMIXED = OFF ;
                    break ;
                case 3:                        // C053  MIXED text/graphics on
                    RdMIXED = ON ;
                    break ;
                case 4:                        // C054  If 80STORE is off: select display page 1
                    RdPAGE2 = OFF ;
                    break ;
                case 5:                        // C055  "                                    " 1
                    RdPAGE2 = ON ;
                    break ;
                case 6:                        // C056  Display LoRes graphics
                    RdHIRES = OFF ;
                    break ;
                case 7:                        // C057  Display HiRes graphics
                    RdHIRES = ON ;
                    break ;
                case 8:
                    break ;   // XXXXXXXXXX  Add annunciator stuff here  XXXXXXXXXXXXXXXXX
                case 9:
                    break ;
                case 0xa:
                    break ;
                case 0xb:
                    break ;
                case 0xc:
                    break ;
                case 0xd:
                    break ;
                case 0xe:
                    RdDBLHIRES = ON ;
                    break ;
                case 0xf:
                    RdDBLHIRES = OFF ;
                    break ;
            }
            break ;

        case 6:                                // C060 .. C06F
            switch (loNibble) {
                case 0:            // C060: Cassette input
                    break ;
                case 1:
                    break ;
                case 2:
                    break ;
                case 3:
                    break ;
                case 4:
                    break ;
                case 5:
                    break ;
                case 6:
                    break ;
                case 7:
                    break ;
/***
From mame - MAME - Multiple Arcade Machine Emulator - redump ...
https://git.redump.net/mame/commit/?id=e019d58dfeb770c18c97a03275498bca9424d580

+		case 0x68:	// STATEREG
+			m_altzp = (data & 0x80);
+			m_page2 = (data & 0x40);
+			m_ramrd = (data & 0x20);
+			m_ramwrt = (data & 0x10);
+			m_lcram = (data & 0x08) ? false : true;
+			m_lcram2 = (data & 0x04);
+			m_intcxrom = (data & 0x01);
+
+			// update the aux state
+			auxbank_update();
+
+			// update LC state
+			if (m_lcram)
+			{
+				m_lcbank->set_bank(1);
+				m_lcaux->set_bank(1);
+				m_lc00->set_bank(1);
+				m_lc01->set_bank(1);
+			}
+			else
+			{
+				m_lcbank->set_bank(0);
+				m_lcaux->set_bank(0);
+				m_lc00->set_bank(0);
+				m_lc01->set_bank(0);
+			}
+			break;

***/
                case 8:                            // ProDos 8 v2.0.3 does a 'TRB  $C068' with A=01, but most docs
                    STATEREG = c ;                 // say $C068 is implemented only on the IIgs and later...
                    if (c & 0x01) RdSLOTCXROM  = ON ;
                    else          RdSLOTCXROM  = OFF ;
                    if (c & 0x04) RdBNK2   = ON ;
                    else          RdBNK2   = OFF ;
                    if (c & 0x08) RdLCRAM  = ON ;
                    else          RdLCRAM  = OFF ;
                    if (c & 0x10) RdRAMWRT = ON ;
                    else          RdRAMWRT = OFF ;
                    if (c & 0x20) RdRAMRD  = ON ;
                    else          RdRAMRD  = OFF ;
                    if (c & 0x40) RdPAGE2  = ON ;
                    else          RdPAGE2  = OFF ;
                    if (c & 0x80) RdALTZP  = ON ;
                    else          RdALTZP  = OFF ;
                    break ;
                case 9:
                    break ;
                case 0xa:
                    break ;
                case 0xb:
                    break ;
                case 0xc:
                    break ;
                case 0xd:
                    break ;
                case 0xe:
                    break ;
                case 0xf:
                    break ;
            }
            break ;
        case 7:                                // C070 .. C07F  Analog Input Reset
//            paddles.reset() ; XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
            break ;
        case 8:
             storeToBankSwitches (p) ;         // Do something with high mem bank switches
             break ;
        case 9:                        // Slot 1 ROM
            break ;
        case 0xa:                      // Slot 2 ROM
            break ;
        case 0xb:                      // Slot 3 ROM
            break ;
        case 0xc:                      // Slot 4 ROM
            break ;
        case 0xd:                      // Slot 5 ROM
            break ;
        case 0xe:                      // Slot 6 ROM .. Our floppy disk drive (See 'fetch_sspage')
            storeFloppy_5_25Inch (loNibble, c) ;
            break ;
        case 0xf:                      // Slot 7
            break ;
    }
}


// Snoop on stores to the "soft switch" page :$C000 ... $C0FF
// This function is provided mainly for the debugging of this emulator.

//  Refer to Apple IIe Technical Reference for the meaning of (most of)
//  the switch names.
//  Or visit <http://www.kreativekorp.com/miscpages/a2info/>

void Machine::ss_store_snoop (quint8 c, quint16 p)
{
    if ((p < 0xC000) || (p > 0xC0FF)) return ;

    int  index = p - 0xC000 ;

    const char* switchName[] = {
/* 00 */  "80STOREOFF", "80STOREON",   "RdMAINRAM",   "RDAUXRAM",     "WRMAINRAM",   "WRAUXRAM",    "SETSLOTCXROM","SETINTCXROM",
/* 08 */  "SETSTDZP",   "SETALTZP",    "SETINTC3ROM", "SETSLOTC3ROM", "CLR80VID",    "SET80VID",    "CLRALTCHAR",  "SETALTCHAR",
/* 10 */  "KBDSTRB",    "-rd only-",   "-rd only-",   "-rd only-",    "-rd only-",   "-rd only-",   "-rd only-",   "-rd only-",
/* 18 */  "-rd only-",  "-rd only-",   "-rd only-",   "-rd only-",    "-rd only-",   "-rd only-",   "-rd only-",   "-rd only-",
/* 20 */  "-rd only-",  "-rd only-",   "-rd only-",   "-rd only-",    "-rd only-",   "-rd only-",   "-rd only-",   "-rd only-",
/* 28 */  "ROMBANK",    "-rd only-",   "-rd only-",   "-rd only-",    "-rd only-",   "-rd only-",   "-rd only-",   "-rd only-",
/* 30 */  "-rd only-",  "-rd only-",   "-rd only-",   "-rd only-",    "-rd only-",   "-rd only-",   "-rd only-",   "-rd only-",
/* 38 */  "-rd only-",  "-rd only-",   "-rd only-",   "-rd only-",    "-rd only-",   "-rd only-",   "-rd only-",   "-rd only-",
/* 40 */  "-rd only-",  "-rd only-",   "-rd only-",   "-rd only-",    "-rd only-",   "-rd only-",   "-rd only-",   "-rd only-",
/* 48 */  "-rd only-",  "-rd only-",   "-rd only-",   "-rd only-",    "-rd only-",   "-rd only-",   "-rd only-",   "-rd only-",
/* 50 */  "TXTCLR",     "TXTSET",      "MIXCLR",      "MIXSET",       "RdTEXTPAGE1", "RdTEXTPAGE2", "HIRESOFF",    "HIRESON",
/* 58 */  "CLRAN0",     "-rd only-",   "CLRAN1",      "-rd only-",    "-rd only-",   "SETAN2",      "CLRAN2",      "CLRAN3",
/* 60 */  "-rd only-",  "BUTN0",       "BUTN1",       "-rd only-",    "-rd only-",   "-rd only-",   "-rd only-",   "-rd only-"
/* 68 */  "STATEREG",   "-rd only-",   "-rd only-",   "-rd only-",    "-rd only-",   "-rd only-",   "-rd only-",   "-rd only-",
/* 70 */  "-rd only-",  "-rd only-",   "-rd only-",   "-rd only-",    "-rd only-",   "-rd only-",   "-rd only-",   "-rd only-",
/* 78 */  "-rd only-",  "-rd only-",   "-rd only-",   "-rd only-",    "-rd only-",   "-rd only-",   "-rd only-",   "-rd only-",
/* 80 */  "RORamBK2",   "RRomWRamBK2", "RORomBK2",    "RWRamBK2",     "RORamBK2",    "RRomWRamBK2", "RORomBK2",    "RWRamBK2",
/* 88 */  "RORamBK1",   "RRomWRamBK1", "RORomBK1",    "RWRamBK1",     "RORamBK1",    "RRomWRamBK1", "RORomBK1",    "RWRamBK1",
    } ;


//    printf ("store @ PC=%4.4X  to  %4.4X ", PC, p) ;
    printf ("store @ PC=%4.4X  %2.2X --> %4.4X ", m_savedPC, c, p) ;
    if (index < 0x90) printf ("     %s", switchName[index]) ;
    else              printf ("     ?\n") ;
    printf ("\n") ;
//    putchar ('\n') ;
}

