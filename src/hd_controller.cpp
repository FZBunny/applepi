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




#ifndef Q_OS_WINDOWS
#  include <unistd.h>
#endif
#include <sys/types.h>


#include "machine.h"
#include "hd_controller.h"
#include "bootblock.h"
#include "debugging_dumps.h"



//------------------------------------------------
//  This class allows ProDos disk I/O for both
//  hard-drive images -and- 3.5" floppy images.
//------------------------------------------------



HdController::HdController (Machine* parent)
{
    m_parent = parent ;

    QString key ;
    CFG->Get ("hd1_volume_path", &m_configuredPath[0]) ;
    CFG->Get ("hd2_volume_path", &m_configuredPath[1]) ;

// qStdOut() << "HdController::HdController " << m_configuredPath[0] << "; drive " << 1 << endl ;
    if (m_configuredPath[0].length()) open (m_configuredPath[0], 0) ;

// qStdOut() << "HdController::HdController " << m_configuredPath[1] << "; drive " << 2 << endl ;
    if (m_configuredPath[1].length()) open (m_configuredPath[1], 1) ;
}


//                         An annoying kludge...
//
//  Find the offset, in bytes, to the beginning of the actual disk data
//  contained in the disk image.
//
//  For reasons I cannot comprehend, some hard-drive disk images found 
//  on the web contain mystery junk(?) before the disk boot blocks.
//  The length of this junk seems to be unpredictable.
//  So:  we search for the 4 bytes we expect to see as the 1st 4 bytes
//  the disk:
//
//  01      DB   #$01
//  38      SEC
//  b0 03   BCS  $0807
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

    for (n=0; n<BLOCKSIZE-3; n++) {
        if (buffer[n]==0x01 && buffer[n+1]==0x38 && buffer[n+2]==0xb0 && buffer[n+3]==0x003) break ;
    }

    m_offset[m_driveIndex] = n ;
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
    if ( ! m_fileInfo[m_driveIndex].exists()) return false ;  // "IsDir()" call doesn't seem to work...
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

int HdController::IO (void)
{
    quint16 buffer = MAC->m_ram[0x45]<<8 | MAC->m_ram[0x44] ;
    int block  = MAC->m_ram[0x47]<<8 | MAC->m_ram[0x46] ;
    quint8 operation = MAC->m_ram[0x42] ;
    if (MAC->m_ram[0x43] & 0x80) m_driveIndex = 1 ;
    else                         m_driveIndex = 0 ;
//printf ("HdController::IO  operation=%i\n", operation) ;
    int stat = 0 ;
//    m_file[m_driveIndex].ClearLastError() ;

    switch (operation) {
       case 0:                         // Status
          stat = status() ;
          break ;
       case 1:                         // Read
          m_parent->m_parent->HDActivityStarted (m_driveIndex) ;
          stat = readBlock (buffer, block) ;
//printf ("read: buffer=%4.4x block=%4.4x stat=%i\n", buffer, block, stat) ;
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


int HdController::status (void)        //  (See 'ProDos Tech. Notes', note 21 for status returns)
{
    int stat = 0 ;

    if (MAC->m_ram[0x43] & 0x80) m_driveIndex = 1 ;
    else                         m_driveIndex = 0 ;

    if (m_file[m_driveIndex].isOpen() == false) return NODEVICE ;

    else if (m_writeable[m_driveIndex] == false)  stat = WRITE_PROTECTED ; 
    quint64 length = (m_fileInfo[m_driveIndex].size() - m_offset[m_driveIndex]) / BLOCKSIZE  ; // (size in blocks)
    MAC->setX (length & 0xff) ;
    MAC->setY ((length >> 8) & 0xff) ;

//printf ("HdController::status m_ram[$42]=%2.2x m_ram[$43]=%2.2x; status=%2.2x\n", MAC->m_ram[0x42], MAC->m_ram[0x43], stat) ;
    return stat ;
}

             // XXXXX FIXME:  I/O to main or aux RAM?   XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
int HdController::readBlock (quint16 offset, int block)
{
    int n = 0 ;
    int stat = 0 ;
    headStepDelay (block) ;
    if (isOpen()) {
        m_file[m_driveIndex].seek (block*BLOCKSIZE + m_offset[m_driveIndex]) ;
        n = m_file[m_driveIndex].read ((char*)MAC->m_ram+offset, BLOCKSIZE) ;
        if (n != BLOCKSIZE) stat = IOERROR ;
    } else {
        memset (MAC->m_ram+offset, 0, BLOCKSIZE) ;
        stat = NODEVICE ;
    }
    return stat ;
}


int HdController::readBlock (quint8* buffer, int block, int driveIndex)
{
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


int HdController::writeBlock (quint16 buffer, int block)
{
    int n = 0 ;
    int stat = 0 ;
    if (isOpen()) {
        m_file[m_driveIndex].seek (block*BLOCKSIZE + m_offset[m_driveIndex]) ;
        n = m_file[m_driveIndex].write ((char*)MAC->m_ram+buffer, BLOCKSIZE) ;
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
    if (byteNumber & 1) n = *p1 ;
    else                n = *p0 ;
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

        int ttseekTime = 10 ;      //  Track-to-track seek time  (10 ms IS JUST A GUESS!) XXXXX
        int totalMsec = ttseekTime * trackDelta ;
        quickSleep (1000*totalMsec) ;
    } else {
                                   //  Must be a hard drive;  should we insert any delays here? XXXXX
    }
}

// XXXXXXXXXXXXXXXXXXXXXXX  SMARTPORT IS UNFINISHED XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

//  Perform a "smartport" call for a hard drive or 3.5" floppy drive.
//  Returns "carry" value as a boolean:  false (carry=0) on success, true (carry=1) on error.
//  See "Apple IIgs Firmware Reference", chapter 7.

bool HdController::smartPort (quint8 cmd, quint8* memory, quint16 paramPtr)
{
    bool status ;
    quint8 paramCount = memory[paramPtr] ;
    quint8 unitNumber = memory[paramPtr+1] ;
printf ("HdController::smartPort - cmd=%i; paramCount=%2.2x unitNumber=%2.2x\n", cmd, paramCount, unitNumber) ;

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
}


bool HdController::smartPortStatus (quint8* memory, quint16 paramPtr)
{
    quint8 unitNumber = memory[paramPtr+1] ;
    if (unitNumber > 1) unitNumber = 1 ;
    m_driveIndex = unitNumber ;
    quint16 statusListPtr = (memory[paramPtr+3]<<8) + memory[paramPtr+2] ;
    quint8 statusCode = memory[paramPtr+4] ;
printf ("HdController::smartPortStatus - statusListPtr=%4.4x statusCode=%2.2x\n", statusListPtr, statusCode) ;


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
