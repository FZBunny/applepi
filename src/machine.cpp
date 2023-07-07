/*****************************************************************************

    File: "machine.cpp"

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


#include <stdlib.h>
#include <QTime>
#include <QRandomGenerator>

#include "machine.h"
#include "usleep.h"
#include "config.h"
#include "device_roms.h"
#include "floppy_controller.h"
#include "traptrace.h"
#ifdef Q_OS_WINDOWS
#   include "windows.h"
#endif

//#include "tape.h"
//#include "disassembler.h"
//#include "debugging_dumps.h"


// ***********************************************************************************************
//  A NOTE about source files for the "Machine" class :
//  Since the code to both fetch and store a single byte totals over 1,000 lines, both "fetch" and
//  "store" functions are given their own source files, "fetch.cpp" and "store.cpp".
//  Otherwise, this file would be too large to be comfortable.
// ***********************************************************************************************


#define MARK_UNIMPLEMENTED true
#define UNIMPLEMENTED  if (MARK_UNIMPLEMENTED)\
        printf("*** Unimplemented opcode %2.2X executed at %4.4x\n", m_registers.opcode, m_savedPC) ;


Machine::Machine (MainWindow* parent)
{
    m_floppy = new FloppyDiskController (this) ;
    m_hardDrive = new HdController (this) ;
    m_printer = new Printer (this) ;

//    m_tape = new Tape (this) ;

    m_disassembler = new Disassembler (m_rom, false) ;
    m_parent = parent ;
    m_powerIsOn = false ;
    m_trace = false ;

    for (int i=0; i<4; i++) {
        m_trapPointAddr[i] = 0 ;
        m_trapPointSet[i] = false ;
    }

    m_nCycles = 0 ;
    initialize (false) ;
}


void Machine::initialize (bool power)
{
    m_powerIsOn = power ;

    if ( ! power) return ;  // Don't clear anything if power has been turned off;
                            // this helps with debugging.

    m_singleStepPhase = CLR ;
    m_halted = false ;
    m_WAI_executed = false ;
    m_STP_executed = false ;

    RdCXROM = ON ;

    m_historyOldest = 0 ;
    m_historyNewest = 0 ;
    memset (m_history, 0, HISTORY_LEN*sizeof(ProcessorState)) ;

    PCLO = m_rom[RESET] ;
    PCHI = m_rom[RESET+1] ;
 //   A = 0 ;
 //   X = 0 ;
 //   Y = 0 ;
 //   S = 0xff ;
 //   P = 0x20 ;
    Areg = 0xff ;
    Xreg = 0xff ;
    Yreg = 0xff ;
    Sreg = 0xfc ;
    Preg = 0xff ;
    memset (m_ss,  0, 0x0100) ;
    initializeRAM() ;

    RdMIXED = OFF ;
    RdHIRES = OFF ;

    m_nCycles = 0 ;
    m_previousCycles = 0 ;
    m_romSlot = 0 ;
    m_slotRomPointer = NULL ;
    m_monoDblHiResState = 0 ;
    m_highRamWrite = true ;        // See "Understanding the Apple IIe" p. 5-23
    m_highWritePreset = false ;
    m_waitForCPUTimer = false ;
    m_waitForCPUTimer = false;
    m_highRamWrite = false;
    m_highWritePreset = false;
    m_snoopSSFetches = false;
    m_snoopSSStores = false;
    m_echoToTerminal = false;
    m_echoingToFile = false ;
    m_WAI_executed = false;
    m_STP_executed = false;
    m_dumpHistory = false;
    for (int i = 0; i < 4; i++) {
        m_trapPointSet[i] = false;
        m_watchAddr[i] = false;

    }

    closeEchoFile() ;

}


// Generate a slightly randomized pattern on screen after power-up.
// A kind'a lame attempt to mimic dynamic RAM behaviour.

void Machine::initializeRAM (void)
{
    QTime time =  QTime::currentTime();
    quint32 seed = time.msecsSinceStartOfDay() ;   // Good enough seed.
//printf ("seed = %8.8x\n", seed) ;
    QRandomGenerator random (seed) ;

    for (int i=0; i<0x010000; i+=4) {
        m_ram[i]   = m_ram[i+1] = 0xff ;
        m_ram[i+2] = m_ram[i+3] = 0x00 ;
        m_aux[i]   = m_aux[i+1] = 0xff ;
        m_aux[i+2] = m_aux[i+3] = 0x00 ;
        int n1 = random.generate() % 1000 ;
        if (n1 < 10) {                       // Every so often, put some random crap in...
            long int n2 = random.generate() ;
            long int n3 = random.generate() ;
            m_ram[i]   = n2 ;
            m_ram[i+1] = n3 ;
            m_aux[i]   = n2 ;                // (No one will notice that the random crap )
            m_aux[i+1] = n3 ;                // (is identical in main & aux RAM. Or care.)
        }
    }
}


//   This is a helper for functions "fetch" & "store".
//
//   For addresses in the range of 0x0200-0xBFFF, return the address of
//   a byte within main or auxiliary memory, depending upon the address "p",
//   and the states of soft-switches 80STORE, PAGE2, HIRES, and RAMRD/RAMWRT.
//   (See  Apple IIe Technical Notes, figures 4, 5, and 6)
//

quint8* Machine::lower48k (quint16 p, bool store)   // (Not really 48K; doesn't include pages 0 & 1, but WTF.)
{

    quint8 ramRW ;
    if (store) ramRW = RdRAMWRT ;
    else       ramRW = RdRAMRD  ;

    if ((ramRW==OFF) && (Rd80STORE==OFF))                  return m_ram + p ;
    if ((ramRW==OFF) && (RdPAGE2==OFF) && (Rd80STORE==ON)) return m_ram + p ;

    if ((ramRW==ON) && (Rd80STORE==OFF))                   return m_aux + p ;
    if ((ramRW==ON) && (RdPAGE2==ON) && (Rd80STORE==ON))   return m_aux + p ;

    if (Rd80STORE==OFF)                                    return m_ram + p ; // XXXXX This may not be correct! FIXME?

    int switches = 0 ;
    if (RdPAGE2==ON) switches |= 1 ;
    if (RdHIRES==ON) switches |= 2 ;
    if (ramRW==ON)   switches |= 4 ;

    quint8* memory = m_ram ;           // default to 'm_ram' (main memory)
    switch (switches) {
       case 1:                         // Only PAGE2 is on :                         //   ... Use Aux memory if ...
          if (p>=0x0400 && p<0x0800) memory = m_aux ;                                // If p in range $0400-$07ff
          break ;
       case 3:                         // PAGE2 & HIRES are ON :
          if ((p>=0x0400 && p<0x0800) || (p>=0x2000 && p<0x4000)) memory = m_aux ;   // If p in range $0400-$07ff
          break ;                                                                    // or $2000...$3fff
       case 4:                         // only ramRW is ON (other two are off) :
          if (p>=0x0800 || p<0x0400) memory = m_aux ;                                // If p is under $400, or p is
          break ;                                                                    // $0800 and up
       case 6:                         // HIRES is ON & ramRW is ON :
          if ((p<0x0400) || (p>=0x4000) || (p>=0x0800 && p<0x2000)) memory = m_aux ; // If p is under $400 or in range 
          break ;                                                                    // $0800-$1fff, or is $4000 and up
    }

    return memory + p ;
}


void Machine::reset (void)
{
    m_WAI_executed = false ;
    m_STP_executed = false ;
//printf ("1... Machine::reset  PC = %4.4X\n", PC) ;
    PCLO = m_rom[RESET] ;
    PCHI = m_rom[RESET+1] ;
//printf ("2... Machine::reset  PC = %4.4X\n\n", PC) ;
    m_romSlot = 0 ;
    m_slotRomPointer = NULL ;
}


bool Machine::powerIsOn (void)
{
    return m_powerIsOn ;
}


void Machine::turnOn(void)
{
    m_powerIsOn = true ;
}


void Machine::turnOff(void)
{
    m_powerIsOn = false ;
}


void Machine::shutDown(void)
{
    m_powerIsOn = false ;
    m_floppy->close(0) ;
    m_floppy->close(1) ;
    m_floppy->forgetTrackNumbers() ;
} 
//    m_tape->close() ;
//    closeCapture() ;


void Machine::performIRQ (void)
{
    PC++ ;
    Sreg &= 0xff ;
    store (PCHI, 0x100|Sreg--) ;
    Sreg &= 0xff ;
    store (PCLO, 0x100|Sreg--) ;
    Sreg &= 0xff ;
    store (Preg, 0x100|Sreg--) ;
    Sreg &= 0xff ;
    PCLO = fetch(IRQ) ;
    PCHI = fetch(IRQ+1) ;
printf ("PCHI,PCLO = %2.2X%2.2X\n", PCHI, PCLO) ;
    Preg |= (UN | I) ;
    cycles(7) ;      // NMOS 6502 does not do this.
}



bool Machine::toggleEchoToTerminal (void)
{
    return m_echoToTerminal = ! m_echoToTerminal ;
}


bool Machine::isEchoingToFile (void)
{
    return m_echoingToFile ;
}


void Machine::echoToFile (QFile* f)
{
    m_textEchoFile = f ;
    m_echoingToFile = true ;
}


void Machine::closeEchoFile (void)
{
    if (m_echoingToFile) {
        m_textEchoFile->close() ;
        delete (m_textEchoFile) ;
    }
    m_echoingToFile = false ;
}


void Machine::setTrap (uint trapIndex, QString addressString , bool flag)
{
    if (trapIndex > 3) trapIndex = 3 ;
    ulong longAddress ;
    longAddress = addressString.toULong (nullptr, 16) ;
    quint16 shortAddress = longAddress ;
    m_trapPointAddr[trapIndex] = shortAddress ;
    m_trapPointSet[trapIndex] = flag ;
// printf ("Machine::setTrap  trapIndex=%i; shortAddress=%4.4X  flag=%i\n", trapIndex, shortAddress, flag) ;
}


void Machine::setWatch (uint watchIndex, QString addressString , bool flag)
{
    if (watchIndex > 3) watchIndex = 3 ;
    ulong longAddress ;
    longAddress = addressString.toULong (nullptr, 16) ;
    quint16 shortAddress = longAddress ;
    m_watchAddr[watchIndex] = shortAddress ;
    m_watchAddrSet[watchIndex] = flag ;
printf ("Machine::setWatch  watchIndex=%i; shortAddress=%4.4X  flag=%i\n", watchIndex, shortAddress, flag) ;
}

void Machine::setTrapTraceDialogPointer (TrapTrace *dialog)
{
    m_trapTraceDialogPointer = dialog ;
}


void Machine::setWatchAddrDialogPointer (WatchAddr *dialog)
{
    m_watchAddrDialogPointer = dialog ;
}

void Machine::setLines2Dump (QString nLines)
{
    ulong longLines ;
    longLines = nLines.toULong (nullptr, 10) ;
    m_requestedDumpLines = longLines ;
}


//       D E C I M A L   A R I T H M E T I C    H E L P E R S


void Machine::addmem (quint16 p)
{
    quint8 c, alo, ahi, mlo, mhi ;
    quint16 s, m ;

    if (Preg & D) {         // decimal addition. Ew...
        c = Preg & C ;
        Preg &= (N | Z | C) ^ 0xff ;
        Areg &= 0xff ;
        s = Areg ;
        m = fetch(p) ;
        alo = Areg & 0x0f ;
        mlo = m & 0x0f ;
        alo = alo + mlo + c ;
        c = 0 ;
        if (alo > 9) {
            c = 1 ;
            alo = (alo - 10) & 0x0f ;
        }
        ahi = Areg >> 4 ;
        mhi = m >> 4 ;
        ahi = ahi + mhi + c ;
        c = 0 ;
        if (ahi > 9) {
            c = 1 ;
            ahi = (ahi - 10) & 0x0f ;
        }
        Areg = ((ahi<<4) & 0xf0) | (alo & 0x0f) ;
        Preg |= c ;
        if ((s & 0x80) == (m & 0x80)) {   // XXXXXX   V flag is sometimes wrong
            if ((Areg & 0x80) != (s & 0x80)) Preg |= V ;
        }
//printf ("addmem: m_parent->romNumber() = %i\n", m_parent->romNumber()) ;
        if (m_parent->romNumber() != APPLE2E_ENHANCED) {
//printf ("addmem: not APPLE2E_ENHANCED\n") ;
            if (Areg == 0) Preg |= N ;     // only on 6502.  65C02 does not set N on decimal addition.
        }
//printf ("PC=%4.4x  romNumber=%i  P=%2.2x\n", m_savedPC, m_parent->romNumber(), P) ;
    } else {           // binary addition.
        s = Areg ;
        m = fetch(p) ;
        Areg += m + (Preg & C) ;
        STATNZC(Areg) ;
        Areg &= 0xff ;
        Preg &= V ^ 0xff ;
        if ((s & 0x80) == (m & 0x80)) {
            if ((Areg & 0x80) != (s & 0x80)) Preg |= V ;
        }
    }
}


void Machine::submem (quint16 p)
{
    quint16  c, alo, ahi, mlo, mhi ;
    quint16  s, m ;

    if (Preg & D) {         // decimal subtraction. Likewise, ew.
        c = Preg & C ;
        Preg &= (N | Z | C | V) ^ 0xff ;
        alo = Areg & 0x0f ;
        ahi = (Areg >> 4) & 0x0f ;
        m = fetch(p) ;
        mlo = m & 0x0f ;
        mhi = (m >> 4) & 0x0f ;

        if (c == 0) mlo += 1 ;
        alo -= mlo ;
        if (alo & 0xfff0) {
            alo += 10 ;
            alo &= 0x0f ;
            mhi += 1 ;
        }
        ahi -= mhi ;
        c = 1 ;
        if (ahi & 0xfff0) {
            ahi += 10 ;
            c = 0 ;
        }
        Preg |= c ;
//
        Areg = ((ahi << 4) & 0xf0) | (alo & 0x0f)  ;
        if (Areg == 0) Preg |= Z ;
    } else {           // binary subtraction

        s = Areg ;
        m = fetch(p) ^ 0xff ;
        c = Preg & C ;
        Areg += m + c ;
        Preg &= (N | Z | C | V) ^ 0xff ;
        if (Areg & 0x100) Preg |= C ;
        Areg &= 0xff ;
        if (Areg == 0) Preg |= Z ;
        if (Areg & 0x80) Preg |= N ;
        if ((s & 0x80) == (m & 0x80)) {
            if ((Areg & 0x80) != (s & 0x80)) Preg |= V ;
        }
    }
}



void Machine::cycles (int n)
{
    m_nCycles += n ;
}


/*****
void Machine::closeSlot1Out (void)
{
    if (m_slot1_OutFileHandle) {
        fclose(m_slot1_OutFileHandle) ;
        m_slot1_OutFileHandle = NULL ;
    }
}



void Machine::closeSlot1In (void)
{
    if (m_slot1_InFileHandle) {
        fclose(m_slot1_InFileHandle) ;
        m_slot1_InFileHandle = NULL ;
    }
}
*****/


// Return 'true' if CLR80VID, DHIRESON, DHIRESOFF, and SET80VID
// have been accessed in the proper sequence, resulting in
// 'm_monoDblHiResState' having been incremented to 6.
// This sequence is expected by certain (most?) colour video
// cards to persuade them to display monochrome.
// Strange, but true.

bool Machine::displayMonoDblHiRes (void)
{
    if (m_monoDblHiResState == 6) return true ;
    else                          return false ;
}


void Machine::setX (quint8 x)
{
    m_registers.Xregister = x ;
}


void Machine::setY (quint8 y)
{
    m_registers.Yregister = y ;
}


quint64 Machine::getCycles (void)
{
    return m_nCycles ;
}


quint16 Machine::getPC (void)
{
    return m_registers.pc.pc_16 ;
}


bool Machine::traceIsOn (void)
{
    return m_trace ;
}


void Machine::enableTrace (bool state)
{
    m_trace = state ;
}


void Machine::setTraceStart (quint16 loAddress)
{
    m_tracelo = loAddress ;
}


void Machine::setTraceEnd (quint16 hiAddress)
{
    m_tracehi = hiAddress ;
}


void  Machine::singleStep (void)
{
    m_singleStepPhase = SET ;
    m_halted = false ;
}


void  Machine::runFree (void)
{
    m_singleStepPhase = CLR ;
    m_halted = false ;
}


void  Machine::halt (void)
{
    if (m_halted) {
        m_singleStepPhase = CLR ;
        m_halted = false ;
    } else {
        m_halted = true ;
        m_singleStepPhase = SET ;
    }
//printf ("Machine::halt   m_singleStepPhase=%i  m_halted=%i\n", m_singleStepPhase, m_halted) ;
}


void Machine::ss_setFetchSnoop (bool flag)
{
    m_snoopSSFetches = flag ;
}


void Machine::ss_setStoreSnoop (bool flag)
{
    m_snoopSSStores = flag ;
}


void Machine::sprint_6502_registers (ProcessorState& regs, char* buffer)
{
    quint8 p = regs.Pstat ;

    static quint8 processorType [256] =     //  0 : 6502
    {// 0 1 2 3 4 5 6 7 8 9 A B C D E F     //  1 : 65C02
/* 0 */ 0,0,0,0,2,0,0,2,0,0,0,0,2,0,0,2,    //  2 : Rockwell & WDC additions to 65C02
/* 1 */ 0,0,1,0,2,0,0,2,0,0,1,0,2,0,0,2,    //  3 : WDC-only additions to 65C02
/* 2 */ 0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,2,
/* 3 */ 0,0,1,0,1,0,0,2,0,0,1,0,1,0,0,2,
/* 4 */ 0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,2,
/* 5 */ 0,0,1,0,0,0,0,2,0,0,1,0,0,0,0,2,
/* 6 */ 0,0,0,0,1,0,0,2,0,0,0,0,0,0,0,2,
/* 7 */ 0,0,1,0,1,0,0,2,0,0,1,0,1,0,0,2,
/* 8 */ 1,0,0,0,0,0,0,2,0,1,0,0,0,0,0,2,
/* 9 */ 0,0,1,0,0,0,0,2,0,0,0,0,1,0,1,2,
/* A */ 0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,2,
/* B */ 0,0,1,0,0,0,0,2,0,0,0,0,0,0,0,2,
/* C */ 0,0,0,0,0,0,0,2,0,0,0,3,0,0,0,2,
/* D */ 0,0,1,0,0,0,0,2,0,0,1,3,0,0,0,2,
/* E */ 0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,2,
/* F */ 0,0,1,0,0,0,0,2,0,0,1,0,0,0,0,2
    } ;

    static char* processorComment[4] =
    {
        (char*)"",
        (char*)"(65C02)",
        (char*)"(65C02: Rockwell & WDC addition)",
        (char*)"(65C02: WDC-only addition)"
    } ;

    char n='-', v='-', b='-', d='-', i='-', z='-', c='-' ;
    if (p & N) n = 'N' ;
    if (p & V) v = 'V' ;
    if (p & B) b = 'B' ;
    if (p & D) d = 'D' ;
    if (p & I) i = 'I' ;
    if (p & Z) z = 'Z' ;
    if (p & C) c = 'C' ;

    sprintf (buffer, "A=%2.2x X=%2.2x Y=%2.2x S=%2.2x   P=%2.2x %c%c.%c%c%c%c%c   ",
                     regs.Aregister, regs.Xregister, regs.Yregister, regs.Sptr,   regs.Pstat, n, v, b, d, i, z, c) ;
    char* comment = processorComment[processorType[regs.opcode]] ;
    strcat (buffer, comment) ;

}


void Machine::printInstruction (quint8 opcode, quint8 c1, quint8 c2)
{
    char buffer1[128], buffer2[128] ;

    m_disassembler->sprint_6502_instruction (m_savedPC, opcode, c1, c2, buffer1) ;
    sprint_6502_registers (m_registers, buffer2) ;
    printf ("%s%s\n", buffer1, buffer2) ;
}


void Machine::enableHistory (bool flag)
{
    m_dumpHistory = flag ;
}


void Machine::dumpHistory (void)
{
    char buffer1[128], buffer2[128] ;
    int nRecorededLines ;
    int nLinesToDump ;

    if (m_historyOldest < m_historyNewest) nRecorededLines = m_historyNewest - m_historyOldest ;
    else                                   nRecorededLines = HISTORY_LEN ;

    nLinesToDump = m_requestedDumpLines ;
    if (nRecorededLines < m_requestedDumpLines) nLinesToDump = nRecorededLines ;

    int historyIndex = m_historyNewest - nLinesToDump ;
    if (historyIndex < 0) historyIndex = HISTORY_LEN + historyIndex ;

    for (int i=0; i<nLinesToDump; i++) {
        ProcessorState p = m_history[historyIndex].pState ;
        quint16 pc    = p.pc.pc_16 ;
        quint8 opcode = p.opcode ;
        quint8 c1     = m_history[historyIndex].c1 ;
        quint8 c2     = m_history[historyIndex].c2 ;
        m_disassembler->sprint_6502_instruction (pc, opcode, c1, c2, buffer1) ;
        sprint_6502_registers (p, buffer2) ;
        printf ("%s%s\n", buffer1, buffer2) ;
        fflush(stdout) ;
        if (++historyIndex >= HISTORY_LEN) historyIndex = 0 ;
        if (historyIndex == m_historyNewest) break ;
    }
}


void Machine::clearCPUWait (void)
{
    m_waitForCPUTimer = false ;
}


//  --------------------------------------------------------------------------
//
//          This is the function which 'executes' 65C02 instructions.
//          It is run as a thread from MainWindow::MainWindow
//
//  --------------------------------------------------------------------------


void Machine::run (void)
{

    RdCXROM = ON ;
    m_halted = false ;
    m_previousCycles = 0 ;
    m_previousUsecs = microseconds() ;
    m_waitForCPUTimer = false ;

#ifdef Q_OS_WINDOWS                  // This bit may or may not be necessary...
    if (SetPriorityClass (GetCurrentProcess(), REALTIME_PRIORITY_CLASS)) {
        bool ret = SetThreadPriority (GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL) ;
        int priority = GetThreadPriority(GetCurrentThread());
    }

    m_syncTimerHandle = CreateWaitableTimer (NULL, false, NULL) ;  // We must use a timer in Windows,
    LARGE_INTEGER firstTime = { 0,0 } ;                            // as sleep functions are too coarse
    bool ret = SetWaitableTimer (m_syncTimerHandle, &firstTime, 1, NULL, NULL, false) ;
#endif


//  define "TIME_TEST" to allow inspection of the instruction-loop timing; times written to stdout.
// #define TIME_TEST

#ifdef TIME_TEST
    quint64 testPreviousTime = microseconds() ;
    quint64 testDeltaTime = 0 ;
    quint64 testTime ;
    quint64 testK = 1e6 ;
#endif

#ifdef Q_OS_WINDOWS
    const quint64 SLICECYCLES = 1030;
#else
    const quint64 SLICECYCLES = 1000 ;
#endif


//  -------------------------------------------------------------------------------------
//     Main loop to execute one 65C02 instruction each time through; repeats forever...
//  -------------------------------------------------------------------------------------

    for (;;) {

#ifdef TIME_TEST
    if ((m_nCycles%testK) < 9) {
        testTime = microseconds() ;
        testDeltaTime = testTime - testPreviousTime ;
        testPreviousTime = testTime ;
        if (testDeltaTime > 100) {
            printf("testDeltaTime = %lli\n", testDeltaTime) ;
            fflush (stdout) ;
        }
    }
#endif

        // Throttle speed to an average of 1 MHz, more or less...
        // (Note that storing microseconds as 64 bits is good for 500,000+ years)

        quint64 deltaCycles = m_nCycles - m_previousCycles ;

#ifdef Q_OS_WINDOWS
        if (deltaCycles > SLICECYCLES) {
            m_previousCycles = m_nCycles ;
            WaitForSingleObject (m_syncTimerHandle, INFINITE) ;
        }
#else
        if (deltaCycles > SLICECYCLES) {
            m_previousCycles = m_nCycles ;
            quint64 now = microseconds() ;
            quint64 nextSlice = now + SLICECYCLES - (now % SLICECYCLES) ;
            while (microseconds() < nextSlice) usleep(5) ; // Keep sleeping until it's time to execute some more 6502 code
        }
#endif

        // See if we hit a trap.

        bool historyHasBeenDumped = false ;

        for (int i=0; i<4; i++) {
            if (m_trapPointSet[i] && (m_savedPC == m_trapPointAddr[i])) {
                m_halted = true ;                // ... yep, we hit one.
                QString trapNo ;
                printf ("\n --- Trap %i at %4.4X hit\n", i+1, m_savedPC) ;
                m_trapTraceDialogPointer->setHalted (true) ;
                fflush (stdout) ;
                if (m_dumpHistory) {             // Dump history ?
                    printf (" --- History dump follows:\n\n") ;
                    dumpHistory() ;
                    historyHasBeenDumped = true ;
                    fflush (stdout) ;
                    m_previousUsecs = microseconds() ;
                } else {
                    printf ("\n") ;
                }
                break ;
            }
        }

        if (m_singleStepPhase == SET) {
            m_singleStepPhase = ACK ;
        }

        if (historyHasBeenDumped == false) {
            if (m_halted || m_singleStepPhase == ACK) {
                quint8 opcode = fetch (m_savedPC) ;
                quint8 c1     = fetch (m_savedPC+1) ;
                quint8 c2     = fetch (m_savedPC+2) ;
                printInstruction (opcode, c1, c2) ;
            }
        }
        historyHasBeenDumped = false ;

        // If the CPU is not running, sit in a wait loop.

        while ((m_powerIsOn == false)
            ||  m_halted
            || (m_singleStepPhase == ACK))
        {
            usleep (50000) ;
            m_previousUsecs = microseconds() ;
        } ;

        // Prepare to echo screen text to the terminal or to a file
        // if we have requested this in the 'preferences' menu

        bool echo ;
        if (Rd80COL) echo = (PC == 0xc307) ; //  XXXXXXXXXXXXX  This will work only on AppleIIe rom.  XXXXXXXXX  FIXME
        else         echo = (PC == 0xfdf0) ;

        if (echo) {
            const char c = Areg & 0x7f ;
            static const char CR  = 0x0d ;
            static const char LF  = 0x0a ;
            if (m_echoToTerminal) {     //   Echo screen character to terminal?
                    printf ("%c", c) ;
                    if (c == CR) printf ("%c", LF) ;
                    fflush (stdout) ;
            }

            if (m_echoingToFile) {        //   Echo screen character to a file?
                m_textEchoFile->write (&c, 1) ;
                if (c == CR) m_textEchoFile->write (&LF, 1) ;
                m_textEchoFile->flush() ;
            }
        }

//  ------------------------------------------------------------------------------------------------
//     Finally!  We arrive at the 256-way 'switch' statement which executes a single 65C02 opcode.
//  ------------------------------------------------------------------------------------------------

        quint8 c ;
        quint16 p=0, s=0 ;

        m_savedPC = PC ;

        m_registers.opcode = fetch(PC++) ;
        switch (m_registers.opcode) {
            case 0x00:        // BRK
                PC++ ;
                Sreg &= 0xff ;
                store (PCHI, 0x100|Sreg--) ;
                Sreg &= 0xff ;
                store (PCLO, 0x100|Sreg--) ;
                Sreg &= 0xff ;
                store (Preg, 0x100|Sreg--) ;
                Sreg &= 0xff ;
                PCLO = fetch(IRQ) ;
                PCHI = fetch(IRQ+1) ;
                Preg |= (UN | B | I) ;
                Preg &= (D ^ 0xff); // This is a 65c02 convention; the 'D' flag is cleared on BRK.
                cycles(7) ;         // NMOS 6502 does not do this.
                break ;
            case 0x01:        // ORA (zero page,X)
                INDIRECTX(p) ;
                Areg |= fetch(p) ;
                STATNZ(Areg) ;
                cycles(6) ;
                break ;
            case 0x02:        // Unimplemented opcode of 65C02.  See the following URL; the tests in that file
                PC++ ;        // can't be passed without acting as if opcode $02 is 2-byte "NOP". (Same as several others)
// https://raw.githubusercontent.com/Klaus2m5/6502_65C02_functional_tests/master/bin_files/65C02_extended_opcodes_test.lst   
                UNIMPLEMENTED
                break ;
            case 0x03:
                break ;
            case 0x04:        // TSB zero page        * 65C02 (Rockwell and WDC only) *
                p = fetch(PC++) ;
                TSB(p)
                cycles(5) ;
                break ;
            case 0x05:        // ORA zero page
                p = fetch(PC++) ;
                Areg |= fetch(p) ;
                STATNZ(Areg) ;
                cycles(3) ;
                break ;
            case 0x06:        // ASL zero page
                p = fetch(PC++) ;
                ASLMEM(p) ;
                cycles(5) ;
                break ;
            case 0x07:        // RMB0                 * 65C02 (Rockwell and WDC only) *
                RMB(0xfe) ;
                cycles(5) ;
                break ;
            case 0x08:        // PHP
                Sreg &= 0xff ;
                store (Preg|UN|B, 0x100|Sreg--) ;
                Sreg &= 0xff ;
                cycles(3) ;
                break ;
            case 0x09:        // ORA immediate
                Areg |= fetch(PC++) ;
                STATNZ(Areg) ;
                cycles(2) ;
                break ;
            case 0x0a:        // ASL accumulator
                Areg <<= 1 ;
                STATNZC(Areg) ;
                Areg &= 0xff ;
                cycles(2) ;
                break ;
            case 0x0b:
                break ;
            case 0x0c:        // TSB absolute         * 65C02 (Rockwell and WDC only) *
                ABSOLUT(p) ;
                TSB(p) ;
                cycles(6) ;
                break ;
            case 0x0d:        // ORA absolute
                ABSOLUT(p) ;
                Areg |= fetch(p) ;
                STATNZ(Areg) ;
                cycles(4) ;
                break ;
            case 0x0e:        // ASL absolute
                ABSOLUT(p) ;
                ASLMEM(p) ;
                cycles(6) ;
                break ;
            case 0x0f:        // BBR0                 * 65C02 (Rockwell and WDC only) *
                BRBIT((zpByte&0x01)==0)
                break ;

//   --------------------------------------------

            case 0x10:        // BPL
                BRADDR(p) ;
                BRANCH((Preg&N)==0) ;
                break ;
            case 0x11:        // ORA (zero page),Y
                INDIRECTY(p)
                Areg |= fetch(p) ;
                STATNZ(Areg) ;
                cycles(5) ;
                break ;
            case 0x12:        // ORA (zero page)      * 65C02 *
                INDIRECTZP(p)
                Areg |= fetch(p) ;
                STATNZ(Areg) ;
                cycles(5) ;
                break ;
            case 0x13:        //  --- Unimplemented opcode 
                UNIMPLEMENTED
                cycles(1) ;
                break ;
            case 0x14:        // TRB zero page        * 65C02 (Rockwell and WDC only) *
                p = fetch(PC++) ;
                TRB(p) ;
                cycles(5) ;
                break ;
            case 0x15:        // ORA zero page,X
                ZEROPAGEX(p) ;
                Areg |= fetch(p) ;
                STATNZ(Areg) ;
                cycles(4) ;
                break ;
            case 0x16:        // ASL zero page,X
                ZEROPAGEX(p) ;
                ASLMEM(p) ;
                cycles(6) ;
                break ;
            case 0x17:        // RMB1                 * 65C02 (Rockwell and WDC only) *
                RMB(0xfd) ;
                cycles(5) ;
                break ;
            case 0x18:        // CLC
                Preg &= C ^ 0xff ;
                cycles(2) ;
                break ;
            case 0x19:        // ORA absolute,Y
                ABSOLUTY(p) ;
                Areg |= fetch(p) ;
                STATNZ(Areg) ;
                cycles(4) ;
                break ;
            case 0x1a:        // INC/INA/INC A        * 65C02 *
                Areg++ ;
                Areg &= 0xff ;
                STATNZ(Areg) ;
                cycles(2) ;
                break ;
            case 0x1b:
                break ;
            case 0x1c:        // TRB absolute         * 65C02 (Rockwell and WDC only) *
                ABSOLUT(p) ;
                TRB(p) ;
                cycles(6) ;
                break ;
            case 0x1d:        // ORA absolute,X
                ABSOLUTX(p) ;
                Areg |= fetch(p) ;
                STATNZ(Areg) ;
                cycles(4) ;
                break ;
            case 0x1e:        // ASL absolute,X
                ABSOLUTX(p) ;
                ASLMEM(p) ;
                cycles(7) ;
                break ;
            case 0x1f:        // BBR1                 * 65C02 (Rockwell and WDC only) *
                BRBIT((zpByte&0x02)==0)
                break ;

//   --------------------------------------------

            case 0x20:        // JSR
                ABSOLUT(p) ;
                PC -= 1 ;
                store (PCHI, 0x100|Sreg--) ;
                Sreg &= 0xff ;
                store (PCLO, 0x100|Sreg--) ;
                Sreg &= 0xff ;
                PC = p ;
                cycles(6) ;
                break ;
            case 0x21:        // AND (zero page,X)
                INDIRECTX(p) ;
                Areg &= fetch(p) ;
                STATNZ(Areg) ;
                cycles(6) ;
                break ;
            case 0x22:        //  --- Unimplemented opcode  
                UNIMPLEMENTED
                PC++ ; 
                break ;
            case 0x23:        //  --- Unimplemented opcode 
                UNIMPLEMENTED
                cycles(1) ;
                break ;
            case 0x24:        // BIT zero page
                p = fetch(PC++) ;
                BIT(p) ;
                cycles(3) ;
                break ;
            case 0x25:        // AND zero page
                p = fetch(PC++) ;
                Areg &= fetch(p) ;
                STATNZ(Areg) ;
                cycles(3) ;
                break ;
            case 0x26:        // ROL zero page
                p = fetch(PC++) ;
                ROLMEM(p) ;
                cycles(5) ;
                break ;
            case 0x27:        // RMB2                 * 65C02 (Rockwell and WDC only) *
                RMB(0xfb) ;
                cycles(5) ;
                break ;
            case 0x28:        // PLP
                Sreg += 1 ;
                Sreg &= 0xff ;
                Preg = fetch(0x100|Sreg) | UN ;
                cycles(4) ;
                break ;
            case 0x29:        // AND immediate
                Areg &= fetch(PC++) ;
                STATNZ(Areg) ;
                cycles(2) ;
                break ;
            case 0x2a:        // ROL accumulator
                Areg <<= 1 ;
                if (Preg & C) Areg |= 1 ;
                STATNZC(Areg) ;
                Areg &= 0xff ;
                cycles(2) ;
                break ;
            case 0x2b:
                break ;
            case 0x2c:        // BIT absolute
                ABSOLUT(p) ;
                BIT(p) ;
                cycles(4) ;
                break ;
            case 0x2d:        // AND absolute
                ABSOLUT(p) ;
                Areg &= fetch(p) ;
                STATNZ(Areg) ;
                cycles(4) ;
                break ;
            case 0x2e:        // ROL absolute
                ABSOLUT(p) ;
                ROLMEM(p) ;
                cycles(6) ;
                break ;
            case 0x2f:        // BBR2                 * 65C02 (Rockwell and WDC only) *
                BRBIT((zpByte&0x04)==0)
                break ;

//   --------------------------------------------

            case 0x30:        // BMI
                BRADDR(p) ;
                BRANCH((Preg&N)) ;
                break ;
            case 0x31:        // AND (zero page),Y
                INDIRECTY(p)
                Areg &= fetch(p) ;
                STATNZ(Areg) ;
                cycles(5) ;
                break ;
            case 0x32:        // AND (zero page)      * 65C02 *
                INDIRECTZP(p) ;
                Areg &= fetch(p) ;
                STATNZ(Areg) ;
                cycles(6) ;
                break ;
            case 0x33:
                break ;
            case 0x34:        // BIT zero page,X      * 65C02 *
                ZEROPAGEX(p) ;
                BIT(p) ;
                cycles(4) ;
                break ;
            case 0x35:        // AND zero page,X
                ZEROPAGEX(p) ;
                Areg &= fetch(p) ;
                STATNZ(Areg) ;
                cycles(4) ;
                break ;
            case 0x36:        // ROL zero page,X
                ZEROPAGEX(p) ;
                ROLMEM(p) ;
                cycles(5) ;
                break ;
            case 0x37:        // RMB3                 * 65C02 (Rockwell and WDC only) *
                RMB(0xf7) ;
                cycles(5) ;
                break ;
            case 0x38:        // SEC
               Preg|= C ;
                break ;
            case 0x39:        // AND absolute,Y
                ABSOLUTY(p) ;
                Areg &= fetch(p) ;
                STATNZ(Areg) ;
                cycles(4) ;
                break ;
            case 0x3a:        // DEC/DEA/DEC A        * 65C02 *
                Areg-- ;
                Areg &= 0xff ;
                STATNZ(Areg) ;
                cycles(2) ;
                break ;
            case 0x3b:
                break ;
            case 0x3c:        // BIT absolute,X       * 65C02 *
                ABSOLUTX(p) ;
                BIT(p) ;
                cycles(4) ;
                break ;
            case 0x3d:        // AND absolute,X
                ABSOLUTX(p) ;
                Areg &= fetch(p) ;
                STATNZ(Areg) ;
                cycles(4) ;
                break ;
            case 0x3e:        // ROL absolute,X
                ABSOLUTX(p) ;
                ROLMEM(p) ;
                cycles(7) ;
                break ;
            case 0x3f:        // BBR3                 * 65C02 (Rockwell and WDC only) *
                BRBIT((zpByte&0x08)==0)
                break ;

//   --------------------------------------------

            case 0x40:        // RTI
                Sreg += 1 ;
                Sreg &= 0xff ;
                Preg = fetch(0x100|Sreg) ;
                Sreg += 1 ;
                Sreg &= 0xff ;
                PCLO = fetch(0x100|Sreg) ;
                Sreg += 1 ;
                Sreg &= 0xff ;
                PCHI = fetch(0x100|Sreg) ;
                cycles(6) ;
                break ;
            case 0x41:        // EOR (zero page,X)
                INDIRECTX(p) ;
                Areg ^= fetch(p) ;
                STATNZ(Areg) ;
                cycles(6) ;
                break ;
            case 0x42:        //  --- Unimplemented opcode, but the 65C02 skips the following byte.  I think. 
                UNIMPLEMENTED
                PC++ ;
                cycles(2) ;
                break ;
            case 0x43:
                break ;
            case 0x44:        //  --- Unimplemented opcode, but the 65C02 skips the following byte.  I think. 
                UNIMPLEMENTED
                PC++ ;
                cycles(2) ;
                break ;
            case 0x45:        // EOR zero page
                p = fetch(PC++) ;
                Areg ^= fetch(p) ;
                STATNZ(Areg) ;
                cycles(3) ;
                break ;
            case 0x46:        // LSR zero page
                p = fetch(PC++) ;
                LSRMEM(p) ;
                cycles(5) ;
                break ;
            case 0x47:        // RMB4                 * 65C02 (Rockwell and WDC only) *
                RMB(0xef) ;
                cycles(5) ;
                break ;
            case 0x48:        // PHA
                store (Areg, 0x100|Sreg--) ;
                Sreg &= 0xff ;
                cycles(3) ;
                break ;
            case 0x49:        // EOR immediate
                Areg ^= fetch(PC++) ;
                STATNZ(Areg) ;
                cycles(2) ;
                break ;
            case 0x4a:        // LSR accumulator
                Preg &= C ^ 0xff ;
                if (Areg & 1) Preg |= C ;
                Areg >>= 1 ;
                STATNZ(Areg) ;
                cycles(2) ;
                break ;
            case 0x4b:
                break ;
            case 0x4c:        // JMP absolute
                ABSOLUT(p) ;
                PC = p ;
                cycles(3) ;
                break ;
            case 0x4d:        // EOR absolute
                ABSOLUT(p) ;
                Areg ^= fetch(p) ;
                STATNZ(Areg) ;
                cycles(4) ;
                break ;
            case 0x4e:        // LSR absolute
                ABSOLUT(p) ;
                LSRMEM(p) ;
                cycles(6) ;
                break ;
            case 0x4f:        // BBR4                 * 65C02 (Rockwell and WDC only) *
                BRBIT((zpByte&0x10)==0)
                break ;

//   --------------------------------------------

            case 0x50:        // BVC
                BRADDR(p) ;
                BRANCH((Preg&V)==0) ;
                break ;
            case 0x51:        // EOR (zero page),Y
                INDIRECTY(p)
                Areg ^= fetch(p) ;
                STATNZ(Areg) ;
                cycles(5) ;
                break ;
            case 0x52:        // EOR (zero page)      * 65C02 *
                INDIRECTZP(p) ;
                Areg ^= fetch(p) ;
                STATNZ(Areg) ;
                cycles(5) ;
                break ;
            case 0x53:                          // XXXXXXXXXX  overlooked opcode; FIX ME
                break ;
            case 0x54:        //  --- Unimplemented opcode, but the 65C02 skips the following byte.  I think. 
                UNIMPLEMENTED
                PC++ ;
                cycles(2) ;
                break ;
            case 0x55:        // EOR zero page,X
                ZEROPAGEX(p) ;
                Areg ^= fetch(p) ;
                STATNZ(Areg) ;
                cycles(4) ;
                break ;
            case 0x56:        // LSR zero page,X
                ZEROPAGEX(p) ;
                LSRMEM(p)
                cycles(6) ;
                break ;
            case 0x57:        // RMB5                 * 65C02 (Rockwell and WDC only) *
                RMB(0xdf) ;
                cycles(5) ;
                break ;
            case 0x58:        // CLI
                Preg &=  I ^ 0xff ;
                cycles(2) ;
                break ;
            case 0x59:        // EOR absolute,Y
                ABSOLUTY(p) ;
                Areg ^= fetch(p) ;
                STATNZ(Areg) ;
                cycles(4) ;
                break ;
            case 0x5a:        // PHY                  * 65C02 *
                store (Yreg, 0x100|Sreg--) ;
                Sreg &= 0xff ;
                cycles(3) ;
                break ;
            case 0x5b:
                break ;
            case 0x5c:        //  --- Unimplemented opcode, but the 65C02 skips the following two bytes.  I think. 
                UNIMPLEMENTED
                PC += 2 ;
                cycles(8) ;
                break ;
            case 0x5d:        // EOR absolute,X
                ABSOLUTX(p) ;
                Areg ^= fetch(p) ;
                STATNZ(Areg) ;
                cycles(4) ;
                break ;
            case 0x5e:        // LSR absolute,X
                ABSOLUTX(p) ;
                LSRMEM(p)
                cycles(7) ;
                break ;
            case 0x5f:        // BBR5                 * 65C02 (Rockwell and WDC only) *
                BRBIT((zpByte&0x20)==0)
                break ;

//   --------------------------------------------

            case 0x60:        // RTS
                Sreg += 1 ;
                Sreg &= 0xff ;
                PCLO = fetch(0x100|Sreg) ;
                Sreg += 1 ;
                Sreg &= 0xff ;
                PCHI = fetch(0x100|Sreg) ;
                PC += 1 ;
                cycles(6) ;
                break ;
            case 0x61:        // ADC (zero page,X)
                INDIRECTX(p) ;
                addmem(p) ;
                cycles(6) ;
                break ;
            case 0x62:         //  --- Unimplemented opcode, but the 65C02 skips the following byte.  I think. 
                UNIMPLEMENTED
                PC++ ;
                cycles(2) ;
                break ;
            case 0x63:
                break ;
            case 0x64:        // STZ zero page        * 65C02 *
                p = fetch(PC++) ;
                store (0, p) ;
                cycles(2) ;
                break ;
            case 0x65:        // ADC zero page
                p = fetch(PC++) ;
                addmem(p) ;
                cycles(3) ;
                break ;
            case 0x66:        // ROR zero page
                p = fetch(PC++) ;
                RORMEM(p) ;
                cycles(5) ;
                break ;
            case 0x67:        // RMB6                 * 65C02 (Rockwell and WDC only) *
                RMB(0xbf) ;
                cycles(5) ;
                break ;
            case 0x68:        // PLA
                Sreg += 1 ;
                Sreg &= 0xff ;
                Areg = fetch(0x100|Sreg) ;
                Sreg &= 0xff ;
                STATNZ(Areg) ;
                cycles(4) ;
                break ;
            case 0x69:        // ADC immediate ;
                p = PC++ ;
                addmem(p) ;
                cycles(2) ;
                break ;
            case 0x6a:        // ROR accumulator
                Areg &= 0xff ;
                if (Preg & C) Areg |= 0x100 ;
                Preg &= C ^ 0xff ;
                if (Areg & 1) Preg |= C ;
                Areg >>= 1 ;
                STATNZ(Areg) ;
                cycles(2) ;
                break ;
            case 0x6b:
                break ;
            case 0x6c:        // JMP (absolute)
                ABSOLUT(p) ;
                PCLO = fetch(p) ;
                PCHI = fetch(p+1) ;
                cycles(5) ;
                break ;
            case 0x6d:        // ADC absolute
                ABSOLUT(p) ;
                addmem(p) ;
                cycles(4) ;
                break ;
            case 0x6e:        // ROR absolute
                ABSOLUT(p) ;
                RORMEM(p) ;
                cycles(6) ;
                break ;
            case 0x6f:        // BBR6                 * 65C02 (Rockwell and WDC only) *
                BRBIT((zpByte&0x40)==0)
                break ;

//   --------------------------------------------

            case 0x70:        // BVS
                BRADDR(p) ;
                BRANCH(Preg&V) ;
                break ;
            case 0x71:        // ADC (zero page),Y
                INDIRECTY(p) ;
                addmem(p) ;
                cycles(5) ;
                break ;
            case 0x72:        // ADC (zero page)      * 65C02 *
                INDIRECTZP(p)
                addmem(p) ;
                cycles(5) ;
                break ;
            case 0x73:
                break ;
            case 0x74:        // STZ zero page,X      * 65C02 *
                ZEROPAGEX(p) ;
                store (0, p) ;
                cycles(4) ;
                break ;
            case 0x75:        // ADC zero page,X
                ZEROPAGEX(p) ;
                addmem(p) ;
                cycles(4) ;
                break ;
            case 0x76:        // ROR zero page,X
                ZEROPAGEX(p) ;
                RORMEM(p) ;
                cycles(6) ;
                break ;
            case 0x77:        // RMB7                 * 65C02 (Rockwell and WDC only) *
                RMB(0x7f) ;
                cycles(5) ;
                break ;
            case 0x78:        // SEI
                Preg |= I ;
                break ;
            case 0x79:        // ADC absolute,Y
                ABSOLUTY(p) ;
                addmem(p) ;
                cycles(4) ;
                break ;
            case 0x7a:        // PLY                  * 65C02 *
                Sreg += 1 ;
                Sreg &= 0xff ;
                Yreg = fetch(0x100|Sreg) ;
                Sreg &= 0xff ;
                STATNZ(Yreg) ;
                cycles(4) ;
                break ;
            case 0x7b:
                break ;
            case 0x7c:        // JMP (absolute,X)     * 65C02 *
                INDIRECTABSX(p) ;
                PC = p ;
                cycles(6) ;
                break ;
            case 0x7d:        // ADC absolute,X
                ABSOLUTX(p) ;
                addmem(p) ;
                cycles(4) ;
                break ;
            case 0x7e:        // ROR absolute,X
                ABSOLUTX(p) ;
                RORMEM(p) ;
                cycles(7) ;
                break ;
            case 0x7f:        // BBR7                 * 65C02 (Rockwell and WDC only) *
                BRBIT((zpByte&0x80)==0)
                break ;

//   --------------------------------------------

            case 0x80:        // BRA                  * 65C02 *
                BRADDR(p) ;
                PC = p ;
                cycles(6) ;
                break ;
            case 0x81:        // STA (zero page,X)
                INDIRECTX(p) ;
                store (Areg, p) ;
                cycles(6) ;
                break ;
            case 0x82:        //  --- Unimplemented opcode, but the 65C02 skips the following byte.  I think. 
                UNIMPLEMENTED
                PC++ ;
                cycles(2) ;
                break ;
            case 0x83:
                break ;
            case 0x84:        // STY zero page
                p = fetch(PC++) ;
                store (Yreg,p) ;
                cycles(3) ;
                break ;
            case 0x85:        // STA zero page
                p = fetch(PC++) ;
                store (Areg,p) ;
                cycles(3) ;
                break ;
            case 0x86:        // STX zero page
                p = fetch(PC++) ;
                store (Xreg,p) ;
                cycles(3) ;
                break ;
            case 0x87:        // SMB0                 * 65C02 (Rockwell and WDC only) *
                SMB(0x01)
                cycles(5) ;
                break ;
            case 0x88:        // DEY
                Yreg -= 1 ;
                Yreg &= 0xff ;
                STATNZ(Yreg) ;
                cycles(2) ;
                break ;
            case 0x89:        // BIT immediate        * 65C02 *
                s = PC++ ;
                if (Areg & s) Preg |= Z ;          // Note: on a 65C02, immediate-mode BIT can
                else       Preg &= (0xff ^ Z) ; // change the processor status Z bit, only.
                cycles(3) ;
                break ;
            case 0x8a:        // TXA
                Areg = Xreg ;
                STATNZ(Areg) ;
                cycles(2) ;
                break ;
            case 0x8b:
                break ;
            case 0x8c:        // STY absolute
                ABSOLUT(p) ;
                store (Yreg, p) ;
                cycles(4) ;
                break ;
            case 0x8d:        // STA absolute
                ABSOLUT(p) ;
                store (Areg, p) ;
                cycles(4) ;
                break ;
            case 0x8e:        // STX absolute
                ABSOLUT(p) ;
                store (Xreg, p) ;
                cycles(4) ;
                break ;
            case 0x8f:        // BBS0                 * 65C02 (Rockwell and WDC only) *
                BRBIT(zpByte&0x01)
                break ;

//   --------------------------------------------

            case 0x90:        // BCC
                BRADDR(p) ;
                BRANCH((Preg&C)==0) ;
                break ;
            case 0x91:        // STA (zero page),Y
                INDIRECTY(p) ;
                store (Areg, p) ;
                cycles(6) ;
                break ;
            case 0x92:        // STA (zero page)      * 65C02 *
                INDIRECTZP(p) ;
                store (Areg, p) ;
                cycles(5) ;
                break ;
            case 0x93:
                break ;
            case 0x94:        // STY zero page,X
                ZEROPAGEX(p) ;
                store (Yreg,p) ;
                cycles(4) ;
                break ;
            case 0x95:        // STA zero page,X
                ZEROPAGEX(p) ;
                store (Areg,p) ;
                cycles(4) ;
                break ;
            case 0x96:        // STX zero page,Y
                ZEROPAGEY(p) ;
                store (Xreg,p) ;
                cycles(4) ;
                break ;
            case 0x97:        // SMB1                 * 65C02 (Rockwell and WDC only) *
                SMB(0x02)
                cycles(5) ;
                break ;
            case 0x98:        // TYA
                Areg = Yreg ;
                STATNZ(Areg) ;
                cycles(2) ;
                break ;
            case 0x99:        // STA absolute,Y
                ABSOLUTY(p) ;
                store (Areg,p) ;
                cycles(5) ;
                break ;
            case 0x9a:        // TXS
                Sreg = Xreg ;
                break ;
            case 0x9b:
                break ;
            case 0x9c:        // STZ absolute         * 65C02 *
                ABSOLUT(p) ;
                store (0,p) ;
                cycles(4) ;
                break ;
            case 0x9d:        // STA absolute,X
                ABSOLUTX(p) ;
                store (Areg,p) ;
                cycles(5) ;
                break ;
            case 0x9e:        // STZ absolute,X       * 65C02 *
                ABSOLUTX(p) ;
                store (0,p) ;
                cycles(4) ;
                break ;
            case 0x9f:        // BBS1                 * 65C02 (Rockwell and WDC only) *
                BRBIT(zpByte&0x02)
                break ;

//   --------------------------------------------

            case 0xa0:        // LDY immediate
                Yreg = fetch(PC++) ;
                STATNZ(Yreg) ;
                cycles(2) ;
                break ;
            case 0xa1:        // LDA (zero page,X)
                INDIRECTX(p) ;
                Areg = fetch(p) ;
                STATNZ(Areg) ;
                cycles(6) ;
                break ;
            case 0xa2:        // LDX immediate
                Xreg = fetch(PC++) ;
                STATNZ(Xreg) ;
                cycles(2) ;
                break ;
            case 0xa3:
                break ;
            case 0xa4:        // LDY zero page
                p = fetch(PC++) ;
                Yreg = fetch(p) ;
                STATNZ(Yreg) ;
                cycles(3) ;
                break ;
            case 0xa5:        // LDA zero page
                p = fetch(PC++) ;
                Areg = fetch(p) ;
                STATNZ(Areg) ;
                cycles(3) ;
                break ;
            case 0xa6:        // LDX zero page
                p = fetch(PC++) ;
                Xreg = fetch(p) ;
                STATNZ(Xreg) ;
                cycles(3) ;
                break ;
            case 0xa7:        // SMB2                 * 65C02 (Rockwell and WDC only) *
                SMB(0x04)
                cycles(5) ;
                break ;
            case 0xa8:        // TAY
                Yreg = Areg ;
                STATNZ(Yreg) ;
                cycles(2) ;
                break ;
            case 0xa9:        // LDA immediate
                Areg = fetch(PC++) ;
                STATNZ(Areg) ;
                cycles(2) ;
                break ;
            case 0xaa:        // TAX
                Xreg = Areg ;
                STATNZ(Xreg) ;
                cycles(2) ;
                break ;
            case 0xab:
                break ;
            case 0xac:        // LDY absolute
                ABSOLUT(p) ;
                Yreg = fetch(p) ;
                STATNZ(Yreg) ;
                cycles(4) ;
                break ;
            case 0xad:        // LDA absolute
                ABSOLUT(p) ;
                Areg = fetch(p) ;
                STATNZ(Areg) ;
                cycles(4) ;
                break ;
            case 0xae:        // LDX absolute
                ABSOLUT(p) ;
                Xreg = fetch(p) ;
                STATNZ(Xreg) ;
                cycles(4) ;
                break ;
            case 0xaf:        // BBS2                 * 65C02 (Rockwell and WDC only) *
                BRBIT(zpByte&0x04)
                break ;

//   --------------------------------------------

            case 0xb0:        // BCS
                BRADDR(p) ;
                BRANCH(Preg&C) ;
                break ;
            case 0xb1:        // LDA (zero page),Y
                INDIRECTY(p) ;
                Areg = fetch(p) ;
                STATNZ(Areg) ;
                cycles(6) ;
                break ;
            case 0xb2:        // LDA (zero page)      * 65C02 *
                INDIRECTZP(p)
                Areg = fetch(p) ;
                STATNZ(Areg) ;
                cycles(5) ;
                break ;
            case 0xb3:
                break ;
            case 0xb4:        // LDY zero page,X
                ZEROPAGEX(p) ;
                Yreg = fetch(p) ;
                STATNZ(Yreg) ;
                cycles(4) ;
                break ;
            case 0xb5:        // LDA zero page,X
                ZEROPAGEX(p) ;
                Areg = fetch(p) ;
                STATNZ(Areg) ;
                cycles(4) ;
                break ;
            case 0xb6:        // LDX zero page,Y
                ZEROPAGEY(p) ;
                Xreg = fetch(p) ;
                STATNZ(Xreg) ;
                cycles(4) ;
                break ;
            case 0xb7:        // SMB3                 * 65C02 (Rockwell and WDC only) *
                SMB(0x08)
                cycles(5) ;
                break ;
            case 0xb8:        // CLV
                Preg &= V ^ 0xff ;
                cycles(2) ;
                break ;
            case 0xb9:        // LDA absolute,Y
                ABSOLUTY(p) ;
                Areg = fetch(p) ;
                STATNZ(Areg) ;
                cycles(4) ;
                break ;
            case 0xba:        // TSX
                Xreg = Sreg ;
                STATNZ(Xreg) ;
                cycles(2) ;
                break ;
            case 0xbb:
                break ;
            case 0xbc:        // LDY absolute,X
                ABSOLUTX(p) ;
                Yreg = fetch(p) ;
                STATNZ(Yreg) ;
                cycles(4) ;
                break ;
            case 0xbd:        // LDA absolute,X
                ABSOLUTX(p) ;
                Areg = fetch(p) ;
                STATNZ(Areg) ;
                cycles(4) ;
                break ;
            case 0xbe:        // LDX absolute,Y
                ABSOLUTY(p) ;
                Xreg = fetch(p) ;
                STATNZ(Xreg) ;
                cycles(4) ;
                break ;
            case 0xbf:        // BBS3                 * 65C02 (Rockwell and WDC only) *
                BRBIT(zpByte&0x08)
                break ;

//   --------------------------------------------

            case 0xc0:        // CPY immediate
                p = PC++ ;
                COMPARE(Yreg,p) ;
                cycles(2) ;
                break ;
            case 0xc1:        // CMP (zero page,X)
                INDIRECTX(p) ;
                COMPARE(Areg,p) ;
                cycles(6) ;
                break ;
            case 0xc2:       //  --- Unimplemented opcode, but the 65C02 skips the following byte.  I think. 
                UNIMPLEMENTED
                PC++ ;
                cycles(2) ;
                break ;
            case 0xc3:
                break ;
            case 0xc4:        // CPY zero page
                p = fetch(PC++) ;
                COMPARE(Yreg,p) ;
                cycles(3) ;
                break ;
            case 0xc5:        // CMP zero page
                p = fetch(PC++) ;
                COMPARE(Areg,p) ;
                cycles(3) ;
                break ;
            case 0xc6:        // DEC zero page
                p = fetch(PC++) ;
                DECMEM(p) ;
                cycles(5) ;
                break ;
            case 0xc7:        // SMB4                 * 65C02 (Rockwell and WDC only) *
                SMB(0x10)
                cycles(5) ;
                break ;
            case 0xc8:        // INY
                Yreg += 1 ;
                Yreg &= 0xff ;
                STATNZ(Yreg) ;
                cycles(2) ;
                break ;
            case 0xc9:        // CMP immediate
                p = PC++ ;
                COMPARE(Areg,p) ;
                cycles(2) ;
                break ;
            case 0xca:        // DEX
                Xreg -= 1 ;
                Xreg &= 0xff ;
                STATNZ(Xreg) ;
                cycles(2) ;
                break ;
            case 0xcb:        // WAI                  * 65C02 (WDC only)
                m_WAI_executed = true ;
                break ;
            case 0xcc:        // CPY absolute
                ABSOLUT(p) ;
                COMPARE(Yreg,p) ;
                cycles(4) ;
                break ;
            case 0xcd:        // CMP absolute
                ABSOLUT(p) ;
                COMPARE(Areg,p) ;
                cycles(4) ;
                break ;
            case 0xce:        // DEC absolute
                ABSOLUT(p) ;
                DECMEM(p) ;
                cycles(6) ;
                break ;
            case 0xcf:        // BBS4                 * 65C02 (Rockwell and WDC only) *
                BRBIT(zpByte&0x10)
                break ;

//   --------------------------------------------

            case 0xd0:        // BNE
                BRADDR(p) ;
                BRANCH((Preg&Z)==0) ;
                break ;
            case 0xd1:        // CMP (zero page),Y
                INDIRECTY(p) ;
                COMPARE(Areg,p) ;
                cycles(5) ;
                break ;
            case 0xd2:        // CMP (zero page)      * 65C02 *
                INDIRECTZP(p) ;
                COMPARE(Areg,p) ;
                cycles(5) ;
                break ;
            case 0xd3:
                break ;
            case 0xd4:        //  --- Unimplemented opcode, but the 65C02 skips the following byte.  I think. 
                UNIMPLEMENTED
                PC++ ;
                cycles(2) ;
                break ;
            case 0xd5:        // CMP zero page,X
                ZEROPAGEX(p) ;
                COMPARE(Areg,p) ;
                cycles(4) ;
                break ;
            case 0xd6:        // DEC zero page,X
                ZEROPAGEX(p) ;
                DECMEM(p) ;
                cycles(6) ;
                break ;
            case 0xd7:        // SMB5                 * 65C02 (Rockwell and WDC only) *
                SMB(0x20)
                cycles(5) ;
                break ;
            case 0xd8:        // CLD
                Preg &= D ^ 0xff;
                cycles(2) ;
                break ;
            case 0xd9:        // CMP absolute,Y
                ABSOLUTY(p) ;
                COMPARE(Areg,p) ;
                cycles(4) ;
                break ;
            case 0xda:        // PHX                  * 65C02 *
                store (Xreg, 0x100|Sreg--) ;
                Sreg &= 0xff ;
                cycles(3) ;
                break ;
            case 0xdb:        // STP                  * 65C02 (WDC only)
                m_STP_executed = true ;
                break ;
            case 0xdc:        //  --- Unimplemented opcode, but the 65C02 skips the following two bytes.  I think. 
                UNIMPLEMENTED
                PC += 2 ;
                cycles(2) ;
                break ;
            case 0xdd:        // CMP absolute,X
                ABSOLUTX(p) ;
                COMPARE(Areg,p) ;
                cycles(4) ;
                break ;
            case 0xde:        // DEC absolute,X
                ABSOLUTX(p) ;
                DECMEM(p) ;
                cycles(7) ;
                break ;
            case 0xdf:        // BBS5                 * 65C02 (Rockwell and WDC only) *
                BRBIT(zpByte&0x20)
                break ;

//   --------------------------------------------

            case 0xe0:        // CPX immediate
                p = PC++ ;
                COMPARE(Xreg,p) ;
                cycles(2) ;
                break ;
            case 0xe1:        // SBC (zero page,X)
                INDIRECTX(p) ;
                submem(p) ;
                cycles(6) ;
                break ;
            case 0xe2:       //  --- Unimplemented opcode, but the 65C02 skips the following byte.  I think. 
                UNIMPLEMENTED
                PC++ ;
                cycles(2) ;
                break ;
            case 0xe3:
                break ;
            case 0xe4:        // CPX zero page
                p = fetch(PC++) ;
                COMPARE(Xreg,p) ;
                cycles(3) ;
                break ;
            case 0xe5:        // SBC zero page
                p = fetch(PC++) ;
                submem(p) ;
                cycles(3) ;
                break ;
            case 0xe6:        // INC zero page
                p = fetch(PC++) ;
                INCMEM(p) ;
                cycles(5) ;
                break ;
            case 0xe7:        // SMB6                 * 65C02 (Rockwell and WDC only) *
                SMB(0x40)
                cycles(5) ;
                break ;
            case 0xe8:        // INX
                Xreg += 1 ;
                Xreg &= 0xff ;
                STATNZ(Xreg) ;
                cycles(2) ;
                break ;
            case 0xe9:        // SBC immediate
                p = PC++ ;
                submem(p) ;
                cycles(2) ;
                break ;
            case 0xea:        // NOP
                break ;
            case 0xeb:
                break ;
            case 0xec:        // CPX absolute
                ABSOLUT(p) ;
                COMPARE(Xreg,p) ;  
                cycles(4) ;
                break ;
            case 0xed:        // SBC absolute
                ABSOLUT(p) ;
                submem(p) ;
                cycles(4) ;
                break ;
            case 0xee:        // INC absolute
                ABSOLUT(p) ;
                INCMEM(p) ;
                cycles(6) ;
                break ;
            case 0xef:        // BBS6                 * 65C02 (Rockwell and WDC only) *
                BRBIT(zpByte&0x40)
                break ;

//   --------------------------------------------

            case 0xf0:        // BEQ
                BRADDR(p) ;
                BRANCH(Preg&Z) ;
                break ;
            case 0xf1:        // SBC (zero page),Y
                INDIRECTY(p) ;
                submem(p) ;
                cycles(5) ;
                break ;
            case 0xf2:        // SBC (zero page)      * 65C02 *
                INDIRECTZP(p) ;
                submem(p) ;
                cycles(5) ;
                break ;
            case 0xf3:
                break ;
            case 0xf4:        //  --- Unimplemented opcode, but the 65C02 skips the following byte.  I think. 
                UNIMPLEMENTED
                PC++ ;
                cycles(2) ;
                break ;
            case 0xf5:        // SBC zero page,X
                ZEROPAGEX(p) ;
                submem(p) ;
                cycles(4) ;
                break ;
            case 0xf6:        // INC zero page,X
                ZEROPAGEX(p) ;
                INCMEM(p) ;
                cycles(6) ;
                break ;
            case 0xf7:        // SMB7                 * 65C02 (Rockwell and WDC only) *
                SMB(0x80)
                cycles(5) ;
                break ;
            case 0xf8:        // SED
                Preg |= D ;
                break ;
            case 0xf9:        // SBC absolute,Y
                ABSOLUTY(p) ;
                submem(p) ;
                cycles(4) ;
                break ;
            case 0xfa:        // PLX                  * 65C02 *
                Sreg += 1 ;
                Sreg &= 0xff ;
                Xreg = fetch(0x100|Sreg) ;
                STATNZ(Xreg) ;
                cycles(4) ;
                break ;
            case 0xfb:
                break ;
            case 0xfc:        //  --- Unimplemented opcode, but the 65C02 skips the following two bytes.  I think. 
                UNIMPLEMENTED
                PC += 2 ;
                cycles(2) ;
                break ;
            case 0xfd:        // SBC absolute,X
                ABSOLUTX(p) ;
                submem(p) ;
                cycles(4) ;
                break ;
            case 0xfe:        // INC absolute,X
                ABSOLUTX(p) ;
                INCMEM(p) ;
                cycles(7) ;
                break ;
            case 0xff:        // BBS7                 * 65C02 (Rockwell and WDC only) *
                BRBIT(zpByte&0x80)
                break ;
            default:
                break ;
        }

        Preg |= UN ;    // status bit 5 is unused, and always set.

        //  Save processor state in the trace queue for possible history dumps on traps.

        m_history[m_historyNewest].pState   = m_registers ;
        m_history[m_historyNewest].pState.pc.pc_16 = m_savedPC ;  // (We have to correct the current program counter)
        m_history[m_historyNewest].c1 = fetch (m_savedPC+1) ;     // (And save the two possible operand bytes, since memory)
        m_history[m_historyNewest].c2 = fetch (m_savedPC+2) ;     // (may be mapped differently when history is dumped.)
        if (++m_historyNewest >= HISTORY_LEN) m_historyNewest = 0 ;
        if (m_historyNewest == m_historyOldest) {
            if (++m_historyOldest >= HISTORY_LEN) m_historyOldest = 0 ;
        }
        
        if (m_trace && (m_savedPC >= m_tracelo) && (m_savedPC <= m_tracehi)) {
            quint8 opcode = fetch (m_savedPC) ;
            quint8 c1     = fetch (m_savedPC+1) ;
            quint8 c2     = fetch (m_savedPC+2) ;
            printInstruction (opcode, c1, c2) ;
            fflush(stdout) ;
        }

    }
}


ProcessorState* Machine::processorState (void)
{
   return &m_registers ;
}


quint16 Machine::savedPC (void)
{
    return m_savedPC ;
}

