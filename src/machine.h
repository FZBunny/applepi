/*****************************************************************************

    File: "machine.h"

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



#ifndef MACHINE_H
#define MACHINE_H


#ifndef Q_OS_WINDOWS
#   include <sys/time.h>
#endif

#include <QThread>
#include <QFile>

#include "defs.h"
#include "mainwindow.h"

#include "disassembler.h"
#include "floppy_controller.h"
#include "hd_controller.h"
#include "gamepad.h"
#include "printer.h"
#include "traptrace.h"
#include "watchaddr.h"
#include "usleep.h"

// #include "tape.h"
// #include "analog_input.h"
#ifdef Q_OS_WINDOWS
    #include "win-speaker.h"
#else
    #include "lin-speaker.h"
#endif

typedef struct ProcessorState {
    union {
        quint8   pc_8[2] ;
        quint16  pc_16 ;
    } pc ;
    quint8  opcode ;        // Not a register, but included for coding convenience
    quint16 Aregister ;
    quint16 Xregister ;
    quint16 Yregister ;
    quint16 Sptr ;
    quint16 Pstat ;
} ProcessorState ;


typedef struct HistoryEntry {
    ProcessorState pState ;
    quint8 c1 ;             // The two bytes following the opcode; these are saved because the
    quint8 c2 ;             // memory mapping may be different when the history is dumped.
} HistoryEntry ;


//       6502 processor status bits (in register "P")

static const quint8 N  = 0x80 ;  // Negative
static const quint8 V  = 0x40 ;  // oVerflow
static const quint8 UN = 0x20 ;  // ... bit 5 is UNused, and always set to '1'.
static const quint8 B  = 0x10 ;  // Break
static const quint8 D  = 0x08 ;  // Decimal mode
static const quint8 I  = 0x04 ;  // Interrupt disable
static const quint8 Z  = 0x02 ;  // Zero result
static const quint8 C  = 0x01 ;  // Carry

#define PC   m_registers.pc.pc_16
#define PCLO m_registers.pc.pc_8[0]
#define PCHI m_registers.pc.pc_8[1]

#define Areg m_registers.Aregister
#define Xreg m_registers.Xregister
#define Yreg m_registers.Yregister

#define Sreg m_registers.Sptr
#define Preg m_registers.Pstat

#define W80STOROFF  m_ss[0x000]
#define W80STORON   m_ss[0x001]
#define RdMAINRAM   m_ss[0x002]
#define RdCARDRAM   m_ss[0x003]
#define WRMAINRAM   m_ss[0x004]
#define WRCARDRAM   m_ss[0x005]
#define RdBNK2      m_ss[0x011]
#define RdLCRAM     m_ss[0x012]
#define RdRAMRD     m_ss[0x013]
#define RdRAMWRT    m_ss[0x014]
#define RdCXROM     m_ss[0x015]
#define RdALTZP     m_ss[0x016]
#define RdC3ROM     m_ss[0x017]
#define Rd80STORE   m_ss[0x018]
#define RdVBL       m_ss[0x019]
#define RdTEXT      m_ss[0x01a]
#define RdMIXED     m_ss[0x01b]
#define RdPAGE2     m_ss[0x01c]
#define RdHIRES     m_ss[0x01d]
#define RdALTCHAR   m_ss[0x01e]
#define Rd80COL     m_ss[0x01f]
#define TAPEOUT     m_ss[0x020]
#define NEWVIDEO    m_ss[0x029]
#define TAPEIN      m_ss[0x060]
#define SW3IN       m_ss[0x060]
#define SW0IN       m_ss[0x061]
#define SW1IN       m_ss[0x062]
#define SW2IN       m_ss[0x063]
#define AI0         m_ss[0x064]
#define AI1         m_ss[0x065]
#define AI2         m_ss[0x066]
#define AI3         m_ss[0x067]
#define STATEREG    m_ss[0x068]
#define AIRESET     m_ss[0x070]
#define BANKSEL     m_ss[0x073]
#define RdDBLHIRES  m_ss[0x07f]

#define IDBYTE      m_rom[0xfbb3]


#define STATNZ(m) Preg &= (N | Z) ^ 0xff ;\
                  if (m & N) Preg |= N ;\
                  if (m == 0) Preg |= Z ;

#define STATNZC(m) Preg &= (N | Z | C) ^ 0xff ;\
                   if (m & 0x100) Preg |= C ;\
                   if ((m & 0xff)==0) Preg |= Z ;\
                   if (m & N) Preg |= N ;

#define STATNZV(m) Preg &= (N | Z | V) ^ 0xff ;\
                   if (m & 0x40) Preg |= V ;\
                   if ((m & 0xff)==0) Preg |= Z ;\
                   if (m & 0x80) Preg |= N ;

#define BRADDR(p) p = fetch(PC) ;\
                  if (p & 0x80) {\
                      p |= 0xff00 ;\
                  }\
                  p += PC + 1 ;

#define BRBIT(test) {\
                        quint16 zpAddr = fetch(PC++) ;\
                        quint8  zpByte = fetch(zpAddr) ;\
                        quint8  offset = fetch(PC++) ;\
                        if (test) {\
                            if (offset & 0x80) {\
                                offset |= 0xff00 ;\
                            }\
                            PC += offset ;\
                        }\
                        cycles(5) ;\
                    }

#define BRANCH(test) {\
                         quint16 s ;\
                         s = fetch(PC++) ;\
                         if (test) {\
                             if (s & 0x80) {\
                                 s |= 0xff00 ;\
                             }\
                             PC += s ;\
                             cycles(3) ;\
                         } else {\
                             cycles(2) ;\
                         }\
                     }

#define RMB(mask) p = fetch(PC++) ;\
                  s = fetch(p) & mask ;\
                  store (s, p) ;\
                  cycles(5) ;

#define TSB(p)\
                s = fetch(p) ;\
                c = Areg & s ;\
                s |= Areg ;\
                store (s, p) ;\
                if (c == 0) Preg = Preg | Z ;\
                else        Preg = Preg & (Z ^ 0xff) ;

#define TRB(p)\
                s = fetch(p) ;\
                c = Areg & s ;\
                s &= (Areg ^ 0xff) ;\
                store (s, p) ;\
                if (c == 0) Preg = Preg | Z ;\
                else        Preg = Preg & (Z ^ 0xff) ;


#define SMB(mask) p = fetch(PC++) ;\
                  s = fetch(p) | mask ;\
                  store (s, p) ;\
                  cycles(5) ;

#define ROLMEM(p) {\
                      quint16 m, oldC, newC ;\
                      oldC = Preg & C ;\
                      Preg &= C ^ 0xff ;\
                      m = fetch(p) ;\
                      newC = m & 0x80 ;\
                      m <<= 1 ;\
                      if (oldC) m |= 0x01 ;\
                      store(m,p) ;\
                      STATNZ(m) ;\
                      if (newC) Preg |= C ;\
                  }

#define RORMEM(p) {\
                      quint16 m, oldC, newC ;\
                      oldC = Preg & C ;\
                      Preg &= C ^ 0xff ;\
                      m = fetch(p) ;\
                      newC = m & 0x01 ;\
                      m >>= 1 ;\
                      if (oldC) m |= 0x80 ;\
                      store (m,p) ;\
                      STATNZ(m) ;\
                      if (newC) Preg |= C ;\
                  }

#define ASLMEM(p) {\
                      quint16 s ;\
                      s = fetch(p) ;\
                      s <<= 1 ;\
                      store (s,p) ;\
                      STATNZC(s) ;\
                  }

#define LSRMEM(p) {\
                      quint16 s ;\
                      Preg &= C ^ 0xff ;\
                      s = fetch(p) ;\
                      if (s & 1) Preg |= C ;\
                      s >>= 1 ;\
                      store(s,p) ;\
                      STATNZ(s) ;\
                  }

#define INCMEM(p) {\
                      quint16 s ;\
                      s = fetch(p) ;\
                      s += 1 ;\
                      s &= 0xff ;\
                      store (s,p) ;\
                      STATNZ(s) ;\
                  }

#define DECMEM(p) {\
                      quint16 s ;\
                      s = fetch(p) ;\
                      s -= 1 ;\
                      s &= 0xff ;\
                      store (s,p) ;\
                      STATNZ(s) ;\
                  }

#define COMPARE(reg,p)  {\
                            quint16 s ;\
                            s = fetch(p) ;\
                            Preg &=  (Z | N | C) ^ 0xff ;\
                            if (reg == s)  Preg |= Z ;\
                            if (s <= reg)  Preg |= C ;\
                            s = reg - s ;\
                            if (s & 0x80) Preg |= N ;\
                        }

#define BIT(p) s = fetch(p) ;\
               Preg &= (N | V | Z) ^ 0xff ;\
               Preg |= s & N ;\
               Preg |= s & V ;\
               if ((Areg & s) == 0) Preg |= Z ;

// ------------------------------------------------------

#define ZEROPAGE(p) p=fetch(PC++);

#define ABSOLUT(p)  p = fetch(PC++) ;\
                    p |= fetch(PC++) << 8 ;

#define ABSOLUTX(p) p = fetch(PC++) ;\
                    p |= fetch(PC++) << 8 ;\
                    p += Xreg & 0xff ;

#define ABSOLUTY(p) p = fetch(PC++) ;\
                     p |= fetch(PC++) << 8 ;\
                     p += Yreg & 0xff ;

#define INDIRECTZP(p) {\
                         quint16 s ;\
                         s = fetch(PC++) ;\
                         p = fetch(s) ;\
                         p |= fetch(s+1) << 8 ;\
                     }

#define INDIRECTABS(p) {\
                         quint16 s ;\
                         p = fetch(PC++) ;\
                         p |= fetch(PC++) << 8 ;\
                       }

#define INDIRECTABSX(p) {\
                         quint16 s ;\
                         s = fetch(PC++) ;\
                         s |= fetch(PC++) << 8 ;\
                         s += Xreg ;\
                         p = fetch(s) ;\
                         p |= fetch(s+1) << 8 ;\
                       }

#define INDIRECTX(p) {\
                         quint16 s ;\
                         s = fetch(PC++) ;\
                         s = (s + Xreg) & 0xff ;\
                         p = fetch(s) ;\
                         p |= fetch(s+1) << 8 ;\
                     }

#define INDIRECTY(p) {\
                         quint16 s ;\
                         s = fetch(PC++) ;\
                         p = fetch(s) ;\
                         p |= fetch(s+1) << 8;\
                         p += Yreg ;\
                     }

#define ZEROPAGEX(p) p = fetch(PC++) ;\
                     p = (p + Xreg) & 0xff ;

#define ZEROPAGEY(p) p = fetch(PC++) ;\
                     p = (p + Yreg) & 0xff ;


#define NMI   0xfffa
#define RESET 0xfffc
#define IRQ   0xfffe

#define ON  0x80
#define OFF 0

#define FAKE_BRK 0xff

#define HISTORY_LEN 100000

// used for single-step:
#define CLR 0
#define SET 1
#define ACK 2

class MainWindow ;
//class FloppyDiskController ;
//class HdController ;
//class Tape ;
class TrapTrace ;
class WatchAddr ;


// ====================================

#include "traptrace.h"
//#include "watchaddr.h"

class Machine : public QThread
{

public:

    Machine (MainWindow* parent) ;
    ~Machine() {} ;

    void initialize (bool powerIsOn) ;

    void turnOn (void) ;
    void turnOff (void) ;
    void reset (void) ;
    void shutDown(void) ;
    void clearCPUWait (void) ;

    void setX (quint8 x) ;
    void setY (quint8 y) ;

    bool    powerIsOn (void) ;
    quint64 getCycles (void) ;

    HdController*  m_hardDrive ;
    Printer*       m_printer ;

    void singleStep (void) ;
    void runFree    (void) ;
    void halt       (void) ;

    void performIRQ (void) ;
    
    bool traceIsOn (void) ;
    void enableTrace (bool on) ;
    void setTraceStart (quint16 loAddress) ;
    void setTraceEnd (quint16 hiAddress) ;
    void setTrap (uint trapIndex, QString address, bool flag) ;
    void setTrapTraceDialogPointer (TrapTrace *trapTraceDialog) ;
    void setWatch (uint watchIndex, QString address, bool flag) ;
    void setWatchAddrDialogPointer (WatchAddr *WatchAddrDialog) ;
    void setLines2Dump (QString nLines) ;
    void enableHistory (bool flag) ;

    void ss_setFetchSnoop (bool flag) ;
    void ss_setStoreSnoop (bool flag) ;

    qint8  fetchFloppy_5_25Inch (int loNibble) ;
    void   storeFloppy_5_25Inch (quint8 offset, quint8 c) ;

    quint16 getPC (void) ;

    bool toggleEchoToTerminal (void) ;
    bool isEchoingToFile (void) ;
    void echoToFile (QFile* f) ;
    void closeEchoFile (void) ;

    ProcessorState *processorState (void) ;
    quint16 savedPC (void) ;


    FloppyDiskController* m_floppy ;
    
    quint8 fetch (quint16 p) ;            // Hack:  made public to allow use in 'floppy_controller.cpp' XXXXXX Fix me?  Or not? XXXXXX
    void   store (quint8 c, quint16 p) ;  // ... same for 'hd_controller.cpp'

    quint8* lower48k (quint16 p, bool write) ;
    quint8* store_highMem (quint16 p) ;

    bool displayMonoDblHiRes (void) ;

    quint8  m_rom[0x10000] ;    // Our motherboard ROM
    quint8  m_ram[0x10000] ;    // Main RAM
    quint8  m_aux[0x10000] ;    // Auxiliary RAM
    quint8  m_ss[256] ;         // Machine "Soft Switches", found in $C000 - $C0FF

    MainWindow*     m_parent ;

//  -----------------------------------------

private:

//    virtual void *Entry (void) ;

    void run (void) ;
    void cycles (int n) ;

    void addmem (quint16 p) ;
    void submem (quint16 p) ;

    void dumpHistory (void) ;

    quint8  fetch_sspage (quint16 p) ;
    quint8  fetch_ioSpace (quint16 p) ;
    void    ss_fetch_snoop (quint16 p) ;
    quint8* fetch_highMem (quint16 p) ;
    void    fetchFromBankSwitches (quint16 p) ;
    void    storeToBankSwitches (quint16 p) ;

    void    store_ioSpace (quint8 c, quint16 p) ;
    void    store_sspage (quint8 c, quint16 p) ;
    void    ss_store_snoop (quint8, quint16 p) ;
    void    printInstruction (quint8 opcode, quint8 c1, quint8 c2) ;
    void    sprint_6502_registers (ProcessorState& State, char* buffer) ;
    void    initializeRAM (void) ;

    Disassembler*  m_disassembler ;
    quint16       m_savedPC ;

    bool     m_waitForCPUTimer ;
    bool     m_powerIsOn ;
    bool     m_highRamWrite ;       // "high RAM" ($C000-$FFFF) write enabled when m_highRamWrite is true    (or is it $D000-$FFFF ? )
    bool     m_highWritePreset ;    // m_highWritePreset must be true before m_highRamWrite can be set 'true'
    quint64  m_nCycles ;
    quint64  m_previousCycles ;
    quint64  m_loopCounter ;

    bool     m_trace ;
    quint16  m_tracelo ;
    quint16  m_tracehi ;
    bool     m_snoopSSFetches ;
    bool     m_snoopSSStores ;

//    quint64  m_debugFlags ;

    int   m_mouseX, m_mouseMaxX ;
    int   m_mouseY, m_mouseMaxY ;
    int   m_mouseButton[3] ;
#ifdef Q_OS_WINDOWS
    HANDLE  m_syncTimerHandle;
#endif

    int     m_monoDblHiResState ;

    bool    m_echoToTerminal ;
    bool    m_echoingToFile ;
    QFile*  m_textEchoFile ;

    quint64  m_previousUsecs ;

    bool      m_haltedOnTrap ;
//    bool      m_paused ;
    bool      m_WAI_executed ;    // Set to true if "WAI" (wait for interrupt) instruction is executed
    bool      m_STP_executed ;    // Set to true if "STP" (wait for reset) instruction is executed
    quint16   m_trapPointAddr[4] ;
    bool      m_trapPointSet[4] ;
    quint16   m_watchAddr[4] ;
    bool      m_watchAddrSet[4] ;
    bool      m_dumpHistory ;
    int       m_halted ;
    int       m_singleStepPhase ;

    TrapTrace  *m_trapTraceDialogPointer ;
    WatchAddr  *m_watchAddrDialogPointer ;

    ProcessorState  m_registers ;

    int            m_historyOldest ;
    int            m_historyNewest ;
    int            m_requestedDumpLines ;
    HistoryEntry   m_history [HISTORY_LEN] ;

    int            m_romSlot ;            // slot# of active peripheral card; 0 if none enabled.
    quint8*        m_slotRomPointer ;     // pointer to the active peripheral ROM; used for ROM addresses $c800-$cfff

} ;


#endif // MACHINE_H


