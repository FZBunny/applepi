/*****************************************************************************

    File: "floppy_controller.h"

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




#ifndef H_FLOPPY
#define H_FLOPPY 1


#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>

#include "defs.h"
#include "machine.h"

using namespace std ;



class Machine ;

class FloppyDiskController
{

public:

    FloppyDiskController (Machine* parent) ;
    ~FloppyDiskController(void) ;

    bool open   (unsigned int drive, QString& path, bool writeProtected) ;
    bool open   (unsigned int drive, QString& path, QFile::OpenMode mode) ;
    QString create (unsigned int drive, QString& path) ;
    int  close  (unsigned int drive) ;
    int  readSector (quint8* buffer, int driveNumber, int track, int sector) ;

    bool writeProtected (unsigned int drive) ;
    void setWriteProtect (unsigned int drive, int flag) ;

    void forgetTrackNumbers (void) ;
//    QString *file (unsigned int drive) ; XXXXXXXXXXXXXXXXXXX FIX
    int diskInDrive (unsigned int drive) ;
    int OSType (int driveNumber) ;

    void  onTimer0 (QTimerEvent&) ;
    void  onTimer1 (QTimerEvent&) ;

    void phase0_off (void) ;    // stepper motor phases
    void phase0_on  (void) ;
    void phase1_off (void) ;
    void phase1_on  (void) ;
    void phase2_off (void) ;
    void phase2_on  (void) ;
    void phase3_off (void) ;
    void phase3_on  (void) ;

    void motorOff  (void) ;
    void motorOn   (void) ;

    void drive0_enable (void) ;
    void drive1_enable (void) ;

    unsigned char  Q6L (void) ;           // strobe data latch for I/O
    void           Q6H (unsigned char) ;  // load data latch
    unsigned char  Q7L (void) ;           // prepare latch for input
    void           Q7H (void) ;           // prepare latch for output

private:

    Machine* m_parent;

    void embarassingKludge(void) ;

    bool    checkDiskType (int driveIndex) ;
    uint    logicalSector (void) ;
    uint    getFileOffset (void) ;
    quint8  addressData (quint8 xx, quint8 yy) ;
    quint8  read (void) ;
    void    write (void) ;

    int  m_currentDrive ;
    int  m_trackTimes2[2] ;
    int  m_volume[2] ;
    int  m_sector[2] ;

    quint8  m_latchedData[2] ;
    int     m_dataLatchedForOutput[2] ;

    int  m_stepperPhase[2] ;   // 0..3
    int  m_stepperState[2] ;   // on(1) or off(0)
    int  m_motorState[2] ;     // on(1) or off(0)

    bool m_pole[2][4] ;        // state of head stepper pole 'n'; on or off
                               // part of an embarassing hack for ProDOS...

    bool m_writeProtectFlag[2] ;
    bool m_diskInDriveFlag[2] ;

    int  m_sectorSkew[2] ;     // Either DOS3 or PRODOS
    int  m_OS[2] ;             // "                   "

    QFile   m_file[2] ;
    QString m_filePath[2] ;

    quint8  m_image[2][143360] ; // Holds contents of both floppy disks

    int     m_bufferIndex[2] ;
    quint8  m_buffer[2][256] ;
    quint8  m_rawBuffer [2][344] ;
    quint8  m_encodedBuffer[2][344] ;

    bool m_writingData ;
    int  m_diskAddressCounter ;
    int  m_dataPrologCounter ;
    
    const quint8 m_addressProlog[3] = {0xd5, 0xaa, 0x96} ;
    const quint8 m_dataProlog[3]    = {0xd5, 0xaa, 0xad} ;
    const quint8 m_epilogs[3]       = {0xde, 0xaa, 0xeb} ;

    int m_delayByte ;
    int m_delayInterSector ;
    int m_delayPostAddress ;


} ;

#endif

