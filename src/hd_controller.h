/*****************************************************************************

    File: "hd_controller.h"

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




#ifndef HD_CONTROLLER_H
#define HD_CONTROLLER_H

//#include <wx/string.h>
//#include <wx/filename.h>

#include <QtGlobal>
#include <QFile>
#include <QFileInfo>

//#include "mainwindow.h"
//#include "machine.h"

class Machine ;


class HdController
{

public:

    HdController (Machine* parent) ;
    ~HdController(void) ;

    quint8 fetch_HD_ROM (int slotNumber, quint8 loByte) ; // called from Machine::fetch

    bool open   (QString &path, int driveIndex) ;
    bool create (QString &path, int drive, int size) ;
    void close  (void) ;
    void close (int drive) ;
    bool isOpen (void) ;
    bool isOpen (int driveIndex) ;
    void offsetKludge (void) ;

    int fetchStatusByteFE (void) ;
    int IO     (void) ;
    int status (void) ;
    int readBlock  (quint16 buffer, int block) ;
    int readBlock  (quint8* buffer, int block, int nBlocks) ;
    int writeBlock (quint16 buffer, int block) ;
    int format (void) ;

    bool smartPort (void) ;
    bool sp_readblock (void) ;

    void   headStepDelay (int block) ;
    quint8 getDiskSize (quint8 byteNumber) ;
    bool   writeProtected (void) ;


private:

    quint8* address (quint16 address, bool write) ;
    void    adjustReturnAddress (int nBytes) ;

    const int IOERROR   = 0x27 ;
    const int NODEVICE  = 0x28 ;
    const int WRITE_PROTECTED = 0x2b ;

    Machine*    m_mac ;

    int         m_driveIndex ;
    QFile       m_file[2] ;
    QFileInfo   m_fileInfo[2] ;
    QString     m_configuredPath[2] ;
    bool        m_writeable[2] ;
    int         m_offset[2] ;
    qulonglong  m_fileSize[2] ;
    int         m_previousTrack[2] ;

    quint16     m_stackPtr ;
    quint16     m_retAddr ;
    quint16     m_paramPtr ;


} ;

#endif
