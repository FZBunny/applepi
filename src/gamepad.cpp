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

#include "defs.h"
#include "gamepad.h"
#include "mainwindow.h"



Gamepad::Gamepad (MainWindow* parent) : QWidget ()
{
    m_parent = parent ;
    openController() ;

    for (int i=0; i<3; i++) m_buttons[i] = false ;
    m_triggerCycles = MAC->getCycles() ;

    m_mouseTimer = new QTimer (this) ;
    connect (m_mouseTimer, &QTimer::timeout, this, &Gamepad::readMouseData) ;
    m_mouseTimer->start (49) ;
}


void Gamepad::openController (void)
{
    QList<int> controllerIDList = QGamepadManager::instance()->connectedGamepads() ;
    if ( controllerIDList.length()) {
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
        if (g->isConnected()) {
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
        } else {
            n = 0xff ;
        }
    }

    if (n) n = 0xff ; 
//if (buttonNumber==1) printf ("x%i", n) ;
    return n ;
}

/*
bool l1, l2, l3, r1, r2, r3 ;
bool center, down, up, left, right, guide, select, start ;
bool a, b, x, y ;

l1 = g->buttonL1(); l2 = g->buttonL2(); l3 = g->buttonL3(); 
r1 = g->buttonR1(); r2 = g->buttonR2(); r3 = g->buttonR3(); 
printf ("%i%i%i %i%i%i\n", l1, l2, l3, r1, r2, r3 ) ;

center = g->buttonCenter(); down = g->buttonDown(); up = g->buttonUp();
left = g->buttonLeft(); right = g->buttonRight(); 
select = g->buttonSelect(); start = g->buttonStart(); guide = g->buttonGuide() ;
printf ("%i %i%i%i%i %i%i%i\n", center, down, up, left, right, guide, select, start) ;

a = g->buttonA(); b = g->buttonB(); x = g->buttonX(); y = g->buttonY(); 
printf ("%i%i %i%i\n", a, b, x, y) ;
printf ("\n") ;
*/

void Gamepad::reset (void)
{
    m_triggerCycles = MAC->getCycles() ;
}


// 

quint8 Gamepad::readGamepad (int n)
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
        f = 1.0 ;
    }

    quint64 deltaCycles = MAC->getCycles() - m_triggerCycles ;
    quint8 value = 0xff ;
    if (deltaCycles > ((f+0.5)*m_maxTimeoutCycles)) value = 0 ;
//if (value==0) printf ("n=%i dCyc=%5lli f=%4.2f\n", n, deltaCycles, f+0.5) ;
    return value ;
}


void buttonBChanged(bool value)
{
printf ("buttonB = %i\n", value) ;
}



// -----------  Mouse ---------------


quint8 Gamepad::readMouse (int n)
{
    QRect r = QApplication::desktop()->screenGeometry() ;
    int w = r.width() ;
    int h = r.height() ;

    QPoint p = QCursor::pos() ;

    int position ;
    float mouseMax ;
    if (n) {
        mouseMax = h ;
        position = p.y() ;
    } else {
        mouseMax = w ;
        position = p.x() ;
    }

    quint64 deltaCycles = MAC->getCycles() - m_triggerCycles ;
    float cyclesPerPixel = m_maxTimeoutCycles/mouseMax ;
    quint8 value = 0xff ;
    if (deltaCycles > (position*cyclesPerPixel)) value = 0 ;

    return value ;
}


typedef struct mouseRecord {
    quint8 buttons ;
    quint8 x ;
    quint8 y ;
} mouseRecord ;

//  Mouse movement readings from /dev/input/mice
//
//       left/down  |  right/up   
//     <--fast    slow     fast-->
//     $81  ..  $ff | $01  ..  $7f
//    -127  ..   -1 |  +1  .. +127

void Gamepad::readMouseData (void)
{
    mouseRecord mouse ;
    int n = read (m_mouseFd, &mouse, sizeof(mouse)) ;
    if ((n==EAGAIN) || (n==EWOULDBLOCK)) return ; 
//putchar('-'); fflush(stdout) ;
    m_buttons[0] = m_buttons[1] = m_buttons[2] = 0 ;
    if (mouse.buttons & 1)  m_buttons[0] = 0xff ;  ;
    if (mouse.buttons & 2)  m_buttons[1] = 0xff ;  ;
    if (mouse.buttons & 4)  m_buttons[2] = 0xff ;  ;

/***
    m_coarseDeltaX = mouse.x ;
    if (mouse.x & 0x80) m_coarseDeltaX = m_coarseDeltaX - 256 ;
    m_coarseDeltaY = mouse.y ;
    if (mouse.y & 0x80) m_coarseDeltaY = m_coarseDeltaY - 256 ;
***/
//printf ("m_coarseDelta: %4i %4i\n", m_coarseDeltaX, m_coarseDeltaY) ;
}


