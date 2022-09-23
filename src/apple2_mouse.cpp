/*****************************************************************************

    File: "apple2_mouse.cpp"

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


/*------------------------------------------------------------------------------------------------

    Information for this class taken from: "AppleMouse // User's Manual", currently
    available at (as of Sept.2022) :
    https://github.com/freitz85/AppleIIMouse/blob/master/AppleMouse%20II%20User's%20Manual.pdf
    and from:
    https://github.com/mamedev/mame/blob/master/src/devices/bus/a2bus/mouse.cpp (and .../mouse.h)

------------------------------------------------------------------------------------------------*/


#include  <QMouseEvent>
#include  <QPoint>

#include  "device_roms.h"
#include  "machine.h"
#include  "apple2_mouse.h"


Apple2Mouse::Apple2Mouse (int slot)
{
    m_slot = slot ;
    m_IOSlotStartAddr = 0xC080 + (m_slot << 4) ;
    m_romStartAddr = 0XC000 + (m_slot<<8) ;
    MAC->m_ram[0x0738+m_slot] = 0x80 ;
    CRA = CRB = DDRA = DDRB = IORA = IORB = 0 ; // (Initialize all 6520 registers)
//printf ("m_IOSlotStartAddr=%4.4X\n", m_IOSlotStartAddr) ;
}


void Apple2Mouse::mouseMoved (QMouseEvent *e)         // Called by 'Screen::mouseMoveEvent'
{
    QPoint p = e->pos() ;
    m_X = p.x()/2 ;
    m_Y = p.y()/2 ;
    
//------------------------
    MAC->m_ram[0x0478+m_slot] = m_X & 0xff ;
    MAC->m_ram[0x0578+m_slot] = m_X >> 8 ;
    MAC->m_ram[0x04f8+m_slot] = m_Y & 0xff ;
    MAC->m_ram[0x05f8+m_slot] = m_Y >> 8 ;
//------------------------

    IORA = m_X ;
    IORB = m_Y ;
//printf ("mouse move event x=%4.4i y=%4.4i\n", p.x(), p.y()) ;  fflush(stdout) ;
}


void Apple2Mouse::mousePressed (QMouseEvent *e)       // Called by 'Screen::mousePressEvent'
{
    QPoint p = e->pos() ;
    m_pressed_X = p.x() ;
    m_pressed_Y = p.y() ;
    MAC->m_ram[0x0778+m_slot] = 0x80 ;
// printf ("mousePressed x=%4.4i y=%4.4i\n", p.x(), p.y()) ;
}


void Apple2Mouse::mouseReleased (QMouseEvent*)        // Called by 'Screen::mouseReleaseEvent'
{
     MAC->m_ram[0x0778+m_slot] = 0x00 ;
// printf ("mouseReleased\n") ;
}


quint8 Apple2Mouse::mouseROMReferenced (quint16 ptr)  // Called by 'Machine::fetch_ioSpace' (In fetch.cpp)
{
    quint8  c ;
    m_pc = MAC->savedPC() ;
    int  loByte = ptr & 0xff ;

    if (ptr < (m_romStartAddr+0x100)) {
        c = mouse_rom_3420270C [loByte] ;
    } else {                               // Must be a fetch address >= C800
        if (ptr > 0xc800) ptr -= 0xc700 ;  // Fold addrs above C4ff to C800-CFFF part of ROM
        c = mouse_rom_3420270C [ptr] ;
    }

    return c ;
}


quint8 Apple2Mouse::PIA_fetch (quint16 ptr)           // Called by 'Machine::fetch_sspage' (In fetch.cpp)
{
    quint8 c ;
    int registerNo = ptr - m_IOSlotStartAddr ;
    switch (registerNo) {
        case 0:
            if (CRA && 0x04) {
                c = DDRA ;
printf ("Fetch mouse DDRA=%2.2X  savedPC=%4.4X\n", c, MAC->savedPC()) ;
            } else  {
                c = IORA ;
printf ("Fetch mouse PIA =%2.2X  savedPC=%4.4X\n", c, MAC->savedPC()) ;
            }
            break ;
        case 1:
            c = CRA ;
printf ("Fetch mouse  CRA=%2.2X  savedPC=%4.4X\n", c, MAC->savedPC()) ;
            break ;
        case 2:
            if (CRB && 0x04) {
                c = DDRB ;
printf ("Fetch mouse DDRB=%2.2X  savedPC=%4.4X\n", c, MAC->savedPC()) ;
             } else {
                c = IORB ;
printf ("Fetch mouse PIB =%2.2X  savedPC=%4.4X\n", c, MAC->savedPC()) ;
             }
            break ;
        case 3:
            c = CRB ;
printf ("Fetch mouse  CRB=%2.2X  savedPC=%4.4X\n", c, MAC->savedPC()) ;
            break ;
        default:
            fprintf (stderr, "Apple2Mouse::PIA_fetch: Fetch of invalid register number (%i) at PC=%4.4X\n", registerNo, MAC->savedPC()) ;
            break ;
    }

//printf ("Fetch mouse PIA savedPC=%4.4X  ptr=%4.4X  c=%2.2X\n", MAC->savedPC(), ptr, c) ;
    return c ;
}


void Apple2Mouse::PIA_store (quint16 ptr, quint8 c)
{   
    int registerNo = ptr - m_IOSlotStartAddr ;
    switch (registerNo) {
        case 0:
            if (CRA && 0x04) {
                DDRA = c ;
printf ("Store mouse DDRA=%2.2X  savedPC=%4.4X\n", c, MAC->savedPC()) ;
            } else {
                IORA = c ;
printf ("Store mouse IORA=%2.2X  savedPC=%4.4X\n", c, MAC->savedPC()) ;
            }
            break ;
        case 1:
            CRA = c ;
printf ("Store mouse  CRA=%2.2X  savedPC=%4.4X\n", c, MAC->savedPC()) ;
            break ;
        case 2:
            if (CRB && 0x04) {
printf ("Store mouse DDRB=%2.2X  savedPC=%4.4X\n", c, MAC->savedPC()) ;
                DDRB = c ;
            } else {
printf ("Store mouse IORB=%2.2X  savedPC=%4.4X\n", c, MAC->savedPC()) ;
                IORB = c ;
            }
            break ;
        case 3:
            CRB = c ;
printf ("Store mouse  CRB=%2.2X  savedPC=%4.4X\n", c, MAC->savedPC()) ;
            break ;
        default:
            fprintf (stderr, "Apple2Mouse::PIA_store: Store invalid register number (%i) at PC=%4.4X\n", registerNo, MAC->savedPC()) ;
            break ;
    }
//printf ("Store mouse PIA savedPC=%4.4X  ptr=%4.4X  c=%2.2X\n", MAC->savedPC(), ptr, c) ;
}