/*****************************************************************************

    File: "help.cpp"

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



#include <QDesktopServices>
#include <QFontComboBox>
#include <QUrl>
#include <QApplication>
#include <QDesktopWidget>
#include <QScreen>
#include <QResizeEvent>


#include "defs.h"
#include "help.h"
#include "version.h"



Help::Help (QMainWindow*)
{
    const int MIN_WIDTH = 420 ;
    const int MIN_HEIGHT = 500 ;

    this->setMinimumSize (MIN_WIDTH, MIN_HEIGHT) ;

    m_panel1 = new QWidget (this) ;

    m_close = new QPushButton ("Close", this) ;
    m_back  = new QPushButton ("Back", this) ;
    m_fwd   = new QPushButton ("Fwd", this) ;
    m_home  = new QPushButton ("Home", this) ;


    QFont font1 ("Times", 30, QFont::Bold)  ;  // 'Times' is probably faked by Qt, but looks good enough...
    m_txt1 = new QLabel ("ApplePi Help", this) ;
    m_txt1->setFont (font1) ;

    QFont font2 ("Times", 12, QFont::Bold)  ;
    m_txt2 = new QLabel ("v" + QString(APPLEPI_VERSION), this) ;
    m_txt2->setFont (font2) ;

    m_html = new QTextBrowser (this) ;
    m_html->move (3, 90) ;

    connect (m_close, &QPushButton::clicked, this, &Help::onCloseButtonClicked) ;
    connect (m_back,  &QPushButton::clicked, this, &Help::onBackButtonClicked) ;
    connect (m_fwd,   &QPushButton::clicked, this, &Help::onFwdButtonClicked) ;
    connect (m_home,  &QPushButton::clicked, this, &Help::onHomeButtonClicked) ;

  //  m_index = QString ("file:///usr/share/applepi/ApplePiHelp.html") ;
    m_index = QString ("file:///home/bward/projects/applepi/help/ApplePiHelp.html") ;   // XXXXXXXXXXXXXX DEV & DEBUG XXXXXXXXXXXXXXXXX
    m_html->setSource (QUrl(m_index)) ;

    QSize size ;
    QPoint pos ;
    CFG->Get ("help_position", &pos) ;
    CFG->Get ("help_size", &size) ;
    this->move (pos.x(), pos.y()) ;
    this->resize (size.width(), size.height()) ;
}


void Help::onCloseButtonClicked (void)
{
    this->accept() ;
}


void Help::onBackButtonClicked (void)
{
    m_html->backward() ;
}


void Help::onFwdButtonClicked (void)
{
    m_html->forward() ;
}


void Help::onHomeButtonClicked (void)
{
    m_html->setSource (QUrl(m_index)) ;
}


void Help::moveEvent (QMoveEvent* e)
{
    QPoint pos = e->pos() ;
    CFG->Set (QString("help_position"), pos) ;
}


void Help::resizeEvent (QResizeEvent* e)
{
    int w = e->size().width() ;
    int h = e->size().height() ;

    int x = centerText (m_txt1) ;
    m_txt1->move (x, 0) ;
    m_txt2->move (10, h-30) ;
    m_close->move (w-100, h-40) ;
    int y = 53 ;
    m_back->move (x-40, y) ;
    m_home->move (x+65, y) ;
    m_fwd->move  (x+170, y) ;
    m_html->resize (w-7, h-145) ;

    CFG->Set (QString("help_size"), QSize(w,h)) ;
}


int Help::centerText (QLabel* label)
{
    int w, txtX ;
    QFont font = label->font() ;
    QFontMetrics fm (font) ;
    w = fm.horizontalAdvance (label->text()) ;
    txtX = (this->width()/2) - w/2 ;
    
    return txtX ;
}

