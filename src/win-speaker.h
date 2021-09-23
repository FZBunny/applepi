/*****************************************************************************

    File: "win-speaker.h"

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

