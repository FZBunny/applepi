/*****************************************************************************

    File: "floppy_controller.cpp"

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


/*****************************************************************************

    Data for this class was extracted from the book:
    "Beneath Apple DOS" by Don Worth & Pieter Lechner, fifth printing 1983
    Published by Quality Software
    ISBN-10: 0912985003   ISBN-13: 978-0912985008

*****************************************************************************/


#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QIODevice>
#include <QDebug>

#ifndef Q_OS_WINDOWS
#  include <unistd.h>
#endif
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "defs.h"
#include "machine.h"
#include "mainwindow.h"
//#include "headsounds.h"
#include "floppy_controller.h"
//#include "debugging_dumps.h"

using namespace std ;

class MainWindow ;


#define DEBUGFLAG 0x00000
#define DBUG(bits) if(DEBUGFLAG & bits)printf

static const unsigned int DISK_II_SIZE = 143360 ;   // In bytes


FloppyDiskController::FloppyDiskController (Machine* parent)
{
 //   m_sectorSkew[0] = DOS3 ;         // Default to DOS3 interleaving of sectors
 //   m_sectorSkew[1] = DOS3 ;

    m_parent = parent ;

    for (int i=0; i<2; i++) {

        m_diskInDriveFlag[i] = false ;
        m_volume[i] = 0 ;
        m_trackTimes2[i] = 34*2 ;
        m_sector[0] = 0 ;
        m_motorState[0] = 0 ;
        m_writeProtectFlag[i] = 0 ;
        m_bufferIndex[i] = -18 ;
        memset (&m_buffer[i], 0, SECTORSIZE*2) ;
        memset (m_encodedBuffer[i], 0, 344) ;

        QString key ;
        QTextStream (&key) << "floppy" << i+1 << "_path" ;
        CFG->Get (key, &m_filePath[i]) ;
        QFileInfo test ;
        test.setFile (m_filePath[i]) ;
        if ( ! test.exists()) continue ;
        if (test.isDir()) continue ;
        if ( ! test.isReadable()) continue ;
        QFile::OpenMode  mode ;

        m_currentDrive = i ;
        m_file[i].setFileName (m_filePath[i]) ;       
        bool writeProtected = ! test.isWritable() ;
        m_diskInDriveFlag[i] = open (i, m_filePath[i],writeProtected) ;

// qStdOut() << "FloppyDiskController::FloppyDiskController    m_filePath[" << i << "]=" << m_filePath[i]  << endl ;
// qStdOut() << "FloppyDiskController::FloppyDiskController    m_diskInDriveFlag[" << i << "]=" << m_diskInDriveFlag[i] << endl  << endl ;
    }

    m_delayByte = 0 ;
    m_delayInterSector  = 0 ;
    m_delayPostAddress = 0 ;
    m_currentDrive = 0 ;
}


FloppyDiskController::~FloppyDiskController(void)
{
    m_file[0].close() ;  // (If they're not open, no harm done)
    m_file[1].close() ;
}


int FloppyDiskController::diskInDrive (unsigned int drive)
{
    if (drive > 1) drive = 1 ;
    return m_diskInDriveFlag[drive] ;
}


bool FloppyDiskController::writeProtected (unsigned int drive)
{
    if (drive > 1) drive = 1 ;
    return m_writeProtectFlag[drive] ;
}


void FloppyDiskController::setWriteProtect (unsigned int drive, int flag)
{
    if (drive > 1) drive = 1 ;
    if (flag) {
        m_writeProtectFlag[drive] = true ;
    } else {
        m_writeProtectFlag[drive] = 0 ;
    }
}


//     Open a disk-image file

/***
bool FloppyDiskController::open (unsigned int drive, QString &path, bool writeProtected)
{
    bool ok ;
    if (writeProtected) ok = open (drive, path, QIODevice::ReadOnly) ;
    else                ok = open (drive, path, QIODevice::ReadWrite) ;

    if (ok) checkDiskType (drive) ;

    return ok ;
}
***/


bool FloppyDiskController::open (unsigned int driveIndex, QString &path, bool writeProtected)
{
// qStdOut() << "FloppyDiskController::open driveIndex=" << driveIndex << "   path =" <<  path << endl ;
    if (driveIndex > 1) driveIndex = 1 ;

    if (m_diskInDriveFlag[driveIndex]) {
        errno = EBUSY ;             //   Is this correct ?? XXXXXXXXXXXXXXXXXXXXXXXXXXXXXx
        return false ;
    }

    m_writeProtectFlag[driveIndex] = writeProtected ;
    m_filePath[driveIndex] = path ;

    QIODevice::OpenMode mode ;
    if (writeProtected) mode = QIODevice::ReadOnly ;
    else                mode = QIODevice::ReadWrite ;

    m_file[driveIndex].setFileName (path) ;
    bool ok = m_file[driveIndex].open (mode) ;
// printf ("FloppyDiskController::open ok = %i\n", ok) ;
    if ( ! ok) {
        return false ;
    }

    checkDiskType (driveIndex) ;
// printf ("driveIndex=%i   m_OS[driveIndex]=%i\n", driveIndex, m_OS[driveIndex]) ;
    m_file[driveIndex].seek(0) ;
    m_diskInDriveFlag[driveIndex] = true ;
    m_bufferIndex[m_currentDrive] = -18 ;
    memset (m_buffer[driveIndex], 0, SECTORSIZE) ;
    memset (m_encodedBuffer[driveIndex], 0, 344) ;

    m_dataPrologCounter = 0 ;
    m_diskAddressCounter = 0 ;
    m_epilogCounter = 0 ;
    m_writingData = false ;
//printf ("m_sectorSkew[%i]=%i\n", driveIndex, m_sectorSkew[driveIndex]) ;
    return true ;
}


//     Create & open a new disk-image file

QString FloppyDiskController::create  (unsigned int driveIndex, QString &path)
{
    int          i ;
//    struct stat  status ;

    if (driveIndex > 1) driveIndex = 1 ;

    if (diskInDrive(driveIndex)) {
        errno = EBUSY ;
        return QString("Drive is not empty") ;
    }

    m_writeProtectFlag[driveIndex] = 0 ;

    m_file[driveIndex].setFileName (path) ;
    bool ok = m_file[driveIndex].open (QIODevice::ReadWrite) ;
    if ( ! ok) {
        return m_file[driveIndex].errorString() ;
    }

    char  m_buffer[SECTORSIZE] ;
    memset (m_buffer, 0, SECTORSIZE) ;
    for (i=0; i<35*16; i++) {
        m_file[driveIndex].write (m_buffer, SECTORSIZE) ;
        m_file[driveIndex].flush() ;
    }

	m_filePath[driveIndex] = path ;
    m_diskInDriveFlag[driveIndex] = true ;
    m_bufferIndex[m_currentDrive] = -18 ;

    m_file[driveIndex].close() ;
    m_file[driveIndex].setFileName (path) ;
    m_file[driveIndex].open (QIODevice::ReadWrite) ;

    return NULL ;
}


//     Close a disk-image file

int FloppyDiskController::close (unsigned int driveIndex)
{
    if (diskInDrive(driveIndex) == false) {
        errno = ENXIO ;
        return errno ;
    }
    m_file[driveIndex].close() ;

    m_filePath[driveIndex] = "" ;
    m_diskInDriveFlag[driveIndex] = false ;

    return 0 ;
}


//     Read a sector from a disk.  This is called only from MainWindow class to
//     get release & volume number info for a DOS disk label. 

int FloppyDiskController::readSector (quint8* buffer, int driveNumber, int track, int sector)
{
    if (driveNumber < 1) driveNumber = 1 ;
    if (driveNumber > 2) driveNumber = 2 ;
    int index = driveNumber - 1 ;
    if (m_file[index].isOpen() == false) return -1 ;

    int fileOffset = (track * 4096) + (sector * SECTORSIZE) ;
    m_file[index].seek (fileOffset) ;
    int n = m_file[index].read ((char*)buffer, SECTORSIZE) ;

    return n ;
}


//
// --------- Track stepper motor ----------
//

static int TRKMAX = 34*2 ;


//  This function allows us to boot later verions of ProDOS.
//  Apparently, the "phaseX_on/off functions below do not
//  model the floppy drives properly.  I'll figure it out
//  later.  Maybe.

void FloppyDiskController::embarassingKludge(void)
{
//    bool allPolesAreOff = false ;

    if (m_OS[m_currentDrive] == PRODOS) {
        int i ;
        for (i=0; i<4; i++) {
            if (m_pole[m_currentDrive][i]) break ;
        }

        if (i == 3) m_trackTimes2[m_currentDrive] = 0 ;
    }

}


// phase 0

void FloppyDiskController::phase0_off (void)
{
    m_pole[m_currentDrive][0] = false ;

    embarassingKludge() ;

    m_stepperState[m_currentDrive] = 0 ;
DBUG(0x0001) ("phase0_off m_trackTimes2[%i] = %2i\n", m_currentDrive, m_trackTimes2[m_currentDrive]) ;
}

void FloppyDiskController::phase0_on  (void)
{
    m_pole[m_currentDrive][0] = true ;
    if (m_stepperPhase[m_currentDrive] == 1) {
        m_trackTimes2[m_currentDrive] -= 1 ;
        if (m_trackTimes2[m_currentDrive] < 0) m_trackTimes2[m_currentDrive] = 0 ;
    }

    if (m_stepperPhase[m_currentDrive] == 3) {
        m_trackTimes2[m_currentDrive] += 1 ;
        if (m_trackTimes2[m_currentDrive] > TRKMAX) m_trackTimes2[m_currentDrive] = TRKMAX ;
    }

    m_stepperPhase[m_currentDrive] = 0 ;
    m_stepperState[m_currentDrive] = 1 ;

DBUG(0x0002) ("phase0_on  m_trackTimes2[%i] = %2i\n", m_currentDrive, m_trackTimes2[m_currentDrive]) ;
}


// phase 1

void FloppyDiskController::phase1_off (void)
{
    m_pole[m_currentDrive][1] = false ;
    if (m_trackTimes2[m_currentDrive] == 0) {
//        m_parent->setFloppySound() ;
    } else {
//        m_parent->setFloppySound() ;
    }
    m_stepperState[m_currentDrive] = 0 ;

DBUG(0x0004) ("phase1_off m_trackTimes2[%i] = %2i\n", m_currentDrive, m_trackTimes2[m_currentDrive]) ;
}

void FloppyDiskController::phase1_on  (void)
{
    m_pole[m_currentDrive][1] = true ;

    main_window->play (1) ;           // Play a 'head step' .wav sound

    if (m_stepperPhase[m_currentDrive] == 2) {
        m_trackTimes2[m_currentDrive] -= 1 ;
        if (m_trackTimes2[m_currentDrive] < 0) m_trackTimes2[m_currentDrive] = 0 ;
    }

    if (m_stepperPhase[m_currentDrive] == 0) {
        m_trackTimes2[m_currentDrive] += 1 ;
        if (m_trackTimes2[m_currentDrive] > TRKMAX) m_trackTimes2[m_currentDrive] = TRKMAX ;
    }

    m_stepperPhase[m_currentDrive] = 1 ;
    m_stepperState[m_currentDrive] = 1 ;
DBUG(0x0008) ("phase1_on  m_trackTimes2[%i] = %2i\n", m_currentDrive, m_trackTimes2[m_currentDrive]) ;
}


// phase 2

void FloppyDiskController::phase2_off (void)
{
    m_pole[m_currentDrive][2] = false ;
    m_stepperState[m_currentDrive] = 0 ;
DBUG(0x0010) ("phase2_off m_trackTimes2[%i] = %2i\n", m_currentDrive, m_trackTimes2[m_currentDrive]) ;
}

void FloppyDiskController::phase2_on  (void)
{
    m_pole[m_currentDrive][2] = true ;
    if (m_stepperPhase[m_currentDrive] == 3) {
        m_trackTimes2[m_currentDrive] -= 1 ;
        if (m_trackTimes2[m_currentDrive] < 0) m_trackTimes2[m_currentDrive] = 0 ;
    }

    if (m_stepperPhase[m_currentDrive] == 1) {
        m_trackTimes2[m_currentDrive] += 1 ;
        if (m_trackTimes2[m_currentDrive] > TRKMAX) m_trackTimes2[m_currentDrive] = TRKMAX ;
    }

    m_stepperPhase[m_currentDrive] = 2 ;
    m_stepperState[m_currentDrive] = 1 ;
DBUG(0x0020) ("phase2_on  m_trackTimes2[%i] = %2i\n", m_currentDrive, m_trackTimes2[m_currentDrive]) ;
}


// phase 3

void FloppyDiskController::phase3_off (void)
{
    m_pole[m_currentDrive][3] = false ;
    m_stepperState[m_currentDrive] = 0 ;
DBUG(0x0040) ("phase3_off m_trackTimes2[%i] = %2i\n", m_currentDrive, m_trackTimes2[m_currentDrive]) ;
}

void FloppyDiskController::phase3_on  (void)
{
    m_pole[m_currentDrive][3] = true ;
    if (m_stepperPhase[m_currentDrive] == 0) {
        m_trackTimes2[m_currentDrive] -= 1 ;
        if (m_trackTimes2[m_currentDrive] < 0) m_trackTimes2[m_currentDrive] = 0 ;
    }

    if (m_stepperPhase[m_currentDrive] == 2) {
        m_trackTimes2[m_currentDrive] += 1 ;
        if (m_trackTimes2[m_currentDrive] > TRKMAX) m_trackTimes2[m_currentDrive] = TRKMAX ;
    }

    m_stepperPhase[m_currentDrive] = 3 ;
    m_stepperState[m_currentDrive] = 1 ;
DBUG(0x0080) ("phase3_on  m_trackTimes2[%i] = %2i\n", m_currentDrive, m_trackTimes2[m_currentDrive]) ;
}


//
// --------- drive motor ----------
//

void FloppyDiskController::motorOff (void)
{
DBUG(0x0100) ("motorOff\n") ; fflush(stdout) ;
}


// This function seem backwards (the floppy tells the main frame that is has started a motor,
// so that the frame can start a motor timer to control it's LEDs).   But this way prevents 
// complaints that timers should be started only in the main thread.

void FloppyDiskController::motorOn  (void)
{
    m_parent->m_parent->floppyMotorHasStarted (m_currentDrive) ;
DBUG(0x0200) ("motorOn\n") ; fflush(stdout) ;
}


//
// --------- drive select ----------
//


void FloppyDiskController::drive0_enable (void)
{
    m_currentDrive = 0 ;
DBUG(0x0400) ("drive0_enable\n") ; fflush(stdout) ;
}


void FloppyDiskController::drive1_enable (void)
{
     m_currentDrive = 1 ;
DBUG(0x0800) ("drive1_enable\n") ; fflush(stdout) ;
}


bool FloppyDiskController::checkDiskType (int driveIndex)
{
    typedef struct diskTypeData {
        quint32 offset ;
        quint8  value ;
    } diskTypeData ;

//   These tables of offsets and values have been shamelessly stolen
//   from AppleWin v1.29.13.0 "DiskImageHelper.cpp", where they are
//   expressed algorithmically.  I just like tables.

    static diskTypeData dos_dos[] =
    {
         0x11102, 0x00,
         0x11202, 0x01,
         0x11302, 0x02,
         0x11402, 0x03,
         0x11502, 0x04,
         0x11602, 0x05,
         0x11702, 0x06,
         0x11802, 0x07,
         0x11902, 0x08,
         0x11A02, 0x09,
         0x11B02, 0x0A,
         0x11C02, 0x0B,
         0x11D02, 0x0C,
         0x11E02, 0x0D,
         0x11F02, 0x0E
    } ;

    static diskTypeData dos_proDos[] =
    {
        0x0500, 0x04,
        0x0502, 0x00,
        0x0700, 0x03,
        0x0702, 0x05,
        0x0900, 0x02,
        0x0902, 0x04,
        0x0B00, 0x00,
        0x0B02, 0x03
    } ;

    static diskTypeData proDos_dos[] =
    {
        0x11502, 0x09,
        0x11602, 0x08,
        0x11702, 0x07,
        0x11802, 0x06,
        0x11902, 0x05,
        0x11A02, 0x04,
        0x11B02, 0x03,
        0x11C02, 0x02,
        0x11D02, 0x01
    } ;

    static diskTypeData proDos_proDos[] =
    {
        0x0400, 0x00,
        0x0402, 0x03,
        0x0600, 0x02,
        0x0602, 0x04,
        0x0800, 0x03,
        0x0802, 0x05,
        0x0A00, 0x04,
        0x0A02, 0x00
    } ;


    m_file[driveIndex].read ((char*)m_image[driveIndex], DISK_II_SIZE) ;


// ============= DOS checks


//  Check for DOS order image of a DOS disk

    bool ok = true ;

    for (int i=0; i<15; i++) {
        uint  offset = dos_dos[i].offset ;
        u_char value = dos_dos[i].value ;
DBUG(0x10000)("dosdos:    driveIndex=%x  offset=%6.6x value=%2.2x   m_image=%2.2x\n", driveIndex, offset, value, m_image[driveIndex][offset]) ;
        if (m_image[driveIndex][offset] != value) {
            ok = false;
            break;
        }
    }

    if (ok) {
DBUG(0x20000)("driveIndex %i: DOS order, DOS image\n", driveIndex) ;
        m_sectorSkew[driveIndex] = DOS3 ;
        m_OS[driveIndex] = DOS3 ;
        return true ;
    }

//  Check for DOS order image of a PRODOS disk

    ok = true ;

    for (int i=0; i<8; i++) {
        uint  offset = dos_proDos[i].offset ;
        u_char value = dos_proDos[i].value ;
DBUG(0x10000)("dosProdos: driveIndex=%x  offset=%6.6x value=%2.2x   m_image=%2.2x\n", driveIndex, offset, value, m_image[driveIndex][offset]) ;
        if (m_image[driveIndex][offset] != value) {
            ok = false;
            break;
        }
    }

    if (ok) {
DBUG(0x20000)("driveIndex %i: DOS order, PRODOS image\n", driveIndex) ;
        m_sectorSkew[driveIndex] = DOS3 ;
        m_OS[driveIndex] = PRODOS ;
        return true ;
    }


// ============= PRODOS checks

//  Check for PRODOS order image of a DOS disk

    ok = true ;

    for (int i=0; i<9; i++) {
        uint  offset = proDos_dos[i].offset ;
        u_char value = proDos_dos[i].value ;
DBUG(0x10000)("proDos_dos:    driveIndex=%x  offset=%6.6x value=%2.2x   m_image=%2.2x\n", driveIndex, offset, value, m_image[driveIndex][offset]) ;
        if (m_image[driveIndex][offset] != value) {
            ok = false;
            break;
        }
    }

    if (ok) {
DBUG(0x20000)("driveIndex %i: PRDOS order, DOS image\n", driveIndex) ;
        m_sectorSkew[driveIndex] = PRODOS ;
        m_OS[driveIndex] = DOS3 ;
        return true ;
    }

//  Check for PRODOS order image of a PRODOS disk

    ok = true ;

    for (int i=0; i<8; i++) {
        uint  offset = proDos_proDos[i].offset ;
        u_char value = proDos_proDos[i].value ;
DBUG(0x10000)("proDos_ProDos:    driveIndex=%x  offset=%6.6x value=%2.2x   m_image=%2.2x\n", driveIndex, offset, value, m_image[driveIndex][offset]) ;
        if (m_image[driveIndex][offset] != value) {
            ok = false;
            break;
        }
    }

    if (ok) {
DBUG(0x20000)("driveIndex %i: PRODOS order, PRODOS image\n", driveIndex) ;
        m_sectorSkew[driveIndex] = PRODOS ;
        m_OS[driveIndex] = PRODOS ;
        return true ;
    }

// Check for a Pascal disk (in a very hack-ish manner)

    m_sectorSkew[driveIndex] = DOS3 ;     // Assume DOS3 skew
    m_OS[driveIndex] = DOS3 ;             // and DOS type

    int n = m_image[driveIndex][0xb06] ;  // We just check for a valid Pascal volume name;
    if (n>0 && n<8) {                     // if it's valid, we assume it's a Pascal disk.
        int i ;                           // (Yes, this is a weak test, but it's all we got...)
        for (i=0; i<n; i++) {
            quint8 c = m_image[driveIndex][0xb07 + i] ;
            if (c < 0x20) break ;
            if (c > 0x7e) break ;
            if ((c=='=') || (c=='$') || (c=='?') || (c==',')) break ;
        }
        if (i==n) m_OS[driveIndex] = PASCAL ;
    }

    if (m_OS[driveIndex] == PASCAL) return true ;

    m_OS[driveIndex] = UNKNOWN ;

DBUG(0x20000)("*** DEFAULTING TO:  driveIndex %i: DOS order, DOS image\n\n", driveIndex) ;

    return false ;
}



//  From AppleWin 'DiskImageHelper.cpp'
//
/* DO logical order  0 1 2 3 4 5 6 7 | 8 9 A B C D E F */
/*    physical order 0 D B 9 7 5 3 1 | E C A 8 6 4 2 F */

/* PO logical order  0 E D C B A 9 8 | 7 6 5 4 3 2 1 F */
/*    physical order 0 2 4 6 8 A C E | 1 3 5 7 9 B D F */
/****
enum SectorOrder_e {eProDOSOrder, eDOSOrder, eSIMSYSTEMOrder, NUM_SECTOR_ORDERS};

BYTE CImageBase::ms_SectorNumber[NUM_SECTOR_ORDERS][0x10] =
{
	{0x00,0x08,0x01,0x09,0x02,0x0A,0x03,0x0B,  0x04,0x0C,0x05,0x0D,0x06,0x0E,0x07,0x0F},  // ProDos
	{0x00,0x07,0x0E,0x06,0x0D,0x05,0x0C,0x04,  0x0B,0x03,0x0A,0x02,0x09,0x01,0x08,0x0F},  // DOS
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}   // "IMSYSTEM" (?)
//     0    1    2    3    4    5    6    7     8    9   10   11   12   13   14   15
};
****/


uint FloppyDiskController::logicalSector (void)
{
    static const int  dos3LogicalToPhysicalSector[16] = {
        0, 13, 11,  9,  7,  5,  3,  1,
       14, 12, 10,  8,  6,  4,  2, 15
    } ;

    static const int  prodosLogicalToPhysicalSector[16] = {
        0, 2, 4, 6, 8, 10, 12, 14,
        1, 3, 5, 7, 9, 11, 13, 15
    } ;

    uint physicalSector = m_sector[m_currentDrive] ;
    uint logicalSector ;

    if (m_sectorSkew[m_currentDrive] == DOS3)  logicalSector = dos3LogicalToPhysicalSector[physicalSector] ;
    else                                       logicalSector = prodosLogicalToPhysicalSector[physicalSector] ;

    return logicalSector ;
}


uint FloppyDiskController::getFileOffset (void)
{
    uint sector = m_sector[m_currentDrive] ;
    uint track = m_trackTimes2[m_currentDrive]/2 ;

    return (SECTORSIZE * sector) + (16*SECTORSIZE * track) ;
}


quint8 FloppyDiskController::addressData (quint8 xx, quint8 yy)
{
    xx <<= 1 ;
    quint8 data = (xx & 0xaa) | (yy & 0x55) ;
    return data ;
}


int FloppyDiskController::OSType (int driveNumber) // Called from MainWindow, so drive# = 1 or 2
{
    if (driveNumber < 1) driveNumber = 1 ;
    if (driveNumber > 2) driveNumber = 2 ;
    int driveIndex = driveNumber - 1 ;

   return m_OS[driveIndex] ;
}


//
// ---------  I / O ----------
//


quint8 FloppyDiskController::read (void)
{

    static const quint8      writeTranslateTable[64] = {
        0x96, 0x97, 0x9a, 0x9b, 0x9d, 0x9e, 0x9f, 0xa6,
        0xa7, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb2, 0xb3,
        0xb4, 0xb5, 0xb6, 0xb7, 0xb9, 0xba, 0xbb, 0xbc,
        0xbd, 0xbe, 0xbf, 0xcb, 0xcd, 0xce, 0xcf, 0xd3,
        0xd6, 0xd7, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde,
        0xdf, 0xe5, 0xe6, 0xe7, 0xe9, 0xea, 0xeb, 0xec,
        0xed, 0xee, 0xef, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6,
        0xf7, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff
    } ;

    quint8  c = 0 ;
    int      bix = m_bufferIndex[m_currentDrive] ;
//printf ("bix=%i\n", bix) ; fflush(stdout) ;
    static quint8  addrCsum0, addrCsum1 ;

//    if (++m_delayByte < 2) {
//        m_bufferIndex[m_currentDrive]-- ;
//        return 0xff ;
//    }
    m_delayByte = 0 ;

    switch (bix) {
        case -17:
//            if (++m_delayInterSector < 20) {
//                m_bufferIndex[m_currentDrive]-- ;
//                return 0 ;
//            }
//            m_delayInterSector = 0 ;
            c = 0xd5 ;                                 // address field prologue
            break ;
        case -16:
            c = 0xaa ;
            break ;
        case -15:
            c = 0x96 ;
            break ;
        case -14:                                      // volume
            c = m_volume[m_currentDrive] >> 1 ;
            c |= 0xaa ;
            addrCsum0 = c ;
            break ;
        case -13:
            c = m_volume[m_currentDrive] | 0xaa ;
            addrCsum1 = c ;
            break ;
        case -12:                                      // track
            c = (m_trackTimes2[m_currentDrive] >> 2) | 0xaa ;
            addrCsum0 ^= c ;
            break ;
        case -11:
            c = (m_trackTimes2[m_currentDrive] >> 1) | 0xaa ;
            addrCsum1 ^= c ;
            break ;
        case -10:                                      // sector
            c = logicalSector() >> 1 ;
            c |= 0xaa ;
            addrCsum0 ^= c ;
            break ;
        case -9:
            c = logicalSector() | 0xaa ;
            addrCsum1 ^= c ;
            break ;
        case -8:                                       // address field checksum
            c = addrCsum0 ;
            break ;
        case -7:
            c = addrCsum1 ;
            break ;
        case -6:
            c = 0xde ;                                 // address field epilogue
            break ;
        case -5:
            c = 0xaa ;
            break ;
        case -4:
            c = 0xeb ;
            break ;
        case -3:
//            if (++m_delayPostAddress < 20) {
//                m_bufferIndex[m_currentDrive]-- ;
//                return 0xff ;
//            }
//            m_delayPostAddress = 0 ; */
            c = 0xd5 ;                                 // data field prologue
            break ;
        case -2:
            c = 0xaa ;
            break ;
        case -1:
            c = 0xad ;
            break ;
        case 0:                                        // *** data ***
            int i ;
            if (m_diskInDriveFlag[m_currentDrive] == false) {
                c = 0 ;
            } else {
                quint8  eor, prev_c ;
                quint8* b = m_buffer[m_currentDrive] ;           // 'b' for Buffer
                quint8* e = m_encodedBuffer[m_currentDrive] ;    // 'e' for Encoded buffer
                int fileOffset = getFileOffset() ;
                m_file[m_currentDrive].seek (fileOffset) ;
                m_file[m_currentDrive].read ((char*)b, SECTORSIZE) ;

                for (i=0; i<86; i++) {       // e[0] .. e[85]
                    c = 0 ;
                    if (b[i] & 2)    c |= 0x01 ;
                    if (b[i] & 1)    c |= 0x02 ;
                    if (b[86+i] & 2) c |= 0x04 ;
                    if (b[86+i] & 1) c |= 0x08 ;
                    if (i < 84) {
                        if (b[2*86+i] & 2) c |= 0x10 ;
                        if (b[2*86+i] & 1) c |= 0x20 ;
                    }
                    e[i] = c ;
                }
                for (i=0; i<SECTORSIZE; i++) {      // e[86] .. e[341]
                    e[i+86] = b[i] >> 2 ;
                }
                quint8 csum = 0 ;           // EOR bytes & calculate checksum
                prev_c = 0 ;
                for (i=0; i<342; i++) {
                    c = e[i] ;
                    eor = c ^ prev_c ;
                    e[i] = writeTranslateTable[eor] ;
                    prev_c = c ;
                    csum ^= eor ;
                }
                e[i] = writeTranslateTable[csum] ;
            }     // note drop thru to default; this is deliberate...
        default:  // cases 1..342 (fileOffset 342 is the data field checksum)
DBUG(0x1000) ("Q6L  reading;  m_bufferIndex = %4d;  c=%2.2X\n", m_bufferIndex[m_currentDrive], c) ;
            if ((bix<-18) || (bix>345)) {
                m_bufferIndex[m_currentDrive] = -18 ;
                break ;
            }
            c = m_encodedBuffer[m_currentDrive][m_bufferIndex[m_currentDrive]] ;
            break ;
        case 343:                                      // data field epilogue
            c = 0xde ;
            break ;
        case 344:
            c = 0xaa ;
            break ;
        case 345:
            c = 0xeb ;
            m_bufferIndex[m_currentDrive] = -18 ;
            m_sector[m_currentDrive] += 1 ;
            if (m_sector[m_currentDrive] > 15) {
                m_sector[m_currentDrive] = 0 ;
            }
//printf ("track %i new (m_sector[%i]=%i\n", m_trackTimes2[m_currentDrive]/2, m_currentDrive, m_sector[m_currentDrive]) ;
            break ;
    }

    return c ;
}


void FloppyDiskController::write (void)
{
    static const int      readTranslateTable[] = {
//       0     1     2     3      4     5     6     7        8     9     A     B      C     D     E     F
/*90*/                                       0x00, 0x01,    0x98, 0x99, 0x02, 0x03,  0x9c, 0x04, 0x05, 0x06,
/*A0*/  0xa0, 0xa1, 0xa2, 0xa3,  0xa4, 0xa5, 0x07, 0x08,    0xa8, 0xa9, 0xaa, 0x09,  0x0a, 0x0b, 0x0c, 0x0d,
/*B0*/  0xb0, 0xb1, 0x0e, 0x0f,  0x10, 0x11, 0x12, 0x13,    0xb8, 0x14, 0x15, 0x16,  0x17, 0x18, 0x19, 0x1a,

/*C0*/  0xc0, 0xc1, 0xc2, 0xc3,  0xc4, 0xc5, 0xc6, 0xc7,    0xc8, 0xc9, 0xca, 0x1b,  0xcc, 0x1c, 0x1d, 0x1e,
/*D0*/  0xd0, 0xd1, 0xd2, 0x1f,  0xd4, 0xd5, 0x20, 0x21,    0xd8, 0x22, 0x23, 0x24,  0x25, 0x26, 0x27, 0x28,
/*E0*/  0xe0, 0xe1, 0xe2, 0xe3,  0xe4, 0x29, 0x2a, 0x2b,    0xe8, 0x2c, 0x2d, 0x2e,  0x2f, 0x30, 0x31, 0x32,
/*F0*/  0xf0, 0xf1, 0x33, 0x34,  0x35, 0x36, 0x37, 0x38,    0xf8, 0x39, 0x3a, 0x3b,  0x3c, 0x3d, 0x3e, 0x3f
//       0     1     2     3      4     5     6     7        8     9     A     B      C     D     E     F
    } ;

    quint8*  r = m_rawBuffer[m_currentDrive] ;            // "Raw" buffer that has been received one byte per call
    quint8*  e = m_encodedBuffer[m_currentDrive] ;        // Encoded buffer; an intermediate buffer
    quint8*  s = m_encodedBuffer[m_currentDrive] + 256 ;  // Secondary buffer; contains lower 2 bits of finished bytes.
    quint8*  b = m_buffer[m_currentDrive] ;               // Buffer containing data to be written to disk
    quint8   c = m_latchedData[m_currentDrive] ;          // Set when Q6H is called from 'store_sspage'
    int       track ;

    int bix = m_bufferIndex[m_currentDrive] ;
    if (bix<0 || bix>342)  bix = m_bufferIndex[m_currentDrive] = 0 ;
DBUG(0x1000) ("Q6L  writing;  bix = %4d;  c=%2.2X\n", bix, c) ;

    // Wait for address prologue, then get vol-track-sector address

    quint8 xx=0, yy=0 ;
    if (m_writingData == false ) {
        switch (m_diskAddressCounter) {
            case 0:
                if (c == m_addressProlog[0]) m_diskAddressCounter++ ;
                break ;
            case 1:
                if (c == m_addressProlog[1]) m_diskAddressCounter++ ;
                break ;
            case 2:
                if (c == m_addressProlog[2]) m_diskAddressCounter++ ;
                break ;
            case 3:                       // Volume
                xx = c ;
                m_diskAddressCounter++ ;
                return ;
                break ;
            case 4:
                yy = c ;
                m_volume[m_currentDrive] = addressData (xx,yy) ;
                m_diskAddressCounter++ ;
                return ;
                break ;
            case 5:                       // Track m_encodedBuffer[1][341
                xx = c ;
                m_diskAddressCounter++ ;
                return ;
                break ;
            case 6:
                yy = c ;
                track = addressData (xx,yy) ;
                m_trackTimes2[m_currentDrive] = track << 1 ;
                m_diskAddressCounter++ ;
                return ;
                break ;
            case 7:                       // Sector
                xx = c ;
                m_diskAddressCounter++ ;
                return ;
                break ;
            case 8:
                yy = c ;
                m_sector[m_currentDrive] = addressData (xx,yy) ;
                m_diskAddressCounter++ ;
                return ;
                break ;
            default:                      // (We ignore address epilogue.)
                m_diskAddressCounter = 0 ;
        }
    }

    // watch for data prologue; when found, set 'm_writingData' to true

    if (m_writingData == false ) {
        switch (m_dataPrologCounter) {
            case 0:
                if (c == m_dataProlog[0]) m_dataPrologCounter++ ;
                return ;
                break ;
            case 1:
                if (c == m_dataProlog[1]) m_dataPrologCounter++ ;
                return ;
                break ;
            case 2:
                if (c == m_dataProlog[2]) {
                    m_dataPrologCounter++ ;
                    m_writingData = true ;
                    m_bufferIndex[m_currentDrive] = 0 ;
                    return ;
                }
                break ;
            default:
                 m_dataPrologCounter = 0 ;
        }
    }

    if (m_writingData) {

        r[bix] = c ;           // *** put 1 byte of raw data in raw buffer ***

        if (++m_bufferIndex[m_currentDrive] > 342) {    // Raw buffer full?
                                                        // If so, untransmogrify it.

//  The ROM code has fed this function, one byte at a time, 342 bytes which can contain only
//  values which have no more than one pair of consecutive zeros.  This was was necessary due to
//  the limitations of the diskII controller card. (The ROM code thinks these bytes are going
//  directly to the read/write head.)  We need to translate everything back to 256 bytes of
//  the original data, which is what the ROM code does when *reading* a sector.

            quint8 eor = 0 ;
            int rix = 0 ;
            for (int i=85; i>-1; i--) {         // Fill "secondary" buffer
                c = r[rix++] ;
                eor ^= readTranslateTable[c-0x96] ;
                s[i] = eor ;
            }

            for (int i=0; i<256; i++) {         // Fill "primary" buffer
                c = r[rix++] ;
                eor ^= readTranslateTable[c-0x96] ;
                e[i] = eor ;
            }

            int sbix = 85 ;                    // Convert 342 6-bit nibbles to 256 8-bit bytes
            for (int i=0; i<256; i++) {
                c = e[i] ;
                c <<= 2 ;
                quint8 loBits = s[sbix] ;
                s[sbix] >>= 2 ;
                if (loBits & 1)  c |= 2 ;
                if (loBits & 2)  c |= 1 ;
                b[i] = c ;
                if (--sbix < 0)  sbix = 85 ;
            }


            uint fileOffset = getFileOffset() ; // calculate fileOffset into file, & write 256-byte buffer

            m_file[m_currentDrive].seek (fileOffset) ;
            m_file[m_currentDrive].write ((char*)b, 256) ;
            m_file[m_currentDrive].flush() ;

            m_bufferIndex[m_currentDrive] = 0 ;
            m_diskAddressCounter = 0 ;
            m_dataPrologCounter = 0 ;
            m_writingData = false ;

        }
    }
}


//     Read or write a single byte of data

unsigned char FloppyDiskController::Q6L (void)
{
    m_lastAccess[m_currentDrive] = (qint64)time(NULL) ;
    if (m_diskInDriveFlag[m_currentDrive] == false) return 0 ;

    if (m_bufferIndex[m_currentDrive]<-18 || m_bufferIndex[m_currentDrive]>345) {
        m_bufferIndex[m_currentDrive] = -18 ;
    }

    quint8  c = 0 ;
    if (m_dataLatchedForOutput[m_currentDrive]) {
        write() ;
    } else {
        m_bufferIndex[m_currentDrive] += 1 ;
        c = read() ;
    }

    return c ;
}


void FloppyDiskController::Q6H (unsigned char c)
{
    m_latchedData[m_currentDrive] = c ;
DBUG(0x2000) ("Q6H  latched data=%2.2X\n", c) ;
}


unsigned char FloppyDiskController::Q7L (void)
{
    unsigned char c ;

    m_dataLatchedForOutput[m_currentDrive] = false ;

    if (m_writeProtectFlag[m_currentDrive]) {
        c =  0x80 ;
    } else {
        c = 0 ;
    }
DBUG(0x4000) ("Q7L  data latched for input; m_writeProtectFlag=%2.2X\n", c) ;
    return c ;
}


void FloppyDiskController::Q7H (void)
{
    m_dataLatchedForOutput[m_currentDrive] = true ;
DBUG(0x8000) ("Q7H  data latched for output\n") ;
}

