/*****************************************************************************

    File: "analog_input.h"

    Copyright (C) 2003,2019 Bruce Ward

    This file is part of ApplePi, an Apple2 emulator.
    (Apple2 is a trademark of Apple, Inc.)

    "ApplePi" is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    "ApplePi" is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You may obtain a copy of the GNU General Public License by writing to:

    The Free Software Foundation, Inc.
    51 Franklin Street, Fifth Floor
    Boston, MA 02110-1335 USA

    Or simply visit: https://www.gnu.org/licenses/gpl-3.0.html

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


