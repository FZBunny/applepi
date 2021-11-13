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
#include <QMouseEvent>
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

    m_mouseX = 0 ;
    m_mouseY = 0 ;

    QRect r = QApplication::desktop()->screenGeometry() ;
    m_screenWidth  = r.width() ;
    m_screenHeight = r.height() ;

    m_mouseTimer = new QTimer (this) ;
    connect (m_mouseTimer, &QTimer::timeout, this, &Gamepad::readMouseButtons) ;
    m_mouseTimer->start (50) ;
}


void Gamepad::openController (void)
{
    QList<int> controllerIDList = QGamepadManager::instance()->connectedGamepads() ;
    if (controllerIDList.length()) {
        int id = controllerIDList.at(0) ;
        m_gamepad = new QGamepad (id, this) ;
        m_useMouse = false ;
    } else {
        m_useMouse = true ;
        m_mouseFd = open ("/dev/input/mouse0",  O_RDONLY|O_NONBLOCK) ;
        if (m_mouseFd < 0) {
            char* text = (char*)"\n"
                "*  Could not open /dev/input/mouse0.\n"
                "*  You will not be able to use mouse buttons\n"
                "*  when using the mouse as a substitute gamepad.\n"
                "*  Use 'Help' to learn how to fix this." ;
            fprintf (stderr, "\n%s\n\n", text) ;
        }
    }
}


quint8 Gamepad::readButton (int buttonNumber)
{
//printf ("Gamepad::readButton %i\n", n) ;
    quint8 n = 0 ;

    if (m_useMouse) {
        switch (buttonNumber) {
            case 0:
                n = m_buttons[0] ;
                break ;
            case 1:
                n = m_buttons[1] ;
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
    } else {
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
    }

    if (n) n = 0xff ; 
//if (buttonNumber==1) printf ("x%i", n) ;
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
    } else {
        QPoint pos = QCursor::pos() ;
        switch (n) {
            case 0:
                f = (float)pos.x()/(float)m_screenWidth ;
                break ;
            case 1:
                f = (float)pos.y()/(float)m_screenHeight ;
                break ;
            default:
                f = 0 ;
                break ;
        }
        
    }

    quint64 deltaCycles = MAC->getCycles() - m_triggerCycles ;
    quint8 value = 0xff ;
    if (deltaCycles > ((f+0.5)*m_maxTimeoutCycles)) value = 0 ;

    return value ;
}


void buttonBChanged(bool value)
{
printf ("buttonB = %i\n", value) ;
}


// -----------  Mouse Buttons  ---------------


typedef struct mouseRecord {
    quint8 buttons ;
    quint8 x ;
    quint8 y ;
} mouseRecord ;


void Gamepad::readMouseButtons (void)
{
    mouseRecord mouse ;
    int n = read (m_mouseFd, &mouse, sizeof(mouse)) ;
    if ((n==EAGAIN) || (n==EWOULDBLOCK)) return ; 

    m_buttons[0] = m_buttons[1] = m_buttons[2] = 0 ;
    if (mouse.buttons & 1)  m_buttons[0] = 0xff ;  ;
    if (mouse.buttons & 2)  m_buttons[1] = 0xff ;  ;
    if (mouse.buttons & 4)  m_buttons[2] = 0xff ;  ;
}
