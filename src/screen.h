/*****************************************************************************

    File: "screen.h"

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




#ifndef SCREENFRAME_H
#define SCREENFRAME_H  1


#include <QPaintDevice> 
#include <QPixmap> 
#include <QFrame> 
#include <QEvent> 
#include <QMainWindow> 

//#include "machine.h"



class Screen : public QWidget
{
public:

    Screen (QWidget *parent = 0) ;
    void initialize (void) ;
    void setScale (uint scale) ;
    void splashScreen (void) ;

private:

    void refreshScreen (void) ;
    void drawLine_40 (quint8 *srcOffsetArray, int dstX, int dstY) ;
    void drawLine_80 (int x, int y, quint8 *main, quint8 *aux) ;
    void draw40Column (int firstLine, quint8 *screen) ;
    void draw80Column (int firstLine) ;
    inline void put_7_bits (QPainter& painter, quint8* screenData, int x, int y, int col, int off) ;
    void drawLoRes (int nLines, quint8 *screen) ;
    void drawHiRes (quint8 *screen) ;
    void doubleHiRes4bytes (quint8* main, quint8* aux, int rowOffset, int row, int column) ;
    void writeDoubleHiRes (quint8 page2) ;
    void paintEvent (QPaintEvent *) ;

    const int m_pixelsWidth  = 564 ;
    const int m_pixelsHeight = 388 ;
    
    QTimer*  m_refreshTimer ;
    const int SLOW = 200 ;   // mSec delay for slow timer (for double hi-res)
    const int FAST = 50 ;    // "            " fast timer (for everything else)

    bool      m_flash ;         // Flag for flashing characters on screen
    int       m_flashCounter ;  // Counter for selecting which flash mask to apply

    uint     m_scale ;
    QPixmap  m_screenBuffer ;
    QPixmap  m_pixmap_40ColumnPrimary ;
    QPixmap  m_pixmap_40ColumnAlternate ;
    QPixmap  m_pixmap_80ColumnPrimary ;
    QPixmap  m_pixmap_80ColumnAlternate ;
    QPixmap  m_pixmap_xpm_hires_0 ;
    QPixmap  m_pixmap_xpm_hires_1 ;
    QPixmap  m_pixmap_empty_hires_line ;
    QPixmap *m_loResBlocks[16] ;
    QPixmap *m_dblHiResBlock[16] ;

} ;

#endif // SCREENFRAME_H

