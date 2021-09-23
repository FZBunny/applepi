/*****************************************************************************

    File: "defs.cpp"

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


#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include <QWindow>
#include <QDialog>


#include "defs.h"

/*
void centerDialog (MainWindow* parent, QDialog* d, int width, int height)
{
    d->resize (width,height) ;
    QPoint pPos = parent->pos() ;
    QSize pSize = parent->size() ;
    int x = ((pPos.x()+pSize.width()/2) - width/2) ;
    int y = ((pPos.y()+pSize.height()/2) - height/2) ;
	d->move (x,y) ;
}
*/

// Trim trailing spaces from a character string

void trim (char* buffer, int len)
{
    int i ;
    for (i=len-1; i>=0; i--) {
        if (buffer[i] != 0x20) break ;
    }
    buffer[i+1] = 0 ;

}


// A Qt-friendly replacement for 'cout'

QTextStream& qStdOut()
{
    static QTextStream ts (stdout) ;
    return ts;
}

