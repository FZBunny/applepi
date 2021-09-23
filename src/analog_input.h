/*****************************************************************************

    File: "analog_input.h"

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





class AnalogInput
{

public:

    AnalogInput(void) ;
    ~AnalogInput() ;

    void init (void) ;
    void reset (void) ;

    unsigned short readPaddle (int paddle) ;
    unsigned short readButton (int button) ;

    enum {
        MOUSE,
        JOY0,
        JOY1,
    } ;

private:

    int js0 ;
    int js1 ;

    int limitMegaCycles[4] ;
    int limitCycles[4] ;
} ;


