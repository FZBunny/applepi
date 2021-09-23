
/*****************************************************************************

    File: "tape.h"

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
