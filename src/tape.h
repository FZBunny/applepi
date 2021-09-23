/*****************************************************************************

    File: "tape.h"

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




#ifndef TAPE_H
#define TAPE_H

#include <wx/string.h>
#include <wx/filename.h>

#include "mainwindow.h"
#include "machine.h"

class Machine ;


class Tape
{

public:

    Tape (Machine* parent) ;
    ~Tape(void) ;

    int  open   (QString &path, bool writeable) ;
    int  create (QString &path) ;
    void close (void) ;
    void rewind (void) ;

    bool hasTape (void) ;
    bool writeProtected (void) ;
    void setWriteableFlag (bool flag) ;

    quint8 readTapeInput (void) ;
    void    toggleTapeOutput (quint64 processorCycles) ;


private:

    void    initialize (void) ;
    quint8 readTapeHeader (void) ;
    quint8 readSync (void) ;
    bool    shiftIn (int bit) ;
    bool    tapeIsInDrive (void) ;
    void    toggleSpeaker (void) ;

    quint8 m_bitMasks[8] ;
    const unsigned int  m_cyclesFor_0 = 250 ;     // Nominally, 250 uSec for half cycle of 2 kHz tone ('0' bit)
    const unsigned int  m_cyclesFor_1 = 500 ;     // Nominally, 500 uSec for half cycle of 1 kHz tone ('1' bit)
    quint8 m_byte ;
    quint8 m_checksum ;
    bool    m_firstOutputCall ;
    int     m_inputState ;
    int     m_outputState ;
    int     m_syncState ;
    int     m_inputHeaderCycleCount ;
    bool    m_fullCycle ;
    int     m_bitNumber ;
    quint8 m_previousBit7Value ;

    quint64   m_previousCycles ;
    bool       m_writeable ;
    bool       m_tapeInDriveFlag ;
    QFile     m_file ;
    QFileInfo m_fileName ;
    QString   m_configuredPath ;

    Machine* m_parent ;

} ;

#endif
