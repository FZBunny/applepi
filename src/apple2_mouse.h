/*****************************************************************************

    File: "apple2_mouse.h"

    Copyright (C) 2022, Bruce Ward

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

#ifndef APPLE2MOUSE_H
#define APPLE2MOUSE_H

#include  <QMouseEvent>


class Apple2Mouse
{
public:
    Apple2Mouse (int slot) ;
    ~Apple2Mouse() {} ;

    void mouseMoved    (QMouseEvent *e) ;
    void mousePressed  (QMouseEvent *e) ;
    void mouseReleased (QMouseEvent *e) ;

    quint8 mouseROMReferenced (quint16 pc) ;

private:
    int m_slot ;
    int m_romStartAddr ;
    int m_X, m_Y ;
    int m_pressed_X, m_pressed_Y ;
    quint16 m_pc ;

    void setMouse (void) ;
    void serveMouse( void) ;
    void readMouse (void) ;
    void clearMouse (void) ;
    void posMouse (void) ;
    void clampMouse (void) ;
    void homeMouse (void) ;
    void initMouse (void) ;

    void setCarry (void) ;
    void clearCarry (void) ;
} ;

#endif
