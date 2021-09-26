/*****************************************************************************

    File: "screen.cpp"

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



#include <QApplication> 
#include <QFileDialog> 
#include <QPixmap> 
#include <QBitmap> 
#include <QColor> 
#include <QPoint> 
#include <QFile> 
#include <QStatusBar>
#include <QEvent>
#include <QPaintDevice>
#include <QPainter>
#include <QPaintEvent>

#include <QImage>
#include <QClipboard>

#include <unistd.h>

#include "machine.h"
#include "version.h"
#include "defs.h"
#include "xpm_images.h"
#include "screen.h"



enum {
    MODE_40ColumnPage1,
    MODE_40ColumnPage2,
    MODE_loresPage1,
    MODE_loresPage2,
    MODE_loresMixedPage1,
    MODE_loresMixedPage2,
    MODE_hiresPage1,
    MODE_hiresPage2,
    MODE_hiresMixedPage1,
    MODE_hiresMixedPage2,
    MODE_80Column
} ;


static quint16 screen_RdTEXT_offsets[24] = {
    0x000, 0x080, 0x100, 0x180, 0x200, 0x280, 0x300, 0x380,
    0x028, 0x0a8, 0x128, 0x1a8, 0x228, 0x2a8, 0x328, 0x3a8,
    0x050, 0x0d0, 0x150, 0x1d0, 0x250, 0x2d0, 0x350, 0x3d0
} ;


Screen::Screen (QWidget *parent) : QWidget (parent)
{
 //   this->setAutoFillBackground (true) ;
    initialize() ;
}


void Screen::initialize (void)
{
    m_pixmap_40ColumnPrimary16 = QPixmap(xpm_40ColChars) ;
    m_pixmap_80ColumnPrimary16 = QPixmap(xpm_80ColChars) ;

    m_pixmap_xpm_hires_0 = QPixmap(xpm_hires_0) ;
    m_pixmap_xpm_hires_1 = QPixmap(xpm_hires_1) ;

    m_pixmap_empty_hires_line = QPixmap (280, 1) ;
    m_pixmap_empty_hires_line.fill (Qt::black) ;

    for (int i=0; i<16; i++) {
        m_loResBlocks[i] = new QPixmap(16,16) ;
    }

    QColor black   = QColor (0,0,0) ;
    QColor magenta = QColor (200,0,100) ;
    QColor dblue   = QColor (0,0,226) ;
    QColor purple  = QColor (240,30,240) ;
    QColor dgreen  = QColor (10,100,10) ;
    QColor grey1   = QColor (128,128,128) ;
    QColor mblue   = QColor (100,100,255) ;
    QColor lblue   = QColor (150,200,255) ;
    QColor brown   = QColor (110,110,70) ;
    QColor orange  = QColor (255,128,0) ;
    QColor grey2   = QColor (135,135,135) ;
    QColor pink    = QColor (255,192,160) ;
    QColor lgreen  = QColor (70,200,70) ;
  //  QColor yellow  = QColor (230,255,0) ;
    QColor yellow  = QColor (191,204,128) ;
  //  QColor aqua    = QColor (0,255,255) ;
    QColor aqua    = QColor (140,220,190) ;
    QColor white   = QColor (255,255,255) ;

//  Lo-res colors

    m_loResBlocks[0]->fill (black) ;   // black
    m_loResBlocks[1]->fill (magenta) ; // magenta
    m_loResBlocks[2]->fill (dblue) ;   // dark blue
    m_loResBlocks[3]->fill (purple) ;  // purple
    m_loResBlocks[4]->fill (dgreen) ;  // dark green
    m_loResBlocks[5]->fill (grey1) ;   // grey 1
    m_loResBlocks[6]->fill (mblue) ;   // medium blue
    m_loResBlocks[7]->fill (lblue) ;   // light blue
    m_loResBlocks[8]->fill (brown) ;   // brown
    m_loResBlocks[9]->fill (orange) ;  // orange
    m_loResBlocks[10]->fill (grey2) ;  // grey 2
    m_loResBlocks[11]->fill (pink) ;   // pink
    m_loResBlocks[12]->fill (lgreen) ; // light green
    m_loResBlocks[13]->fill (yellow) ; // yellow
    m_loResBlocks[14]->fill (aqua) ;   // 'aquamarine' = cyan?
    m_loResBlocks[15]->fill (white) ;  // white

// Double hi-res colors

    QColor color[16] ;
    color[0]  = black ;
    color[1]  = dblue ;
    color[2]  = dgreen ;
    color[3]  = mblue ; 
    color[4]  = brown ;
    color[5]  = grey2 ;
    color[6]  = lgreen ;
    color[7]  = aqua ;
    color[8]  = magenta ;
    color[9]  = purple ;
    color[10] = grey1 ;
    color[11] = lblue ;
    color[12] = orange ;
    color[13] = pink ;
    color[14] = yellow ;
    color[15] = white ;

    for (int i=0; i<16; i++) {
        m_dblHiResBlock[i] = new QPixmap(4,2) ;
        m_dblHiResBlock[i]->fill (color[i]) ;
    }

    m_screenBuffer = QPixmap (m_pixelsWidth, m_pixelsHeight) ;
    m_screenBuffer.fill (Qt::black) ;

 //   m_usingSlowTimer = false ;

    m_refreshTimer = new QTimer() ;
    connect (m_refreshTimer, &QTimer::timeout, this, &Screen::refreshScreen) ;
    m_refreshTimer->start (FAST) ;
 
    this->show() ;
    this->update (this->rect()) ;

}


void Screen::setScale (uint scale)
{
    m_scale = scale ;

    QSize size ;
    if (scale == 1) size = QSize (SCREEN_PIXELS_WIDTH+4, SCREEN_PIXELS_HEIGHT+4) ;
    else            size = QSize (2*SCREEN_PIXELS_WIDTH+4, 2*SCREEN_PIXELS_HEIGHT+4) ;

    this->resize (size) ;
}


void Screen::splashScreen (void)
{
    QString splashText1 ("ApplePi") ;
    QString splashText2 ("the") ;
    QString splashText3 ("Apple //e") ;
    QString splashText4 ("Emulator") ;
    QString splashText5 ("for the Raspberry Pi (and others)") ;
    QString splashText6 (QString("v") + QString(APPLEPI_VERSION)) ;

    QPainter painter(this) ;
    painter.fillRect (this->rect(), QColor(0,0,0)) ;

    QSize screenSize = this->size() ;
    int midX = screenSize.width()/2 ;
    int midY = screenSize.height()/2 ;
    
    QPen gPen (QColor(0x61,0xbb,0x46), 20) ; 
    QPen yPen (QColor(0xfd,0xb8,0x27), 20) ;
    QPen oPen (QColor(0xf5,0x82,0x1f), 20) ;
    QPen rPen (QColor(0xe0,0x3a,0x3e), 20) ;
    QPen vPen (QColor(0x96,0x3d,0x97), 20) ;
    QPen bPen (QColor(0x00,0x9d,0xdc), 20) ;
    
    QPen blackPen  (QColor(0,0,0)) ;
    QPen yellowPen (QColor(255,255,0)) ;
    QPen goldPen   (QColor(0xff,0xd0,0x27)) ;

    int y0 = midY - 158 ;
    int w = screenSize.width() ;

    painter.setPen (gPen) ;
    painter.drawLine (0,y0, w,y0) ;
    painter.setPen (yPen) ;
    painter.drawLine (0,y0+20, w,y0+20) ;
    painter.setPen (oPen) ;
    painter.drawLine (0,y0+40, w,y0+40) ;
    painter.setPen (rPen) ;
    painter.drawLine (0,y0+60, w,y0+60) ;
    painter.setPen (vPen) ;
    painter.drawLine (0,y0+80, w,y0+80) ;
    painter.setPen (bPen) ;
    painter.drawLine (0,y0+100, w,y0+100) ;

    QFont font1 ("Times",  72, QFont::Bold) ;
    QFont font2 ("Times",  16, QFont::Bold) ;
    QFont font3 ("Times",  32, QFont::Bold) ;
    QFont font4 ("Courier", 9, QFont::Bold) ;
    QFontMetrics fm1 (font1);
    QFontMetrics fm2 (font2);
    QFontMetrics fm3 (font3);
    QFontMetrics fm4 (font4);

    QRect rect = fm1.boundingRect (splashText1) ;
    w = rect.width() ;
    painter.setFont (font1) ;
    painter.setPen (blackPen) ;
    painter.drawText ((midX-w/2)+4, (midY-83), splashText1) ;  // black shadow letters
    painter.setPen (yellowPen) ;
    painter.drawText ((midX-w/2), (midY-85), splashText1) ;    // bright yellow letters

    painter.setPen (goldPen) ;

    painter.setFont (font2) ;
    rect = fm2.boundingRect (splashText2) ;
    w = rect.width() ;
    painter.drawText ((midX-w/2), (midY-22), splashText2) ;

    painter.setFont (font3) ;
    rect = fm3.boundingRect (splashText3) ;
    w = rect.width() ;
    painter.drawText ((midX-w/2), midY+20, splashText3) ;

    rect = fm3.boundingRect (splashText4) ;
    w = rect.width() ;
    painter.drawText ((midX-w/2), midY+80, splashText4) ;

    painter.setFont (font2) ;
    rect = fm2.boundingRect (splashText5) ;
    w = rect.width() ;
    painter.drawText ((midX-w/2), midY+150, splashText5) ;

    painter.setFont (font4) ;
    rect = fm4.boundingRect (splashText6) ;
    w = rect.width() ;
    int x = screenSize.width() - w - 15 ;
    painter.drawText (x, screenSize.height()-10, splashText6) ;

}


//   Draw a single 40-column line on the screen

void Screen::drawLine_40 (QPixmap& charsPixmap, uchar *characters, int dstX, int dstY)
{
    QPainter painter (&m_screenBuffer) ;

    for (int i=0; i<40; i++) {
        int srcPixmapOffset = 16 * characters[i] ;              // draw a single character
        painter.drawPixmap (dstX+(14*i)+2, dstY+2, charsPixmap, 0, srcPixmapOffset, 14, 16) ;
    }
}


//   Refresh 40-column text-only screen (or lower lines of a mixed lores-&-text screen)

void Screen::draw40Column (int firstLine, quint8 *loresData)
{
    int  line, h, offSet, y ;
    h = 16 ;

    for (line=firstLine; line<24; line++) {
        y = h*line ;
        offSet = screen_RdTEXT_offsets[line] ;
        drawLine_40 (m_pixmap_40ColumnPrimary16, loresData+offSet, 0, y) ;  // draw a line of 40 characters
    }

    QPainter painter ;
    painter.drawPixmap (QPoint(0,0), m_screenBuffer) ;

}



//   Refresh a low-resolution screen (or upper area of a mixed lores-&-RdTEXT screen)

void Screen::drawLoRes (int nLines, quint8 *screen)
{
    int      h, line, col, off, dstX, dstY ;
    quint8   byte, nibble ;
    QPixmap  *map ;
    QPainter painter(&m_screenBuffer) ;

    h = 16 ;
    dstX = 0 ;

    for (line=0; line<nLines; line++) {
        dstY = h*line ;
        off = screen_RdTEXT_offsets[line] ;
        for (col=0; col<40; col++) {
            byte = screen[off+col] ;
            nibble = byte & 0x0f ;
            map = m_loResBlocks[nibble] ;
            painter.drawPixmap (dstX+col*14, dstY, *map, 0,0, 14,16) ;
            nibble = byte>>4 ;
            map = m_loResBlocks[nibble] ;
            painter.drawPixmap (dstX+col*14, dstY+8, *map, 0,0, 14,16) ;
        }
    }

}



//   Draw a single 80-column line on the screen

void Screen::drawLine_80 (QPixmap& charsPixmap, int x, int y, quint8 *main, quint8 *aux)
{
    int i, charOffM, charOffA, dest_xM, dest_xA ;
    QPainter painter(&m_screenBuffer) ;

    for (i=0; i<40; i++) {
        charOffM = 16 * main[i] ;
        charOffA = 16 * aux[i] ;
        dest_xA = x + i*14 ;
        dest_xM = dest_xA + 7 ;
        painter.drawPixmap (dest_xA, y, charsPixmap, 0, charOffA, 7, 16) ;
        painter.drawPixmap (dest_xM, y, charsPixmap, 0, charOffM, 7, 16) ;
    }
//    QWidget::update (x,y,  m_pixelsWidth, 16) ;
}



//   Refresh 80-column text-only screen

void Screen::draw80Column (void)
{
    int    h, line, off, x, y ;
    quint8  *mainScreen, *auxScreen ;

    x = 0 ;
    h = 16 ;

    mainScreen = MAC->m_ram + 0x400 ;
    auxScreen  = MAC->m_aux + 0x400 ;

    for (line=0; line<24; line++) {
        y = h*line ;
        off = screen_RdTEXT_offsets[line] ;
        drawLine_80 (m_pixmap_80ColumnPrimary16, x, y, mainScreen+off, auxScreen+off) ;
    }
}



inline void Screen::put_7_bits (QPainter& painter, quint8* screenData, int x, int y, int col, int off)
{
    uint      byte ;
    quint8    prev, next ;
    QPixmap*  cPixMap ;

    prev = 0 ;
    if (col) prev = screenData[off+col-1] & 0x40 ;
    byte = screenData[off+col] ;
    if (byte & 0x80) {
        cPixMap = &m_pixmap_xpm_hires_1 ;
    } else {
        cPixMap = &m_pixmap_xpm_hires_0 ;
    }
    byte &= 0x7f ;
    next = screenData[off+col+1] & 1 ;

    if (col == 39) next = 0 ;
    if (prev)    byte += 128 ;
    if (next)    byte += 256 ;
    if (col & 1) byte += 512 ;

    painter.drawPixmap (x, y, *cPixMap, 0, byte<<1, 14, 2) ;

}



void Screen::drawHiRes (quint8 *screenData)
{
    int x, y, line, col, off, offset1 ;
    QPainter painter (&m_screenBuffer) ;

    for (line=0; line<64; line++) {
        offset1 = ((line<<10) & 0x1c00) | ((line<<4) & 0x0380) ;
        x = 0 ;
        for (col=0; col<40; col++) {

            y = 2*line ;
            off = offset1 ;
            put_7_bits (painter, screenData, x, y, col, off) ;

            y += 0x80 ;
            off += 40 ;
            put_7_bits (painter, screenData, x, y,  col, off) ;

            if (MAC->RdMIXED && (line > 31)) {
                x += 14 ;
                continue ;
            }

            y += 0x80 ;
            off += 40 ;
            put_7_bits (painter, screenData, x, y,  col, off) ;
            
            x += 14 ;
        }
    }
}


// -------------------------  Double Hi-Res Graphics  -------------------------


//     Double hi-res bit packing: 7 "dots" are packed in 4 bytes of screen data
//     (Bit 7 of the screen data bytes is unused; a dot is 2x2 pixels)
//
// |   b0   |   b1   |   b2   |   b3   | ...  bytes
// |-BBBAAAA|-DDCCCCB|-FEEEEDD|-GGGGFFF| ...  color #s

//    Use 4 bytes of screen data to write 7 dots to the screen buffer DC

inline void Screen::doubleHiRes4bytes (quint8* main, quint8* aux, int rowOffset, int row, int column)
{
    main += rowOffset + column*2 ;
    aux  += rowOffset + column*2 ;

    int b0 = *aux++ ;           // b0...b3 are 4 bytes containing 7 horizontal "dots"
    int b1 = *main++ ;
    int b2 = *aux ;
    int b3 = *main ;

    int iA = b0 & 0x0f ;        // iA...iG are indices into the color array 'm_dblHiResBlock'
    int iB = ((b0 & 0x70)>>4) | ((b1 & 1)<<3) ;
    int iC = (b1 & 0x1e)>>1 ;
    int iD = ((b1 & 0x60)>>5) | ((b2 & 0x03)<<2);
    int iE = (b2 & 0x3c)>>2 ;
    int iF = ((b2 & 0x40)>>6) | ((b3 & 0x07)<<1) ;
    int iG = (b3 & 0x78)>>3 ;

    int x = 7*4*column + 2 ;
    int y = 2*row ;
    QPainter painter (&m_screenBuffer) ;

    painter.drawPixmap(x,    y+2, *m_dblHiResBlock[iA]) ;  
    painter.drawPixmap(x+4,  y+2, *m_dblHiResBlock[iB]) ;
    painter.drawPixmap(x+8,  y+2, *m_dblHiResBlock[iC]) ;
    painter.drawPixmap(x+12, y+2, *m_dblHiResBlock[iD]) ;
    painter.drawPixmap(x+16, y+2, *m_dblHiResBlock[iE]) ;
    painter.drawPixmap(x+20, y+2, *m_dblHiResBlock[iF]) ;
    painter.drawPixmap(x+24, y+2, *m_dblHiResBlock[iG]) ;

}


//    Write an entire double-hires screen

void Screen::writeDoubleHiRes (quint8 page2)
{
    int screenBase = 0x2000 ;
    if (page2) screenBase = 0x4000 ;
    quint8* main = MAC->m_ram + screenBase ;
    quint8* aux  = MAC->m_aux + screenBase ;

    for (int row=0; row<194; row++) {
        int rowOffset = ((row<<10) & 0x1c00) | ((row<<4) & 0x0380) | ((row/64)*0x28) ;
        for (int column=0; column<40; column++) {
            doubleHiRes4bytes (main, aux, rowOffset, row, column) ;
        }
    }
}


//--------------------------------------------------


void Screen::paintEvent (QPaintEvent* e)
{
    QPainter painter(this) ;

    if (MAC->powerIsOn()) {
        painter.drawPixmap (e->rect(), m_screenBuffer, m_screenBuffer.rect()) ;
    } else {
        splashScreen() ;
    }
}



/***************************************************

        enum {
            MODE_40ColumnPage1,    a
            MODE_40ColumnPage2,    b
            MODE_loresPage1,       c
            MODE_loresPage2,       d
            MODE_loresMixedPage1,  e
            MODE_loresMixedPage2,  f
            MODE_hiresPage1,       g
            MODE_hiresPage2,       h
            MODE_hiresMixedPage1,  j
            MODE_hiresMixedPage2,  k
            MODE_80Column          x
        } ;

           T = RdTEXT     H = HIRES    E = Rd80COL
           M = MIXED    P = PAGE2

                  Graphics   |     RdTEXT
                   T = 0     |     T = 1
                             |
              \ HM           |            HM /
            PE \ 00 01 11 10 |  00 01 11 10 /  PE
             00  c  e  j  g  |  a  a  a  a  00
             01  c  e  j  g  |  x  x  x  x  01
             11  d  f  k  h  |  x  x  x  x  11
             10  d  f  k  h  |  b  b  b  b  10

***************************************************/


void Screen::refreshScreen (void)
{
//putchar('.'); fflush(stdout) ;
 //   int timeoutsPerSecond = 1000/m_timerPeriod ;  // NB: we depend on 'timeoutsPerSecond' being an multiple of 5
 //   if (--m_flashCounter == 0) m_flashCounter = timeoutsPerSecond ;
 //   if ((m_flashCounter%5) == 0) m_flash = !m_flash ;

    quint8  *loresData, *hiresData ;

    quint8 rdText     = MAC->m_ss[0x01a] ;
    quint8 rdMixed    = MAC->m_ss[0x01b] ;
    quint8 page2      = MAC->m_ss[0x01c] ;
    quint8 rdHiRes    = MAC->m_ss[0x01d] ;
    quint8 rd80Col    = MAC->m_ss[0x01f] ;
    quint8 rdDblHiRes = MAC->m_ss[0x07f] ;

    if (page2) {
        loresData = MAC->m_ram+0x800 ;
        hiresData = MAC->m_ram+0x4000 ;
    } else {
        loresData = MAC->m_ram+0x400 ;
        hiresData = MAC->m_ram+0x2000 ;
    }

    if  (rdDblHiRes                   // Double hi-res
         && rdHiRes
         && rdText == OFF
         && rd80Col)
    {
        if (m_refreshTimer->interval() == FAST) {    // Use a slow refresh speed when displaying double hi-res to
             m_refreshTimer->setInterval (SLOW) ;    // let Raspberry Pi machines run at less than 100% CPU usage
        }
        writeDoubleHiRes (page2) ;
        QWidget::update (this->rect()) ;
        return ;
    } else {
        if (m_refreshTimer->interval() == SLOW) {
            m_refreshTimer->setInterval (FAST) ;
        }
    }

    if (rdText) {                     // RdTEXT
        if (rd80Col) {
            draw80Column() ;
        } else {
            draw40Column (0, loresData) ;
        }
    } else {                          // GRAPHICS
        if (rdHiRes) {         // hi-res
            if (rdMixed) {
                drawHiRes (hiresData) ;
                draw40Column (20, loresData) ;
            } else {
                drawHiRes (hiresData) ;
            }
        } else {               // lo-res
            if (rdMixed) {
                drawLoRes (20, loresData) ;
                draw40Column (20, loresData) ;
            } else {
                drawLoRes (24, loresData) ;
            }
        }
    }
    QWidget::update (this->rect()) ;
}


