/*****************************************************************************

    File: "gamepad.h"

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


#ifndef  JOYSTICK_H
#define  JOYSTICK_H


#include <QtGlobal>
#include <QList>
#include <QCursor>
#include <QScreen>
#include <QTimer>
#include <QMouseEvent>
#include <QGamepad>

#include "mainwindow.h"

class MainWindow ;


class Gamepad : public QWidget
{

public:

    Gamepad (MainWindow* parent) ;
    ~Gamepad() {} ;

    quint8 readButton (int n) ;
    quint8 readGamepad (int n) ;
    quint8 readMouse (int n) ;
    void   openController (void) ;
    void   reset (void) ;
    
private:

    MainWindow* m_parent ;
    void readMouseData (void) ;
    void buttonBChanged(bool value) ;

    QString    m_controllerName ;
    uint       m_controllerID ;
    bool       m_useMouse ;
    int        m_mouseFd ;
    QList<int> m_gamepadIDs ;
    QGamepad*  m_gamepad ;
    bool       m_arcadeStyle ;
    bool       m_buttons[4] ;
    int        m_coarseDeltaX ;
    int        m_coarseDeltaY ;
    quint64    m_triggerCycles ;
    QTimer*    m_mouseTimer ;
    
   
    const float m_maxTimeoutCycles = 11*255 ; // Maximum CPU cycles for paddle circuit time-out ( = 2805)
    const float m_coarseK = 11*255 / 127 ;    // /dev/input/mice position-to-timeout conversion  constant

} ;

#endif
