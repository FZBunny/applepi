/*****************************************************************************

    File: "gamepad.cpp"

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


#include <stdio.h>
#include <QRect>
#include <QApplication>
#include <QDesktopWidget>
#include <QErrorMessage>
#include <QScreen>

#include "defs.h"
#include "gamepad.h"
#include "mainwindow.h"



Gamepad::Gamepad (MainWindow* parent) : QWidget ()
{
    m_parent = parent ;
    m_gamepad = new QGamepad (0, this) ;
    openController() ;

    for (int i=0; i<3; i++) m_buttons[i] = false ;
    m_triggerCycles = MAC->getCycles() ;

    QRect r = QApplication::desktop()->screenGeometry() ;
    m_screenWidth  = r.width() ;
    m_screenHeight = r.height() ;

}


void Gamepad::openController (void)
{
    QList<int> controllerIDList = QGamepadManager::instance()->connectedGamepads() ;
    if (controllerIDList.length()) {
        int id = controllerIDList.at(0) ;
        m_gamepad = new QGamepad (id, this) ;
    }
}


quint8 Gamepad::readButton (int buttonNumber)
{
//printf ("Gamepad::readButton %i\n", n) ;
    quint8 n = 0 ;

    QGamepad* g = m_gamepad ;
    switch (buttonNumber) {
        case 0:
            n = g->buttonL2() ;
            break ;
        case 1:
            n = g->buttonR2() ;
            break ;
        case 2:
            n = m_buttons[2] ;
            break ;
        case 3:
            n = m_buttons[3] ;
            break ;
        default:
            n = 0 ; 
            break ;
    }

    if (n) n = 0xff ; 
    return n ;
}


void Gamepad::reset (void)
{
    m_triggerCycles = MAC->getCycles() ;
}


// 

quint8 Gamepad::readAnalog (int n)
{
    float  f ;
    quint8 value = 0xff ;

    if (m_gamepad->isConnected()) {
        switch (n) {
            case 0:
                f = m_gamepad->axisLeftX() ;
                break ;
            case 1:
                f = m_gamepad->axisLeftY() ;
                break ;
            case 2:
                f = m_gamepad->axisRightX() ;
                break ;
            case 3:
                f = m_gamepad->axisRightY() ;
                break ;
            default:
                f = 0 ;
                break ;
        }
        quint64 deltaCycles = MAC->getCycles() - m_triggerCycles ;
        if (deltaCycles > ((f+0.5)*m_maxTimeoutCycles)) value = 0 ;
    } 

//printf ("value=%i\n", value) ;
    return value ;
}
