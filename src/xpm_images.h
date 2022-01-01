/*****************************************************************************

    File: "xpm_images.h"

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




#ifndef XPM_IMAGES_H
#define XPM_IMAGES_H

extern const char* xpm_led_bright_red[] ;
extern const char* xpm_led_dim_red[] ;
extern const char* xpm_led_bright_green[] ;
extern const char* xpm_led_dim_green[] ;

extern const char* xpm_printerPins[] ;

extern const char* xpm_40ColChars[] ;
extern const char* xpm_40ColCharsEnhanced[] ;
//extern const char* xpm_40ColMouseText[] ;
extern const char* xpm_40ColCharsMono[] ;

extern const char* xpm_80ColChars[] ;
extern const char* xpm_80ColCharsEnhanced[] ;
//extern const char* xpm_80ColMouseText[] ;
extern const char* xpm_80ColCharsMono[] ;

extern const char* xpm_hires_0[] ;
extern const char* xpm_hires_1[] ;

extern const char* xpm_redapple[] ;


// This is a .wav file, rather than an XPM;
// just a convenient place to put it.

#include <QtGlobal>
extern const quint8* step1_wav ;


#endif

