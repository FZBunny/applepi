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


/*--------------------------------------------------------------------------------------------

    Information for this class largely taken from: "AppleMouse // User's Manual", currently
    available at (as of Sept.2022) :
    https://github.com/freitz85/AppleIIMouse/blob/master/AppleMouse%20II%20User's%20Manual.pdf

--------------------------------------------------------------------------------------------*/


#include  <QMouseEvent>
#include  <QPoint>

#include  "device_roms.h"
#include  "machine.h"
#include  "apple2_mouse.h"


Apple2Mouse::Apple2Mouse (int slot)
{
    m_slot = slot ;
    m_romStartAddr = 0XC000 + (m_slot<<8) ;
    MAC->m_ram[0x0738+m_slot] = 0x80 ;
}


void Apple2Mouse::mouseMoved (QMouseEvent *e)         // Called by 'Screen::mouseMoveEvent'
{
    QPoint p = e->pos() ;
    m_X = p.x()/2 ;
    m_Y = p.y()/2 ;
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


/*******
void Apple2Mouse::PIA_store (quint8)
{

}
*******/


quint8 Apple2Mouse::mouseROMReferenced (quint16 ptr)  // Called by 'Machine::fetch_ioSpace' (In fetch.cpp)
{
    static int RTS = 0x60 ;

    quint8  c ;
    m_pc = MAC->savedPC() ;
    bool aJSR = false ;
    if (m_pc == ptr) aJSR = true ;
    int  loByte = ptr & 0xff ;

    if (ptr < (m_romStartAddr+0x100)) {

        if (aJSR) {                        // This is a JSR call 

            clearCarry() ;   // (Clear carry bit; functions may set it to indicate errors)

            switch (loByte) {
                case 0xb3:
                    setMouse() ;
                    c = RTS ;
                    break ;
                case 0xc4:
                    serveMouse() ;
                    c = RTS ;
                    break ;
                case 0x9b:
                    readMouse() ;
                    c = RTS ;
                    break ;
                case 0xa4:
                    clearMouse() ;
                    c = RTS ;
                    break;
                case 0xc0:
                    posMouse() ;
                    c = RTS ;
                    break ;
                case 0x8a:
                    clampMouse() ;
                    c = RTS ;
                    break ;
                case 0xdd:
                    homeMouse() ;
                    c = RTS ;
                    break ;
                case 0xbc:
                    initMouse() ;
                    c = RTS ;
                    break ;
                default:
                    c = mouse_rom_3420270C [loByte] ;
                    break ;
            }
        } else {
            c = mouse_rom_3420270C [loByte] ;
        }

    } else {                               // Must be a fetch address >= C800
        if (ptr > 0xc800) ptr -= 0xc700 ;  // Fold addrs above C4ff to C800-CFFF part of ROM
        c = mouse_rom_3420270C [ptr] ;
    }

    return c ;
}
//printf ("Mouse fetched %2.2X from %4.4X; savedPC=%4.4X\n", c, ptr, m_pc) ; fflush(stdout) ;


void Apple2Mouse::setCarry (void)
{
    MAC->processorState()->Pstat |= C ;
}


void Apple2Mouse::clearCarry (void)
{
     MAC->processorState()->Pstat &= (C^0xff) ;
}


void Apple2Mouse::setMouse (void)
{
printf ("setMouse:    savedPc = %4.4X, A = %2.2X\n", m_pc, MAC->processorState()->Areg) ;
    MAC->m_ram[0x0738+m_slot] = MAC->processorState()->Areg ;
    if (MAC->processorState()->Areg > 0x0f) setCarry() ; // Set carry bit on illegal value

    return ;
}


void Apple2Mouse::serveMouse (void)
{
printf ("serveMouse:  savedPc = %4.4X\n", m_pc) ;
    return ;
}


void Apple2Mouse::readMouse (void)
{
    MAC->m_ram[0x0478+m_slot] = m_X & 0xff ;
    MAC->m_ram[0x0578+m_slot] = m_X >> 8 ;
    MAC->m_ram[0x04f8+m_slot] = m_Y & 0xff ;
    MAC->m_ram[0x05f8+m_slot] = m_Y >> 8 ;
//printf ("readMouse:   savedPc = %4.4X\n", m_pc) ;
    return ;
}


void Apple2Mouse::clearMouse (void)
{
printf ("clearMouse:  savedPc = %4.4X\n", m_pc) ;
    return ;
}


void Apple2Mouse::posMouse (void)
{
printf ("posMouse:    savedPc = %4.4X\n", m_pc) ;
    return ;
}


void Apple2Mouse::clampMouse (void)   //  What to do about thr received limits?   XXXXX FIXME XXXXX
{
int AReg = MAC->processorState()->Areg ;
printf ("clampMouse  A=%2.2X  LO:%2.2X.%2.2X  HI:%2.2X.%2.2X\n",
AReg, MAC->m_ram[0x0578], MAC->m_ram[0x0778], MAC->m_ram[0x05f8], MAC->m_ram[0x04f8]) ;

    return ;
}


void Apple2Mouse::homeMouse (void)
{
printf ("homeMouse:   savedPc = %4.4X\n", m_pc) ;
    return ;
}


void Apple2Mouse::initMouse (void)
{
printf ("initMouse:   savedPc = %4.4X\n", m_pc) ;
    return ;
}

