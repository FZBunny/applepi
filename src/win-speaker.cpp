/*****************************************************************************

    File: "win-speaker.cpp"

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


#include "win-speaker.h"
#include "mainwindow.h"

#pragma message ("************  compiling win-speaker.cpp")

Speaker::Speaker (MainWindow* parent)
{
    m_parent = parent ;
}


void Speaker::toggleSpeaker (void)
{

    
}


void* Speaker::Entry (void)
{
    for (;;) {
        Sleep(1) ;
    }
}


void Speaker::setVolume (float value)
{
    if (value > 100) value = 100 ;
    if (value < 0)   value = 0 ;

    float volume = 1.28 * (value*value / 100) ;

    m_hi = MID + volume ;
    m_lo = MID - volume ;
//printf ("MID=%i  value=%f  volume=%f m_hi=%i  m_lo=%i\n", MID, value, volume, m_hi, m_lo) ;
}
