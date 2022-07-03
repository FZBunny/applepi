/*****************************************************************************

    File: "fetch.cpp"

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


#include <QGuiApplication>

#include "machine.h"
#include "device_roms.h"
#include "printer.h"
#ifdef Q_OS_WINDOWS
#  include "win-speaker.h"
#else
#  include "lin-speaker.h"
#endif

//#include "tape.h"
//#include "serial_card.h"
#include "debugging_dumps.h"


#define DBUG(bits) if(m_debugFlags & bits)
#define PBUG(bits) if(m_debugFlags & bits)printf


// See "AppleIIe Technical Reference Manual" for information on soft switches (C000..C0FF) & memory management
// and "Inside The Apple//e" by Gary B. Little  ISBN 0-89303-551-3

 
//  ---------------------------------------------------------------------
//
//                    F e t c h    F u n c t i o n s
//
//  ---------------------------------------------------------------------



quint8 Machine::fetch (quint16 p)
{

// return m_rom[p] ; // =============== Uncomment to run CPU-test ROMS ====================
// Go to  https://github.com/Klaus2m5/6502_65C02_functional_tests/blob/master/bin_files/65C02_extended_opcodes_test.lst
// for test-rom image.

    quint8  c ;
    quint8 *memory = m_ram ;        // Default to main RAM 

    if (p < 0x200) {                 // 0000 - 01ff   page 0 & stack
        if (RdALTZP) memory = m_aux ;
        return memory[p] ;
    }

    if (p < 0xc000) {                // 0400 - bfff   Lower 48k RAM (except pages 0 & 1)
        c = *lower48k (p, false) ;   // ('lower48k' is in machine.cpp; shared by fetch.cpp & store.cpp)
        return c ;
    }

    if (p < 0xd000) {                // c000 - cfff   I/O-related address space
        c = fetch_ioSpace (p) ;
        return c ;
    }

    c = *fetch_highMem (p) ;         // d000 - ffff   ROM & bank-switched RAM

    return c ;

}


#define EMPTY_SLOT 0xFF
#define RTS 0x60


quint8 Machine::fetch_ioSpace (quint16 p)     //  Addresses c000 - cfff
{
    quint8  c = 0 ;

    if (p == 0xcfff) {
//printf ("0xCFFF fetched at %4.4X\n", m_savedPC) ;
        RdCXROM = OFF ;             // (Reference of 0xCFFF turns off all peripheral ROM in range 0xC800-0xCFFE)
        m_romSlot = 0 ;
        m_slotRomPointer = NULL ;
        return 0 ;   // (No idea what would actually be returned...)
    }

    if (p < 0xc100) {               // C0xx range (soft switches)
        c = fetch_sspage (p) ;
        return c ;
    }

//     ------------------------------------------------

    if (p > 0xc7ff) {
        if (m_slotRomPointer) {
            if (m_slotRomPointer) {
                if (m_romSlot == 3) c = m_slotRomPointer[p-0xc700] ;
            }
            else                  c = m_rom[p] ; 
        } else {
            c = m_rom[p] ;
        }
    } else {
        if (RdCXROM) {
            c = m_rom[p] ;
        } else {
            int loByte = p & 0xff ;
            int slotNumber = (p >> 8 ) & 0x000f ;
            switch (slotNumber) {
              case 0:
                m_romSlot = 0 ;
                break ;
              case 1:                                                           // Slot 1 $C1xx   (Printer)
                c = m_printer->fetch_Printer_ROM (slotNumber, loByte) ;
                m_slotRomPointer = NULL ;
                m_romSlot = 1 ;
                break ;
              case 2:                                                           // Slot 2 $C2xx
                c = EMPTY_SLOT ;
                m_slotRomPointer = NULL ;
                m_romSlot = 0 ;
                break ;
              case 3:                                                           // Slot 3 $C3xx   (80-col. display)
                if (IDBYTE == 6) {
                    c = m_rom[p] ;               // IDBYTE = 6 on Apple IIe & later;
                    m_slotRomPointer = NULL ;    // Use main ROM for C300+ on those models.
                    m_romSlot = 0 ;
                } else {                         // Original apple II & II+: use Card in slot 3.
//                    c = AE_Viewmaster80[loByte] ;        ** NOT IMPLEMENTD as of 2022-03-03 **
//                    m_romSlot = 3 ;
//                    m_slotRomPointer = AE_Viewmaster80 ;
                }
                break ;
              case 4:                                                           // Slot 4 $C4xx
                c = EMPTY_SLOT ;
                m_slotRomPointer = NULL ;
                m_romSlot = 0 ;
                break ;
              case 5:                                                           // Slot 5 $C5xx                                                          // Slot 4 $C4xx
                c = EMPTY_SLOT ;
                m_slotRomPointer = NULL ;
                m_romSlot = 0 ;
                break ;
              case 6:                                                           // Slot 6 $C6xx   (Disk II; 5.25" drive)
                c = floppy_5_25Inch_ROM[loByte] ;
                m_slotRomPointer = NULL ;
                m_romSlot = 6 ;
                break ;
              case 7:                                                           // Slot 7 $C7xx   (Hard or 3.5" drive)
                c = m_hardDrive->fetch_HD_ROM (slotNumber, loByte) ;
                m_slotRomPointer = NULL ;
                m_romSlot = 7 ;
                break ;
              default:
                break ;
            }
        }
    }

    return c ;
}


// Address is in range $D000 - $FFFF
// Return a pointer to main or auxiliary RAM, or to ROM.

quint8* Machine::fetch_highMem (quint16 p)
{
    quint8 *memory ;

    if (RdLCRAM) {
        if (RdALTZP) memory = m_aux ;
        else         memory = m_ram ;
        if ((p < 0xe000) && RdBNK2) memory -= 0x1000 ;  // map c000-cfff to d000-dfff
    } else {
        memory = m_rom ;
    }

    return memory + p ;

}

// Fetch a byte from the "Soft Switch" page, C000 - C0FF

quint8 Machine::fetch_sspage (quint16 p)
{
    quint8  c ;
    quint32 mods ;   // used only for open & closed Apple keys ($C061 & $C062)

    if (m_trace && m_snoopSSFetches) {
        if ((m_savedPC >= m_tracelo) && (m_savedPC <= m_tracehi)) {
            ss_fetch_snoop (p) ;
        }
    }

    int hiNibble = (p & 0x00f0) >> 4 ;
    int loNibble =  p & 0x000f ;

    if ((p < 0xc000) || (p > 0xc0ff)) {
        printf ("Bad call to fetch_sspage near PC=0x%4.4X, to loc=0x%4.4X\n", PC, p) ;
        return 0 ;
    }

    c = m_ss[p-0xC000] ;

    switch (hiNibble) {
        case 0:                                // C000 .. C00F
            c = m_ss[0] ;
            break ;
        case 1:                                // C010 .. C01F
            switch (loNibble) {
                case 0:                        // C010: clear keyboard strobe bit
                    m_ss[0] &= 0x7f ;
                    break ;
                case 1:                        // C011  read BANK2 switch
                    c = RdBNK2 ;
                    break ;
                case 2:                        // C012  read LCRAM switch
                    c = RdLCRAM ;
                    break ;
                case 3:                        // C013  read RdRAMRD switch
                    c = RdRAMRD ;
                    break ;
                case 4:                        // C014  read RAMWRT switch
                    c = RdRAMWRT ;
                    break ;
                case 5:                        // C015  read RdCXROM switch
                    c = RdCXROM ;//printf ("RdCXROM = %2.2x  m_savedPC=%4.4X\n", RdCXROM, m_savedPC) ;
                    break ;
                case 6:                        // C016  read ALTZP switch
                    c = RdALTZP ;
                    break ;
                case 7:                        // C017  read RdC3ROM switch
                    c = RdC3ROM ;
                    break ;
                case 8:                        // C018  read Rd80STORE switch
                    c = Rd80STORE ;
                    break ;
                case 9:                        // C019  read RdVBL switch
//                    c = RdVBL ;
                    c = rand() ;   //  XXXXXXXXXXXXXXXXXXXXXXXXXXXX is this a reasonable thing to do ?  XXXXXXXXX
                    break ;
                case 0xa:                      // C01A  read TEXT switch
                    c = RdTEXT ;
                    break ;
                case 0xb:                      // C01B  read mixed-display switch "RdMIXED"
                    c = RdMIXED ;
                    break ;
                case 0xc:                      // C01C  read PAGE2 switch
                    c = RdPAGE2 ;
                    break ;
                case 0xd:                      // C01D  read HIRES switch
                    c = RdHIRES ;
                    break ;
                case 0xe:                      // C01E  read ALTCHAR switch
                    c = RdALTCHAR ;
                    break ;
                case 0xf:                      // C01F  read 80COL switch
                    c = Rd80COL ;   
                    break ;
            }
            break ;
        case 2:                                // C020 .. C02F  ; Cassette output
//            m_tape->toggleTapeOutput (m_nCycles) ;
            break ;
        case 3:                                // C030 .. C03F  ; Speaker
            m_parent->speaker()->toggleSpeaker () ;
            break ;
        case 4:                                // C040 .. C04F
            break ;
        case 5:                                // C050 .. C05F
            switch (loNibble) {
                case 0:                        // C050  TEXT off
                    RdTEXT = OFF ;
                    break ;
                case 1:                        // C051  TEXT on
                    RdTEXT = ON ;
                    break ;
                case 2:                        // C052  MIXED off
                    RdMIXED = OFF ;
                    break ;
                case 3:                        // C053  MIXED on
                    RdMIXED = ON ;
                    break ;
                case 4:                        // C054  display page2 off
                    RdPAGE2 = OFF ;
                    break ;
                case 5:                        // C055  display page2 on
                    RdPAGE2 = ON ;
                    break ;
                case 6:                        // C056  HIRES off
                    RdHIRES = OFF ;
                    break ;
                case 7:                        // C057  HIRES on
                    RdHIRES = ON ;
                    break ;
                case 8:                        //   XXXXXX add annunciator stuff ? XXXXXX
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
                    RdDBLHIRES = ON ;
                    break ;
                case 0xf:
                    RdDBLHIRES = OFF ;
                    break ;
            }
            break ;

        case 6:                                // C060 .. C06F
            switch (loNibble) {
                case 0:                        // C060: Cassette input  /  also Switch 3 ?
 //                   c = m_tape->readTapeInput() ;
                    c = m_parent->m_gamepad->readButton(3) ;
                    break ;
                case 1:                        // C061  Switch 0 / Left-alt  ("open apple" key)
                    mods = QGuiApplication::queryKeyboardModifiers() ;   // XXXX FIXME - left / right apple keys are not distinguished XXXXX
                    if (mods & Qt::AltModifier) c = 0xff ;
                    else                        c = m_parent->m_gamepad->readButton(0) ;
                    break ;
                case 2:                        // C062  Switch 1 / Right-alt ("solid apple" key)
                    mods = QGuiApplication::queryKeyboardModifiers() ;   // XXXX FIXME - left / right apple keys are not distinguished XXXXX
                    if (mods & Qt::AltModifier) c = 0xff ;
                    else                        c = m_parent->m_gamepad->readButton(1) ;
                    break ;
                case 3:                        // C063  Switch 2
                    c = m_parent->m_gamepad->readButton(2) ;
                    break ;
                case 4:                        // C064  Analog input 0  (paddle 0 / gamepad 1 X-axis)
                    c = m_parent->m_gamepad->readAnalog(0) ;
                    break ;
                case 5:                        // C065  Analog input 1  (paddle 1 / gamepad 1 Y-axis)
                    c = m_parent->m_gamepad->readAnalog(1) ;
                    break ;
                case 6:                        // C066  Analog input 2  (paddle 2 / gamepad 2 X-axis)readGamepadreadGamepad
                    c = m_parent->m_gamepad->readAnalog(2) ;
                    break ;
                case 7:                        // C067  Analog input 3  (paddle 3 / gamepad 2 y-axis)
                    c = m_parent->m_gamepad->readAnalog(3) ;
                    break ;
                case 8:                        // C068 STATEREG
                    c = 0 ;                         // ( ProDos 8 v2.0.3 does a 'TRB  $C068', but most docs )
                    if (RdCXROM)       c =  0x01 ;  // ( say 'STATEREG' is implemented only on the IIgs and later... )
                    if (RdBNK2)        c |= 0x04 ;
             //               if (RdBNK2 == OFF) c |= 0x08 ;RdLCRAM
                    if (RdLCRAM)       c &= 0xf7 ;
                    if (RdRAMWRT)      c |= 0x10 ;
                    if (RdRAMRD)       c |= 0x20 ;
                    if (RdPAGE2)       c |= 0x40 ;
                    if (RdALTZP)       c |= 0x80 ;
//printf ("fetch from C068:  m_savedPC=%4.4x c=%2.2x\n", m_savedPC, c) ;
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
        case 7:                                // C070 .. C07F  Misc...
            if (loNibble==0) {    // Reset Analog inputs (gamepad)
                m_parent->m_gamepad->reset() ;
            }
            c = 0 ;
            break ;
       case 8:                                 // C080 .. C08F  Bank Switching
            fetchFromBankSwitches(p) ;
            break ;
        case 9:                   //  slot 1      C090 - C09F  (Printer I/O)
            m_romSlot = 1 ;
            c = m_printer->fetch (loNibble) ;
printf ("Fetch %2.2x from slot 1, p=%4.4x\n", c, p) ;
            break ;
        case 0xa:                 //  slot 2      C0A0 - C0AF
            m_romSlot = 2 ;
            break ;
        case 0xb:                 //  slot 3      C0B0 - C0BF
            m_romSlot = 3 ;
            break ;
        case 0xc:                 //  slot 4      C0C0 - C0CF
            m_romSlot = 4 ;
            break ;
        case 0xd:                 //  slot 5      C0D0 - C0DF
            m_romSlot = 5 ;
// printf ("Slot5: fetch from %4.4X\n", p) ;
            break ;
        case 0xe:                 //  slot 6      C0E0 - C0EF  (floppy disk I/O)
            m_romSlot = 6 ;       //  (Value of 'm_romSlot' is not actually used..)
            c = fetchFloppy_5_25Inch (loNibble) ;
            break ;
        case 0xf:                 //  slot 7      C0F0 - C0FF
printf ("Slot7 ss: fetch from %4.4X at PC=%4.4X\n", p, m_savedPC) ;
            m_romSlot = 7 ;
            break ;
    }

    return c ;
}


//
//   Set soft-switches for access to addresses for ROM $D000-$FFFF
//   and main/aux. RAM $C000-$FFFF
//
// In addition to Apple docs, see: "Understanding the Apple lie",
// by James Fielding Sather
// Brady Communications Co., Inc.  ISBN0-a351-fl0n-7
//
// [ pp 5-23 and 5-24 contain details on writing to soft-switches ]
// [ $C080-$C08F which are unavailable elsewhere.                 ]
//
void Machine::fetchFromBankSwitches (quint16 p)                  // C080 .. C08F; set bank switches
{
    quint16  pLoBits = p & 0x0003 ;   // Mask for bits 0 & 1; note that we ignore bit 2.

    if (p & 0x0008) RdBNK2 = OFF ;   // Use bank1 in $D000-$DFFF range
    else            RdBNK2 = ON ;    // Use bank2 in $D000-$DFFF range

    switch (pLoBits) {
        case 0:                    // Read RAM, no write
            RdLCRAM = ON ;
            break ;
        case 1:                    // Read ROM, write RAM (if odd address fetched twice)
            if (m_highWritePreset) m_highRamWrite = true ;
            m_highWritePreset = !m_highWritePreset ;
            if (m_highRamWrite)  RdLCRAM = OFF ;
            break ;
        case 2:                    // Read ROM, no write
            RdLCRAM = OFF ;
            break ;
        case 3:                    // Read RAM, write RAM (if odd address fetched twice)
            if (m_highWritePreset) m_highRamWrite = true ;
            m_highWritePreset = !m_highWritePreset ;
            if (m_highRamWrite)  RdLCRAM = ON ;
            break ;
    }

}


//qint8 Machine::fetchFloppy_5_25Inch (int loNibble) { return 0; } ;


qint8 Machine::fetchFloppy_5_25Inch (int loNibble)  //  Read or write a single byte from/to a 5-1/4" floppy drive
{
    quint8 c = 0 ;

    switch (loNibble) {
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
            c = m_floppy->Q6L() ;
            break ;
        case 0xd:
            m_floppy->Q6H(0) ;
            break ;
        case 0xe:
            c = m_floppy->Q7L() ;
            break ;
        case 0xf:
            m_floppy->Q7H() ;
            break ;
    }

    return c ;
}


// Snoop on fetches fom the "soft switch" page :$C000 ... $C0FF
// This function is provided mainly for debugging this emulator.

//   Refer to Apple IIe Technical Reference for the meaning of
//   (most of) the switch names.
//   Or visit <http://www.kreativekorp.com/miscpages/a2info/>


void Machine::ss_fetch_snoop (quint16 p)
{
    if ((p < 0xc000) || (p > 0xc0ff)) return ;

    int    index = p - 0xC000 ;
    quint8 c = m_ss[index] ;

    const char* switchName[] = {
/* 00 */  "KBD",        "-wrt only-", "-wrt only-", "-wrt only-", "-wrt only-", "-wrt only-", "-wrt only-",  "-wrt only-", // 07
/* 08 */  "-wrt only-", "-wrt only-", "-wrt only-", "-wrt only-", "-wrt only-", "-wrt only-", "-wrt only-",  "-wrt only-", // 0F
/* 10 */  "KBDSTRB",    "RdBNK2",     "RdLCRAM",    "RdRAMRD",    "RdRAMWRT",   "RdCXROM",    "RdALTZP",     "RdC3ROM",    // 17
/* 18 */  "Rd80STORE",  "RdVBL",      "RdTEXT",     "RdMIXED",    "RdPAGE2",    "RdHIRES",    "RdALTCHAR",   "Rd80COL",    // 1F
/* 20 */  "TAPEOUT",    "-wrt only-", "-wrt only-", "-wrt only-", "-wrt only-", "-wrt only-", "-wrt only-",  "-wrt only-", // 27
/* 28 */  "ROMBANK",    "?",          "?",          "?",          "?",          "?",          "?",           "?",          // 2F
/* 30 */  "SPEAKER",    "?",          "?",          "?",          "?",          "?",          "?",           "?",          // 37
/* 38 */  "?",          "?",          "?",          "?",          "?",          "?",          "?",           "?",          // 3F
/* 40 */  "?",          "?",          "?",          "?",          "?",          "?",          "?",           "?",          // 47
/* 48 */  "?",          "?",          "?",          "?",          "?",          "?",          "?",           "?",          // 4F
/* 50 */  "TXTCLR",     "TXTSET",     "MIXCLR",     "MIXSET",     "TXTPAGE1",   "TXTPAGE2",   "HIRESOFF",    "HIRESON",    // 57
/* 58 */  "CLRAN0",     "?",          "CLRAN1",     "?",          "?",          "SETAN2",     "DBLHIRESON",  "DBLHIRESOFF",// 5F
/* 60 */  "TAPEIN",     "RDBTN0",     "RDBTN1",     "?",          "PADDL0",     "PADDL1",     "PADDL2",      "PADDL3",     // 67
/* 68 */  "STATEREG",   "?",          "?",          "?",          "?",          "?",          "?",           "?",          // 6F
/* 70 */  "PTRIG",      "?",          "?",          "?",          "?",          "?",          "?",           "?",          // 77
/* 78 */  "?",          "?",          "?",          "?",          "?",          "?",          "RDIOUDIS",    "RdDBLHIRES", // 7F
/* 80 */  "RORamBK2",   "RRomWRamBK2","RORomBK2",   "RWRamBK2",   "RORamBK2",   "RRomWRamBK2","RORomBK2",    "RWRamBK2",   // 87
/* 88 */  "RORamBK1",   "RRomWRamBK1","RORomBK1",   "RWRamBK1",   "RORamBK1",   "RRomWRamBK1","RORomBK1",    "RWRamBK1",   // 8F
/* 90 */  "SLOT1,0",    "SLOT1,1",    "SLOT1,2",    "SLOT1,3",    "SLOT1,4",    "SLOT1,5",    "SLOT1,6",     "SLOT1,7",    // 90
/* 98 */  "SLOT1,8",    "SLOT1,9",    "SLOT1,A",    "SLOT1,B",    "SLOT1,C",    "SLOT1,D",    "SLOT1,E",     "SLOT1,F",    // 9F
/* A0 */  "SLOT2,0",    "SLOT2,1",    "SLOT2,2",    "SLOT2,3",    "SLOT2,4",    "SLOT2,5",    "SLOT2,6",     "SLOT2,7",    // A0
/* A8 */  "SLOT2,8",    "SLOT2,9",    "SLOT2,A",    "SLOT2,B",    "SLOT2,C",    "SLOT2,D",    "SLOT2,E",     "SLOT2,F",    // AF
/* B0 */  "SLOT3,0",    "SLOT3,1",    "SLOT3,2",    "SLOT3,3",    "SLOT3,4",    "SLOT3,5",    "SLOT3,6",     "SLOT3,7",    // B0
/* B8 */  "SLOT3,8",    "SLOT3,9",    "SLOT3,A",    "SLOT3,B",    "SLOT3,C",    "SLOT3,D",    "SLOT3,E",     "SLOT3,F",    // BF
/* C0 */  "SLOT4,0",    "SLOT4,1",    "SLOT4,2",    "SLOT4,3",    "SLOT4,4",    "SLOT4,5",    "SLOT4,6",     "SLOT4,7",    // C0
/* C8 */  "SLOT4,8",    "SLOT4,9",    "SLOT4,A",    "SLOT4,B",    "SLOT4,C",    "SLOT4,D",    "SLOT4,E",     "SLOT4,F",    // CF
/* D0 */  "SLOT5,0",    "SLOT5,1",    "SLOT5,2",    "SLOT5,3",    "SLOT5,4",    "SLOT5,5",    "SLOT5,6",     "SLOT5,7",    // D0
/* D8 */  "SLOT5,8",    "SLOT5,9",    "SLOT5,A",    "SLOT5,B",    "SLOT5,C",    "SLOT5,D",    "SLOT5,E",     "SLOT5,F",    // DF
/* E0 */  "DK_P0_OFF",  "DK_P0_ON",   "DK_P1_OFF",  "DK_P1_ON",   "DK_P2_OFF",  "DK_P2_ON",   "DK_P3_OFF",   "DK_P3_ON",   // E0
/* E8 */  "DK_MOT_OFF", "DK_MOT_ON",  "DK_SEL_DR0", "DK_SEL_DR1", "DK_Q6L",     "DK_Q6H",     "DK_Q7L",      "DK_Q7H",     // EF
/* F0 */  "SLOT7,0",    "SLOT7,1",    "SLOT7,2",    "SLOT7,3",    "SLOT7,4",    "SLOT7,5",    "SLOT7,6",     "SLOT7,7",    // F0
/* F8 */  "SLOT7,8",    "SLOT7,9",    "SLOT7,A",    "SLOT7,B",    "SLOT7,C",    "SLOT7,D",    "SLOT7,E",     "SLOT7,F",    // FF
    } ;

    printf ("fetch @ PC=%4.4X from %4.4X=%2.2X ", m_savedPC, p, c) ;
    printf ("%s\n", switchName[index]) ;
    fflush (stdout) ;
}


