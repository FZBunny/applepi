
/*****************************************************************************

    File: "win-speaker.h"

    Copyright (C) 2003,2019 Bruce Ward

    This file is part of ApplePi, an Apple2 Emulator.
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


#ifndef WIN_SPEAKER_H
#define WIN_SPEAKER_H

#include <wx/defs.h>
#include <wx/thread.h>
#include <wx/timer.h>
#include <wx/event.h>


//#include "defs.h"
#include "mainwindow.h"

class MainWindow ;

class Speaker : public wxThread, public wxEvtHandler
{
public:

    Speaker (MainWindow* parent) ;
    ~Speaker(void) {} ;

    void setVolume (float value) ;
    void toggleSpeaker (quint64 processorCycles) ;   // Called in Machine::fetch_sspage upon fetch from $C030
 //   void playRecal (void) ;
 //   void playStep  (void) ;

private:

    virtual void *Entry (void) ;

    MainWindow*  m_parent ;
    
    static const int MID = 0x80 ;
    uint  m_lo ;
    uint  m_hi ;

} ;

#endif

