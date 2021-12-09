/*****************************************************************************

    File: "printer.cpp"

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

#include <QtGlobal>
#include <stdio.h>
#include <limits.h>

#include "defs.h"
#include "machine.h"
#include "printer.h"
#include "xpm_images.h"
#include "debugging_dumps.h"




Printer::Printer (Machine* mac)
{
    m_parent = mac ;
    m_out = new QFile() ;
    m_out->open (1, QIODevice::WriteOnly) ;
    m_writingToFile = false ;
    m_pixmapPrinterPins = QPixmap (xpm_printerPins) ;
}


bool Printer::open (QString path)
{
    if (m_out) delete m_out ;
    m_out = new QFile (path) ;
    bool ok = m_out->open (QIODevice::WriteOnly) ;
    if (ok) m_writingToFile = true ;
    //                                 XXXXX add error dialog XXXXX
    m_graphicsState = 0 ;
    m_pixelsWidth = 0 ;
    m_numberLines = 0 ;
    m_pixelCounter = INT_MAX ;

    return ok ;
}


QString Printer::error()
{
    return m_out->errorString() ;
}


void Printer::close (void)
{
    if (m_graphicsState) makePDF() ;

    m_out->close() ;
    delete m_out ;
    m_out = new QFile() ;
    m_out->open (1, QIODevice::WriteOnly) ;
    m_writingToFile = false ;
    MAC->m_ram[0x36] = 0xf0 ;  // restore the char. output vector to COUT1 ($FDF0)
    MAC->m_ram[0x37] = 0xfd ;
}


bool Printer::haveOpenFile (void)
{
    return m_writingToFile ;
}


//
//  This is called by Machine::fetch when fetching a byte from our fictitious printer ROM.
//  This usually occurs when fetching 3 butes for a JSR $C100 (When a "PR#1" is entered),
//  or 3 bytes when a JSR $C110 is executed to print a single character.
//  ProDOS may also fetch single bytes when attempting to identify the device in slot 1. 

quint8 Printer::fetch_Printer_ROM (int slotNumber, quint8 p)
{                                      
    static const quint8 RTS = 0x60 ;
    
    quint16 calledFrom = MAC->savedPC() ;
    quint16 slotAddr = 0xc000 | (slotNumber << 8) ;
    quint16 entryPoint = slotAddr + 0x10 ;
    ProcessorState* ps = MAC->processorState() ;
    quint8 c ;

    // Ignore 2 fetches after $Cn00 or $Cn10 entry; just a side-effect
    // of last few lines of fn. "Machine::run" saving history data.
    if ((calledFrom==slotAddr)   && ((p==0x01) || (p==0x02))) return 0 ;
    if ((calledFrom==entryPoint) && ((p==0x11) || (p==0x12))) return 0 ;

    if ((p==0) && (calledFrom==slotAddr)) {                        // Did someone say "PR#1" ?
        MAC->m_ram[0x36] = 0x10 ;            // Set the character output vector
        MAC->m_ram[0x37] = slotAddr >> 8 ;   // to point to us
        ps->Areg = 0 ;
        ps->Pstat &= C ^ 0xff ;
        c = RTS ;
    } else if ((p==0x10) && (calledFrom==entryPoint)) {            // Print a single character.
        m_out->putChar (ps->Areg&0x7f) ;
        if ((ps->Areg&0x7f) == '\r') m_out->putChar ('\n') ;
        m_out->flush() ;
        if (MAC->m_ss[0] == 0x83) {          // If a ctrl-C was entered at the keyboard,
            close() ;                        // close the output file & reset the output vector.
        }
        c = RTS ;
    } else {                                                       // PC isn't Cn00 or Cn10; just return a byte from ROM.
        if (p > 0x4f) p = 0 ;
        c = epson_ROM_fragment[p] ;
    }
//printf ("fetch_Printer_ROM %2.2x from %4.4x\n", c, p) ;
    return c ;
}


quint8 Printer::fetch (int loNibble)
{
    quint8 c ;

    if (loNibble > 0x4f) c = 0 ;
    else                 c = epson_ROM_fragment[loNibble] ;

    return c ;
}


void Printer::store (int /*loNibble*/, quint8 c)
{
//printf ("Printer::store %2.2x to %4.4x\n", c, loNibble) ;
    m_out->putChar (c) ;
    m_out->flush() ;

    if (m_writingToFile) handleGraphics (c) ;
}


void Printer::handleGraphics (quint8 c)
{    
    switch (m_graphicsState) {
        case 0:
            if (c == 0x1b) m_graphicsState = 1 ;
            break ;
        case 1:
            if (c == 0x4c) m_graphicsState = 2 ;
            break ;
        case 2:
            m_pixelsWidthLo = c ;
            m_graphicsState = 3 ;
            break ;
        case 3:
            if (m_pixelsWidth == 0) {     // Take the width of the 1st line, and use it for all.
                m_pixelsWidth = m_pixelsWidthLo + 256*c ;
            }
            m_pixelCounter = 0 ;
            m_numberLines++ ;
            m_graphicsState = 4 ;
            break ;
        case 4:
            if (m_pixelCounter++ < m_pixelsWidth) {
                m_printerPixelData.append (c) ;
//printf ("%2.2x ", c) ;
            } else {
                m_graphicsState = 0 ;
                if (c == 0x1b) m_graphicsState = 1 ;
//printf ("\n-----\n") ;
            }
            break ;
        default:
            m_graphicsState = 0 ;
            break ;
    }

}


void Printer::makePDF (void)
{
printf ("Printer::makePDF m_graphicsState=%i\n", m_graphicsState) ;
    if ( ! m_graphicsState) return ;

    QString fileName = m_out->fileName() ;                  // Make the PDF filename
    int len = fileName.length() ;
    int ix = fileName.indexOf (".txt", 0, Qt::CaseInsensitive) ;  // XXXXXXXXXXXXXXXX  FIXME : don't assume ".txt"
    if (ix ==  len-4) {
        fileName.truncate (ix) ;
    }
    fileName.append (".pdf") ;
qStdOut() << "PDF filename = " << fileName << endl ;

    QFile pdfFile (fileName) ;
    bool ok = pdfFile.open (QIODevice::WriteOnly) ;         // Open the PDF file
    if ( !ok) {
printf ("Open PDF failed.\n") ;                                   //  XXXXXXXXXXXXXXXXX Change this to an error dialog XXXXX
        return ;
    }

    QPdfWriter writer (fileName) ;                          // Set up the PDF writer
    writer.setResolution (120) ;  // A magic number which seems to work...
    writer.setPageSize (QPagedPaintDevice::Letter) ;
    writer.setPageMargins (QMargins(0.8, 0.8, 0.8, 0.6), QPageLayout::Inch) ;

    QPainter painter (&writer) ;
    QPen pen (Qt::black) ;
    pen.setWidth(1) ;
    painter.setPen (pen) ;

    QRect r = painter.viewport() ;

/*****************************************
//----------------------------------------
printf ("Painter viewport is %ix%i\n", r.width(), r.height()) ;
printf ("m_numberLines=%i\n", m_numberLines) ;

painter.drawLine (0,0,  r.width(),r.height()) ;
painter.drawLine (r.width(),0,  0,r.height()) ;

QPixmap cpixmap = QPixmap (xpm_40ColChars) ;
for (int i=0; i<64; i++) {
    painter.drawPixmap (14*i, 660, cpixmap, 0, i*16, 14, 16) ;
}
//----------------------------------------
*****************************************/

    for (int line=0; line<m_numberLines; line++) {
        for (int column=0; column<m_pixelsWidth; column++) {
            quint8 byte = m_printerPixelData.at (line*m_pixelsWidth+column)<< 1 ;
            painter.drawPixmap (column,30+(line*7), m_pixmapPrinterPins, 0,byte*8, 1,8) ;
        }
    }

    painter.end() ;
    pdfFile.close() ;

    m_numberLines = 0 ;
}
