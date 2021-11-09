/*****************************************************************************

    File: "defs.h"

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


#ifndef MISC_DEFS_H
#define MISC_DEFS_H

#include <QtGlobal>
#include <QTextStream>
#include <QWindow>
#include <QDialog>

#include "mainwindow.h"


QTextStream& qStdOut() ;        // A Qt-friendly replacement for 'cout'


#define CFG main_window->m_cfg
#define MAC main_window->m_mac


extern "C" void trim (char* buffer, int len) ;
//extern "C" void centerDialog (MainWindow* parent, QDialog* dialog, int width, int height) ;

#define SHARE_DIR "/usr/share/applepi/"

#define  MIN_MAINWINDOW_WIDTH  577
#define  MIN_MAINWINDOW_HEIGHT 580
//#define  MIN_MAINWINDOW_HEIGHT 600

#define  MAX_MAINWINDOW_WIDTH  1138
#define  MAX_MAINWINDOW_HEIGHT 968
//#define  MAX_MAINWINDOW_HEIGHT 989

#define  SCREEN_PIXELS_WIDTH  560
#define  SCREEN_PIXELS_HEIGHT 384

#define SECTORSIZE 256
#define BLOCKSIZE  512

// disk types
#define DOS3    0
#define PRODOS  1
#define PASCAL  2
#define UNKNOWN 3

#define CENTER_DIALOG \
  { \
    resize (width,height) ; \
    QPoint pPos = parent->pos() ; \
    QSize pSize = parent->size() ; \
    x = ((pPos.x()+pSize.width()/2) - width/2) ; \
    y = ((pPos.y()+pSize.height()/2) - height/2) ; \
    move (x, y) ; \
  }

enum romNumber {
    APPLE2,
    APPLE2PLUS,
    APPLE2E,
    APPLE2E_ENHANCED,
    APPLE_UNKNOWN
} ;


#endif // MISC_DEFS_H
