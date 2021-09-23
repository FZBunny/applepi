
/*****************************************************************************

    File: "hd_controller.h"

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



#ifndef HD_CONTROLLER_H
#define HD_CONTROLLER_H

//#include <wx/string.h>
//#include <wx/filename.h>

//#include "mainwindow.h"
#include "machine.h"

class Machine ;


class HdController
{

public:

    HdController (Machine* parent) ;
    ~HdController(void) ;

    bool open   (QString &path, int driveIndex) ;
    bool  create (QString &path, int drive, int size) ;
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

    bool smartPort (quint8 cmd, quint8* m_ram, quint16 paramPtr) ;
    bool smartPortStatus (quint8* m_ram, quint16 paramPtr) ;

    void    headStepDelay (int block) ;
    quint8 getDiskSize (quint8 byteNumber) ;
    bool    writeProtected (void) ;


private:

    const int IOERROR   = 0x27 ;
    const int NODEVICE  = 0x28 ;
    const int WRITE_PROTECTED = 0x2b ;

    int         m_driveIndex ;
    QFile       m_file[2] ;
    QFileInfo   m_fileInfo[2] ;
    QString     m_configuredPath[2] ;
    bool        m_writeable[2] ;
    int         m_offset[2] ;
    qulonglong  m_fileSize[2] ;
    int         m_previousTrack[2] ;

    Machine*    m_parent ;

} ;

#endif
