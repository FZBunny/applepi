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
    m_mac = parent ;

    QString key ;
    CFG->Get ("hd1_volume_path", &m_configuredPath[0]) ;
    CFG->Get ("hd2_volume_path", &m_configuredPath[1]) ;

    if (m_configuredPath[0].length()) open (m_configuredPath[0], 0) ;
    if (m_configuredPath[1].length()) open (m_configuredPath[1], 1) ;
}


//                            --- ENTRY POINT TO HD CONTROLLER CODE ---
//
// "fetch_HD_ROM" is called from "Machine::fetch_ioSpace" when fetching a byte from our fictitious slot 7 ROM

quint8 HdController::fetch_HD_ROM (int slotNumber, quint8 p)
{
//printf ("1... fetch_HD_ROM p=%2.2x\n", p) ;
    static int NOP = 0xea ;
    static int RTS = 0x60 ;
    static int SLOOP = 0xfaba ;                               // ROM loop which checks for disk drives to boot from
    int slotAddr = 0xc000 | (slotNumber & 0x07) << 8 ;        // The slot address of our fake ROM
    p &= 0xff ;                                               // (c700 for slot 7)
    quint8 c = 0 ;
    quint16 entryPoint = 0xc000 | (slotNumber << 8) | 0x10 ;  // Entry point for non-smartport calls
    quint16 dispatchAddr = entryPoint + 3 ;                   // Dispatch address for smartport calls
    ProcessorState* ps = m_mac->processorState() ;

    // Ignore 2 fetches after $Cn10 entry; just a side-effect of last
    // few lines of fn. "Machine::run" saving history data.
    if ((m_mac->savedPC()==entryPoint) && ((p==0x11) || (p==0x12))) return 0 ;

    switch (p) {
        case 0:                                              // Boot?
            if (m_mac->savedPC() == slotAddr) {              // Yes; 
                if (isOpen(0)) {                             // Disk in drive?
                    offsetKludge() ;                         // Yes. Read the first block on the hard drive
                    readBlock (m_mac->m_ram+0x0800, 0, 0) ;  // into $0800 - 0x9ff...
                    ps->Xreg  = slotNumber << 4 ;            // Then jump to $0801
                    ps->pc.pc_16 = 0x0801 ;                  // (The code on the 1st block takes care of the rest.)
                    c  = NOP ;
                } else {                                     // No disk in drive; tell ROM to try slot 6.
                    ps->pc.pc_16 = SLOOP ;
                    c = NOP ;
                }
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
//printf ("C707 fetched from %4.4x\n", m_mac->savedPC()) ;
            c = 0x3c ;   // A non-zero tells ProDOS we're a dumb hard drive controller. (And we're dumb as a brick.)
            break ;      // ($3c seems to make some code happy.)
        case 0x10:
            if (m_mac->savedPC() == entryPoint) {          // ProDOS call
                ps->Areg = IO() ;
                if (ps->Areg) ps->Pstat |= C ;
                else          ps->Pstat &= C ^ 0xff ;
                return RTS ;
            } else {
                c = 0 ;
            }
            break ;
        case 0x13:                                         // ProDOS 'smartport' call
            if (m_mac->savedPC() == dispatchAddr) { 
                smartPort() ;
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


int HdController::IO (void)
{
//printf ("HdController::IO\n") ;
    quint16 buffer = m_mac->fetch(0x45)<<8 | m_mac->fetch(0x44) ;
    int block = m_mac->fetch(0x47) << 8 | m_mac->fetch(0x46) ;
    quint8 operation = m_mac->fetch(0x42) ;
    if (m_mac->fetch(0x43) & 0x80)  m_driveIndex = 1 ;
    else                            m_driveIndex = 0 ;

    int stat = 0 ;

    switch (operation) {
       case 0:                         // Status
          stat = status() ;
          break ;
       case 1:                         // Read
          m_mac->m_parent->HDActivityStarted (m_driveIndex) ;
          stat = readBlock (buffer, block) ;
          break ;
       case 2:                         // Write
          m_mac->m_parent->HDActivityStarted (m_driveIndex) ;
          stat = writeBlock (buffer, block) ;
          break ;
       case 3:                         // Format
          m_mac->m_parent->HDActivityStarted (m_driveIndex) ;
          stat = format() ;
          break ;
       default:
          fprintf (stderr, "*** Internal error;  bad hard-drive operation code: %2.2x\n", operation) ;
          stat = 0x27 ;
          break ;
    }

    return stat ;
}


// Return a pointer to a bypte within either m_ram or m_aux,
// depanding on soft-switch settings

quint8* HdController::address (quint16 address, bool write)
{
    quint8* p ;

    if (address < 0xd000) p = m_mac->lower48k (address, write) ;
    else                  p = m_mac->store_highMem (address) ;  

    return p ;
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


// This overload of readBlock is called from fn. "IO" during normal ProDOS I/O.
 
int HdController::readBlock (quint16 addr, int block)
{
    quint8* p = address (addr, true) ;

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
    
    return stat ;
}


int HdController::writeBlock (quint16 buffer, int block)
{
//printf ("HdController::writeBlock buffer=%4.4x block=%4.4x\n", buffer, block) ;
    int n = 0 ;
    int stat = 0 ;

    quint8* p = address (buffer, true) ;
       
    if (isOpen()) {
        m_file[m_driveIndex].seek (block*BLOCKSIZE + m_offset[m_driveIndex]) ;
        n = m_file[m_driveIndex].write ((char*)p, BLOCKSIZE) ;
        m_file[m_driveIndex].flush() ;
        if (n != BLOCKSIZE) stat = IOERROR ;
    } else {
        stat = IOERROR ;
    }

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
    if ( ! m_fileInfo[m_driveIndex].exists()) return false ;
    if ( ! m_fileInfo[m_driveIndex].isReadable()) return false ;
    m_writeable[m_driveIndex] = m_fileInfo[m_driveIndex].isWritable() ;
    m_fileSize[m_driveIndex] = m_fileInfo[m_driveIndex].size() ;

    QIODevice::OpenMode mode ;
    if (m_writeable[m_driveIndex]) mode = QIODevice::ReadWrite ;
    else                           mode = QIODevice::ReadOnly ;
    m_file[m_driveIndex].setFileName (m_configuredPath[m_driveIndex]) ;
    int ok = m_file[m_driveIndex].open (mode) ;
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

    if (m_mac->fetch(0x43) & 0x80) m_driveIndex = 1 ;
    else                           m_driveIndex = 0 ;

    if (m_file[m_driveIndex].isOpen() == false) return NODEVICE ;

    else if (m_writeable[m_driveIndex] == false)  stat = WRITE_PROTECTED ; 
    quint64 length = (m_fileInfo[m_driveIndex].size() - m_offset[m_driveIndex]) / BLOCKSIZE  ; // (size in blocks)
    m_mac->setX (length & 0xff) ;
    m_mac->setY ((length >> 8) & 0xff) ;

    return stat ;
}


int HdController::format  (void)
{
fprintf (stderr, "*** FORMAT OPERATION NOT IMPLEMENTED ***\n") ;
    return 0 ;                            //  XXXXXXXXXXXXXXXXXXX  What goes here?
}


//  Return the low (if byteNumber=0) or high (if byteNumber=1) byte of the current disk size (in blocks).
//  ProDos occasionally requests this information.

quint8 HdController::getDiskSize (quint8 byteNumber)
{  
    quint64  longSize ;   // (Coded in this eccentric way because g++ kept ) 
    quint8*  p0 ;         // (puking on a simple 'union' statement. WTF?   )
    quint8*  p1 ;

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
    else               m_offset[m_driveIndex] = 0 ;// If we don't find the starting bytes at all, just assume that it
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

    return false ;
}
***/


//  ------------------  SmartPort calls ----------------------
//   XXXXXXXXXXXXXXXX  -NONE- of this code is called at the moment.  Maybe someday.

//  Fix up the return address on the stack before returning from a SmartPort call.

void HdController::adjustReturnAddress (int nBytes)
{
    quint16 m_retAddr = m_mac->fetch(m_stackPtr+2)<<8 | m_mac->fetch(m_stackPtr+1) ;
    m_retAddr += nBytes ;
    m_mac->store (m_retAddr,    m_stackPtr+1) ;
    m_mac->store (m_retAddr>>8,  m_stackPtr+2) ;
}


//  Perform a "smartport" call for a hard drive or 3.5" floppy drive.
//  Returns "carry" value as a boolean:  false (carry=0) on success, true (carry=1) on error.
//  See "Apple IIgs Firmware Reference", chapter 7.
//
//  cmdlist for SmartPort call:
//  jsr C713
//    byte 0:  command number
//    byte 1:  cmd list ptr lo
//    byte 2:  cmd list ptr hi
//  next instruction

bool HdController::smartPort (void)
{
    m_stackPtr = 0x100 + m_mac->processorState()->Sptr ;
    m_retAddr  = 1 + (m_mac->fetch(m_stackPtr+2)<<8 | m_mac->fetch(m_stackPtr+1)) ;
    m_paramPtr = m_mac->fetch(m_retAddr+2)<<8 | m_mac->fetch(m_retAddr+1) ;
    quint8  cmd = m_mac->fetch (m_retAddr) ;

    if (cmd & 0x40) {               // This is an "extended" call.  We don't do extended.
        fprintf (stderr, "\n\a*** Unimplemented 'extended' SmartPort call made from 0x%4.4x\n", m_retAddr-2) ;
        fprintf (stderr, "*** This will probably be fatal to the running Apple II program.\n\n") ;
        m_mac->processorState()->Pstat |= C ;
        return true ;   // No idea what effect this will have...
    }

    m_mac->m_parent->HDActivityStarted (m_driveIndex) ;

//------------------------------

    bool error ;

    switch (cmd) {
        case 0:
printf ("SmartPort 'status'\n") ;
    //        status = sp_status (paramPtr) ;   // Status
            break ;
        case 1:
            error = sp_readblock() ;
            adjustReturnAddress (3) ;
            break ;
        case 2:
printf ("SmartPort 'write'\n") ;
            break ;
        case 3:
printf ("SmartPort 'format'\n") ;
            break ;
        case 4:
printf ("SmartPort 'control'\n") ;
            break ;
        case 5:
printf ("SmartPort 'init'\n") ;
            break ;
        case 6:
printf ("SmartPort 'open'\n") ;
            break ;
        case 7:
printf ("SmartPort 'close'\n") ;
            break ;
        case 8:
printf ("SmartPort 'char read'\n") ;
            break ;
        case 9:
printf ("SmartPort 'char write'\n") ;
            break ;
        default:
            fprintf (stderr, "\n*** \aProDOS \"SmartPort\" call made with bad command value = 0x%2.2X\n", cmd) ;
            fprintf (stderr, "*** This will probably be fatal to the running Apple II program.\n\n") ;
            error = true ;
    }

    return error ;
}


// cmdlist for SmartPort read:
//
//  00 param count (3) <---(m_paramPtr)
//  01 unit number
//  02 buffer ptr   lo
//  03 "        "   hi
//  04 block number lo
//  05 "          " hi (for 16-bit Apples)
//  06 "          " hi hor IIGS machines

bool HdController::sp_readblock (void)
{
    int driveIndex = m_mac->fetch (m_paramPtr+1) - 1 ;
    quint8 bplo = m_mac->fetch (m_paramPtr+2) ;
    quint8 bphi = m_mac->fetch (m_paramPtr+3) ;
    quint8* buffer = address (bphi<<8|bplo, true) ;
    quint8 blklo = m_mac->fetch (m_paramPtr+4) ;
    quint8 blkhi = m_mac->fetch (m_paramPtr+5) ;
    quint16 block = blkhi<<8 | blklo ;
//printf ("sp_readblock call from $%4.4x; buffer=%4.4x block=%i ($%4.4x)\n", m_retAddr-3, bphi<<8|bplo, block, block) ;

    if (driveIndex < 0) driveIndex = 0 ;
    if (driveIndex > 1) driveIndex = 1 ;
    m_driveIndex = driveIndex ;
    
    int n = 0 ;
    bool error = false ;
    headStepDelay (block) ;

    if (m_file[m_driveIndex].isOpen()) {
        m_file[m_driveIndex].seek (block*BLOCKSIZE + m_offset[m_driveIndex]) ;
        n = m_file[m_driveIndex].read ((char*)buffer, BLOCKSIZE) ;
        if (n != BLOCKSIZE) error = true ;
    }

    return error ;
}



/***

// p. 143

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

------------------------------------------------------------------------------

bool HdController::sp_status (quint16 paramPtr)
{
    quint8 unitNumber = *address (paramPtr+1) ;
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


/*****
// This is a dummy stand-in for a smartport call.
// It should never be called, but just in case, try to respond with an error status.

bool HdController::smartPort (void)
{
    ProcessorState* ps = m_mac->processorState() ;
    quint16 s = ps->Sptr + 0x100 ;
    quint16 retAddr = m_mac->fetch (s+1) | (m_mac->fetch (s+2)<<8) ;
printf ("*** Hard drive 'SmartPort' call was made from $%4.4x. This should not happen.\n", retAddr-2) ;
    retAddr += 3 ;
    m_mac->store (retAddr, s+1) ;
    m_mac->store (retAddr>>8, s+2) ;
    ps->Pstat |= C ;
    ps->Areg = 0x28 ;     // ($28 == "No device connected")

    return false ;
}
*****/

