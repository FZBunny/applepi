/*****************************************************************************

    File: "hd_controller.cpp"

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


//------------------------------------------------
//  This class allows ProDos disk I/O for both
//  hard-drive images -and- 3.5" floppy images.
//------------------------------------------------


#ifndef Q_OS_WINDOWS
#  include <unistd.h>
#endif
#include <sys/types.h>


#include "defs.h"
#include "machine.h"
#include "hd_controller.h"
//#include "bootblock.h"
//#include "device_roms.h"
#include "debugging_dumps.h"



HdController::HdController (Machine* parent)
{
    m_parent = parent ;

    QString key ;
    CFG->Get ("hd1_volume_path", &m_configuredPath[0]) ;
    CFG->Get ("hd2_volume_path", &m_configuredPath[1]) ;

    if (m_configuredPath[0].length()) open (m_configuredPath[0], 0) ;
    if (m_configuredPath[1].length()) open (m_configuredPath[1], 1) ;
m_p_is_4 = false ;
}


//                            --- ENTRY POINT TO HD CONTROLLER CODE ---
//
// "fetch_HD_ROM" is called from "Machine::fetch_ioSpace" when fetching a byte from our fictitious slot 7 ROM

quint8 HdController::fetch_HD_ROM (int slotNumber, quint8 p)
{
//printf ("1... fetch_HD_ROM p=%2.2x\n", p) ;
    static int NOP = 0xea ;
    static int RTS = 0x60 ;
    int slotAddr = 0xc000 | (slotNumber & 0x07) << 8 ;          // The slot address of our fake ROM
    p &= 0xff ;                                                 // (c700 for slot 7)
    quint8 c = 0 ;
    quint16 entryPoint = 0xc000 | (slotNumber << 8) | 0x10 ;    // Entry point for non-smartport calls
    quint16 dispatchAddr = entryPoint + 3 ;                     // Dispatch address for smartport calls
    ProcessorState* ps = MAC->processorState() ;

    // Ignore 2 fetches after $Cn10 entry; just a side-effect of last
    // few lines of fn. "Machine::run" saving history data.
    if ((MAC->savedPC()==entryPoint) && ((p==0x11) || (p==0x12))) return 0 ;

    switch (p) {
        case 0:                                            // Boot?
            if (MAC->savedPC() == slotAddr) {              // Yes. On boot, read the first block on the hard drive
                offsetKludge() ;                           // into $0800 - 0x9ff...
                readBlock (MAC->m_ram+0x0800, 0, 0) ;
                ps->Xreg  = slotNumber << 4 ;
                ps->pc.pc_16 = 0x0801 ;                    // Then jump to $0801
                c  = NOP ;                                 // (The code on the 1st block takes care of the rest.)
            } else {
                c = 0xa9 ;
            }
            break ;
        case 1:          // ROM code inspects bytes 1, 3, & 5 when searching for a drive to boot from.
            c = 0x20 ;
            break ;
        case 3:          // - ditto
            c = 0x00 ;
            break ;
        case 5:          // - ditto
            c = 0x03 ;
            break ;
        case 7:
            c = 0x3c ;   // ProDOS inspects byte 7; a zero value would indicate we're a "smartport" controller.
            break ;      // '3c' seems to tell it we're a dumb hard drive controller. (And we're dumb as a brick.)
        case 0x10:
            if (MAC->savedPC() == entryPoint) {            // ProDOS call
//printf ("A... fetch_HD_ROM\n") ;
                ps->Areg = IO() ;
//printf ("B... fetch_HD_ROM\n") ;
                if (ps->Areg) ps->Pstat |= C ;
                else          ps->Pstat &= C ^ 0xff ;
                return RTS ;
            } else {
                c = 0 ;
            }
            break ;
        case 0x13:                                         // ProDOS 'smartport' call
            if (MAC->savedPC() == dispatchAddr) { 
        //        smartPort() ;
                c = RTS ;
            } else {
                c = 0 ;
            }
            break ;
        case 0xfc:
            c = getDiskSize(0) ;  // Size of disk, low byte 
            break ;
        case 0xfd:
            c = getDiskSize(1) ;  // Size of disk, high byte 
            break ;
        case 0xfe:  
            c = fetchStatusByteFE() ;
            break ;
        case 0xff:
            c = 0x10 ;
            break ;
        default:
//printf ("2...fetch_HD_ROM, p=%2.2x\n", p) ;
        c = 0 ;
            break ;
    }

    return c ;
} 
/**
fetch_HD_ROM, p=17
fetch_HD_ROM, p=37
fetch_HD_ROM, p=4c
fetch_HD_ROM, p=02
fetch_HD_ROM, p=0b
**/

int HdController::IO (void)
{
//    quint16 buffer = MAC->m_ram[0x45]<<8 | MAC->m_ram[0x44] ;
//    int block  = MAC->m_ram[0x47]<<8 | MAC->m_ram[0x46] ;
//    quint8 operation = MAC->m_ram[0x42] ;
//    if (MAC->m_ram[0x43] & 0x80) m_driveIndex = 1 ;
//    else                         m_driveIndex = 0 ;
//printf ("HdController::IO\n") ;
    quint16 buffer = MAC->fetch(0x45)<<8 | MAC->fetch(0x44) ;
    int block = MAC->fetch(0x47) << 8 | MAC->fetch(0x46) ;
    quint8 operation = MAC->fetch(0x42) ;
    if (MAC->fetch(0x43) & 0x80)  m_driveIndex = 1 ;
    else                          m_driveIndex = 0 ;

//ProcessorState *s = MAC->processorState() ;
//printf ("HdController::IO  PC=%4.4X  operation=%i\n", s->pc.pc_16, operation) ;
    int stat = 0 ;

    switch (operation) {
       case 0:                         // Status
          stat = status() ;
          break ;
       case 1:                         // Read
          m_parent->m_parent->HDActivityStarted (m_driveIndex) ;
          stat = readBlock (buffer, block) ;
//ProcessorState *s = MAC->processorState() ;
//printf ("read: PC=%4.4X  buffer=%4.4x block=%4.4x stat=%i\n", s->pc.pc_16, buffer, block, stat) ;
          break ;
       case 2:                         // Write
          m_parent->m_parent->HDActivityStarted (m_driveIndex) ;
          stat = writeBlock (buffer, block) ;
          break ;
       case 3:                         // Format
          m_parent->m_parent->HDActivityStarted (m_driveIndex) ;
          stat = format() ;
          break ;
       default:
          printf ("*** Internal error;  bad hard-drive operation code: %2.2x\n", operation) ;
          stat = 0x27 ;
          break ;
    }

    return stat ;
}


// This overload of readBlock is called from 'MainWindow::setHDLabel'   
// upon power-on to get the ProDOS disk labels of any disks in the two
// drives, and from 'fetch_HD_ROM' (above), once, upon boot.

int HdController::readBlock (quint8* buffer, int block, int driveIndex)
{
//printf ("three-param readBlock  block=%i, driveIndex=%i\n", block, driveIndex) ;
    if (driveIndex < 0) driveIndex = 0 ;
    if (driveIndex > 1) driveIndex = 1 ;
    m_driveIndex = driveIndex ;
    
    int n = 0 ;
    int stat = 0 ;
    headStepDelay (block) ;

    if (m_file[m_driveIndex].isOpen()) {
        m_file[m_driveIndex].seek (block*BLOCKSIZE + m_offset[m_driveIndex]) ;
        n = m_file[m_driveIndex].read ((char*)buffer, BLOCKSIZE) ;
        if (n != BLOCKSIZE) stat = IOERROR ;
    }
    return stat ;
}


// This overload of readBlock is called from fn. "IO" during nornmal ProDOS I/O.
 
int HdController::readBlock (quint16 addr, int block)
{
//printf ("readBlock blk %i to %4.4X\n", block, addr) ;
//ProcessorState* ps = MAC->processorState() ;
//quint8 Sptr = ps->Sptr ;
//printf ("Sptr=1%2.2x\n", Sptr) ;
//xdump (MAC->m_ram+0x1e0, 0x20, 0x1e0) ;
//printf ("\n") ;

    quint8* p ; 
    if (addr < 0xd000) p = MAC->lower48k (addr, true) ;
    else               p = MAC->store_highMem (addr) ;

    if (p == NULL) {
        fprintf (stderr, "\n*** Error:  Attempting to read a disk block into ROM ***\n\n") ;
        return IOERROR ;
    }

    int n = 0 ;
    int stat = 0 ;
    headStepDelay (block) ;
    if (isOpen()) {
        m_file[m_driveIndex].seek (block*BLOCKSIZE + m_offset[m_driveIndex]) ;
        n = m_file[m_driveIndex].read ((char*)p, BLOCKSIZE) ;
        if (n != BLOCKSIZE) {
            fprintf (stderr, "\n*** I/O error reading block %i from hard drive image.\n", block) ;
            stat = IOERROR ;
        }
    } else {
        stat = IOERROR ;
    }
 //   } else {
 //       memset (MAC->m_ram+addr, 0, BLOCKSIZE) ;
 //       stat = NODEVICE ;
 //   }
    return stat ;
}


bool HdController::open (QString &path, int driveIndex)
{
// qStdOut() << "HdController::open " << path << "; driveIndex " << driveIndex << endl ;
    if (driveIndex < 0) driveIndex = 0 ;
    if (driveIndex > 1) driveIndex = 1 ;
    m_driveIndex = driveIndex ;

    m_previousTrack[m_driveIndex] = 0 ;
    m_configuredPath[m_driveIndex] = path ;

    m_fileInfo[m_driveIndex].setFile (path) ;
    if ( ! m_fileInfo[m_driveIndex].exists()) return false ;  // "IsDir()" call doesn't seem to work...XXXXX wxWidgets only: FIXME XXXXX
    if ( ! m_fileInfo[m_driveIndex].isReadable()) return false ;
    m_writeable[m_driveIndex] = m_fileInfo[m_driveIndex].isWritable() ;
    m_fileSize[m_driveIndex] = m_fileInfo[m_driveIndex].size() ;

    QIODevice::OpenMode mode ;
    if (m_writeable[m_driveIndex]) mode = QIODevice::ReadWrite ;
    else                           mode = QIODevice::ReadOnly ;
    m_file[m_driveIndex].setFileName (m_configuredPath[m_driveIndex]) ;
    int ok = m_file[m_driveIndex].open (mode) ;
//printf ("HdController::open    m_configuredPath = '%s', mode=%i  ok=%i\n", m_configuredPath, mode, ok) ;
    offsetKludge() ;

    return ok ;
}


bool HdController::isOpen (void)
{
    return m_file[m_driveIndex].isOpen() ;
}


bool HdController::isOpen (int driveIndex)
{
    return m_file[driveIndex].isOpen() ;
}


int HdController::fetchStatusByteFE (void)
{
    int byte = 0xcf ;
    if (m_file[0].isOpen()) byte |= 0x20 ;
    if (m_file[1].isOpen()) byte |= 0x10 ;

    return byte ;
}


void HdController::close (void)
{
    m_file[m_driveIndex].close() ;
}


void HdController::close (int driveIndex)
{
    if (driveIndex < 0) driveIndex = 0 ;
    if (driveIndex > 1) driveIndex = 1 ;
    m_driveIndex = driveIndex ;
    m_file[m_driveIndex].close() ;
}


int HdController::status (void)        //  (See 'ProDos Tech. Notes', note 21 for status returns)
{
    int stat = 0 ;

    if (MAC->fetch(0x43) & 0x80) m_driveIndex = 1 ;
    else                         m_driveIndex = 0 ;

    if (m_file[m_driveIndex].isOpen() == false) return NODEVICE ;

    else if (m_writeable[m_driveIndex] == false)  stat = WRITE_PROTECTED ; 
    quint64 length = (m_fileInfo[m_driveIndex].size() - m_offset[m_driveIndex]) / BLOCKSIZE  ; // (size in blocks)
    MAC->setX (length & 0xff) ;
    MAC->setY ((length >> 8) & 0xff) ;

//printf ("HdController::status MAC->m_ram[$42]=%2.2x MAC->m_ram[$43]=%2.2x; status=%2.2x\n", MAC->m_ram[0x42], MAC->m_ram[0x43], stat) ;
    return stat ;
}


int HdController::writeBlock (quint16 buffer, int block)
{
    int n = 0 ;
    int stat = 0 ;
    quint8* p ; 
    if (buffer > 0xd000) p = MAC->lower48k (buffer, true) ;
    else                 p = MAC->store_highMem (buffer) ;
    
    if (isOpen()) {
        m_file[m_driveIndex].seek (block*BLOCKSIZE + m_offset[m_driveIndex]) ;
        n = m_file[m_driveIndex].write ((char*)p, BLOCKSIZE) ;
        m_file[m_driveIndex].flush() ;
        if (n != BLOCKSIZE) stat = IOERROR ;
    } else {
        stat = NODEVICE ;
    }

    return stat ;
}


int HdController::format  (void)
{
printf ("*** FORMAT OPERATION NOT IMPLEMENTED ***\n") ;
    return 0 ;                            //  XXXXXXXXXXXXXXXXXXX  What goes here?
}


//  Return the low (if byteNumber=0) or high (if byteNumber=1) byte of the current disk size (in blocks).
//  ProDos occasionally requests this information.

quint8 HdController::getDiskSize (quint8 byteNumber)
{  
    quint64 longSize ;   // (Coded in this eccentric way because g++ kept ) 
    quint8*    p0 ;      // (puking on a simple 'union' statement. WTF?   )
    quint8*    p1 ;

    longSize = (m_fileInfo[m_driveIndex].size() - m_offset[m_driveIndex]) / BLOCKSIZE ;  // (convert bytes size to blocks)
    p0 = (quint8*)&longSize ;
    p1 = p0 ;
    p1++ ;

    quint8 n ;
    if (byteNumber) n = *p1 ;
    else            n = *p0 ;
//printf ("p[%i]=%2.2x\n", byteNumber, n) ;
    return n ;
}


// Sleep the amount of time required to move between tracks.
// (800KiB disks are double sided, with 80 tracks/side,)
// (giving 800 512-byte blocks/side.)

void HdController::headStepDelay (int block)
{
    if (m_fileSize[m_driveIndex] < 90000) { // Is this a 3.5" floppy?
        int newTrack = block/10 ;  // actually, there were 8-12 blocks/track; we just use an average...)
        if (newTrack > 800) newTrack -= 800 ;

        int trackDelta = abs (newTrack-m_previousTrack[m_driveIndex]) ;
        m_previousTrack[m_driveIndex] = newTrack ;

        int ttseekTime = 50 ;      //  Track-to-track seek time  (millisecond number IS JUST A GUESS!) XXXXX
        int totalMsec = ttseekTime * trackDelta ;
        usleep (1000*totalMsec) ;
    } else {
                                   //  Must be a hard drive;  should we insert any delays here? XXXXX
    }
}


//                         An annoying kludge...
//
//  Find the offset, in bytes, to the beginning of the actual disk data
//  contained in the disk image.
//
//  For reasons I cannot comprehend, some hard-drive disk images found 
//  on the web contain mystery junk(?) before the disk boot blocks.
//  The length of this junk seems to be unpredictable.
//  So:  we search for the 3 bytes we expect to see as the 1st 4 bytes
//  the disk:
//
//  01      DB   #$01
//  38      SEC
//  b0 xx   BCS  $xxxx
//
//  The offset of the "01" byte is saved for use when seeking before
//  read & writes:  "seek (block*BLOCKSIZE + m_offset[m_driveIndex])"


void HdController::offsetKludge (void)
{
    quint8 buffer[BLOCKSIZE] ;
    int n = 0 ;

    if (isOpen()) {
        m_file[m_driveIndex].seek (0) ;
        n = m_file[m_driveIndex].read ((char*)buffer, BLOCKSIZE) ;
    }

    for (n=0; n<BLOCKSIZE; n++) {
        if (buffer[n]==0x01 && buffer[n+1]==0x38 && buffer[n+2]==0xb0) break ;
    }

    if (n < BLOCKSIZE) m_offset[m_driveIndex] = n ;
    else               m_offset[m_driveIndex] = 0 ; // If we don't find the starting bytes at all, just assume that it
                                                    // starts with someting else at location 0.  (What else can we do?)
}






// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX  UNFINISHED  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
/***
bool HdController::create (QString &path, int drive, int nBlocks)
{

    m_driveIndex = drive - 1 ;
    if (m_file[m_driveIndex].isOpen()) {
        errno = EBUSY ;
        return errno ;
    }

    m_writeable[m_driveIndex] = true ;

    bool ok = m_file[m_driveIndex].open (QIODevice::NewOnly) ;
    if ( ! ok) return false ;

    m_configuredPath[m_driveIndex] = path ;

    int n = m_file[m_driveIndex].Write (&bootBlock, BLOCKSIZE) ;  // Write the boot bock to block 0
    if (n != BLOCKSIZE) {
        return m_file[m_driveIndex].GetLastError() ;
    }

    quint8 zeros[BLOCKSIZE] ;
    memset (zeros, 0, BLOCKSIZE) ;
    for (int i=0; i<nBlocks-1; i++) {
        m_file[m_driveIndex].Write (zeros, BLOCKSIZE) ;
    }
    m_file[m_driveIndex].Flush() ;
    m_file[m_driveIndex].seek(0) ;
    m_offset[m_driveIndex] = 0 ;
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX  UNFINISHED  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

    return false ;
}
***/


// XXXXXXXXXXXXXXXXXXXXXXX  SMARTPORT IS UNFINISHED XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

//  Perform a "smartport" call for a hard drive or 3.5" floppy drive.
//  Returns "carry" value as a boolean:  false (carry=0) on success, true (carry=1) on error.
//  See "Apple IIgs Firmware Reference", chapter 7.

/***
bool HdController::smartPort (void)
{

    bool status ;
    quint8 paramCount = memory[paramPtr] ;
    quint8 unitNumber = memory[paramPtr+1] ;

//------------------
//    quint8  aLo = MAC->m_ram [ps->Sptr+0x101] ;
//    quint8  aHi = MAC->m_ram [ps->Sptr+0x102] ;
//    quint16 cmdPtr = (aHi<<8) + aLo + 1 ;
//    quint8  pLo = MAC->m_ram [cmdPtr+1] ;
//    quint8  pHi = MAC->m_ram [cmdPtr+2] ;
//    quint16 paramPtr = (pHi<<8) + pLo ;
//    quint8  cmd = MAC->m_ram [cmdPtr] ;
//printf ("fetch_HD_ROM: *smartport* call  MAC->savedPC()=%4.4x cmdPtr=%4.4x cmd=%2.2x  paramPtr=%4.4x\n",  MAC->savedPC(), cmdPtr, cmd, paramPtr) ;
    if (cmd & 0x40) {               // This is an "extended" call.  We don't do extended.
        printf ("*** 'extended' smartPort call made from 0x%4.4x\n", MAC->savedPC()) ;
        ps->Pstat |= C ;
        return BRK ;   // No idea what effect this will have...
    }
//------------------------------

//printf ("HdController::smartPort - cmd=%i; paramCount=%2.2x unitNumber=%2.2x\n", cmd, paramCount, unitNumber) ;

    switch (cmd) {
        case 0:
            status = smartPortStatus (memory, paramPtr) ;   // Status
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
        case 8:
            break ;
        case 9:
            break ;
        default:
            printf ("*** ProDOS \"smart port\" call made with bad command value = 0x%2.2X\n", cmd) ;
            status = true ;
    }

    return status ;

return 0 ;
}
bool HdController::smartPortStatus (quint8* memory, quint16 paramPtr)
{
    quint8 unitNumber = memory[paramPtr+1] ;
    if (unitNumber > 1) unitNumber = 1 ;
    m_driveIndex = unitNumber ;
    quint16 statusListPtr = (memory[paramPtr+3]<<8) + memory[paramPtr+2] ;
    quint8 statusCode = memory[paramPtr+4] ;
//printf ("HdController::smartPortStatus - statusListPtr=%4.4x statusCode=%2.2x\n", statusListPtr, statusCode) ;


    switch (statusCode) {
        case 0:                      // Return status of smartPort "host"
            if (unitNumber == 0) {
                memory[statusListPtr]   = 1 ;     // 1 device connected
                memory[statusListPtr+1] = 0x40 ;  // "No interrupts sent"
            } else {
                memory[statusListPtr] = 0xe0 ;    // Block device | write allowed | read allowed
                if (isOpen(m_driveIndex))         statusListPtr |= 0x40 ;
                if ( ! m_writeable[m_driveIndex]) statusListPtr |= 0x20 ;
            }
            return false ;
            break ;
        case 1:                      // Return DCB
            break ;
        case 2:
            break ;
        case 3:
            break ;
        default:
            printf ("(* HdController::smartPortStatus Illegal status request code: 0x%2.2X\n", statusCode) ;
            return true ;
    }
//    quint8 status = 0xe0 ;   // (bits: 7=block device, 6=write allowed, 5=read allowed)


    return true ;
}

***/





// p. 143

/*****

STATUS Calls
A STATUS call with unit number = $00 and status code = $00 is a request to
return the status of the SmartPort host, as opposed to unit numbers greater
than zero which return the status of individual devices. The number of
devices as well as the current interrupt status is returned. The format of
the status list returned is illustrated in Figure 1.
       +------------------+
Byte 0 | Device Count     |
       +------------------+
Byte 1 | Interrupt Status |
       +------------------+
Byte 2 |     Vendor       |   $0000         Vendor unknown
       +                  +---$0001         Apple Computer, Inc.
Byte 3 |       ID         |   $0002-$FFFF   Third-Party Vendor
       +------------------+
Byte 4 |     Interface    |   _____|___________________|_____
       +                  +--|F|E|D|C|B|A|9|8|7|6|5|4|3|2|1|0|
Byte 5 |      Version     |  |_______|_______________|_______|
       +------------------+  |       |               |
Byte 6 |     Reserved     |  |Major  |    Minor      |$A=Alpha
       +------------------+  |Release|    Release    |$B=Beta
Byte 7 |     Reserved     |                          |$E=Experimental
       +------------------+                          |$0=Final
       
            Figure 1-Host General Status Return Information
            
Stat_list byte 0     Number of devices
          byte 1     Interrupt Status (If bit 6 is set, then no interrupt)
          bytes 2-3  Driver manufacturer (were Reserved prior to May 1988):        
                     $0000       Undetermined
                     $0001       Apple
                     $0002-$FFFF Third-party driver
          bytes 4-5  Interface Version
          bytes 6-7  Reserved (must be $0000)

*****/


