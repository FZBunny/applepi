/*****************************************************************************

    File: "paste_to_keyboard.cpp"

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



#include <QtCore/QString>
#include <QtGui/QClipboard>

#include "defs.h"
#include "machine.h"
#include "mainwindow.h"
#include "config.h"
#include "paste_to_keyboard.h"


Paste::Paste (MainWindow* parent, QString string)
{
    m_parent = parent ;
    m_string = string ;
}


void Paste::run (void)
{
    m_parent->pastingText(true) ;
//qStdOut() << "Paste::  text to paste: " << m_string << endl ;
    char c = 0 ;
    int  strLen = m_string.length() ;
    for (int i=0; i<strLen; i++) {
        c = m_string.at(i).cell() ;
        if ((c < 0x20) && (c != 0x0a) && (c != 0x0d)) c |= 0x40 ;
        if (c == 0x0a) c = 0x0d ;            // Convert <LF> to <CR>, which the Apple expects.
        MAC->m_ss[0] = c | 0x80 ;
        while (MAC->m_ss[0] & 0x80) usleep (10) ;
        if (MAC->m_ss[0] == 0x03) {         // Quit if <ctrl-C> was entered while we were sleeping.
            MAC->m_ss[0] = 0x8d ;
            break ;
        }
    }
    
    m_parent->pastingText(false) ;

    return ;

}
