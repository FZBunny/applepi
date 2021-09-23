

#include <QDesktopServices>
#include <QFontComboBox>
#include <QUrl>

#include "about.h"
#include "version.h"



About::About (QWindow*)
{

//    QFontComboBox* fb = new QFontComboBox (this) ;

    this->resize (WIDTH, HEIGHT) ;
    this->setMinimumSize (WIDTH, HEIGHT) ;
    this->setMaximumSize (WIDTH, HEIGHT) ;

    m_panel1 = new QWidget (this) ;

    m_close = new QPushButton ("Close", this) ;
    m_close->move (WIDTH-100, HEIGHT-40) ;

    QFont font1 ("Times", 40, QFont::Bold)  ;  // 'Times' probably faked by Qt, but looks good enough...
    m_txt1 = new QLabel ("ApplePi", this) ;
    m_txt1->setFont (font1) ;
    int x = centerText (m_txt1) ;
    m_txt1->move (x, 20) ;

    QFont font2 ("Times", 12, QFont::Bold)  ;
    m_txt2 = new QLabel (QString(APPLEPI_VERSION), this) ;
    m_txt2->setFont (font2) ;
    x = centerText (m_txt2) ;
    m_txt2->move (x, 90) ;

    QString str1 ("An Apple II emulator for Raspberry Pi OS,\n"
   //                "other Linux distributions, and Windows 10") ;
                   "     and most other Linux distributions.") ;
    m_txt3 = new QLabel (str1, this) ;
    m_txt3->setFont (font2) ;
    m_txt3->move (65, 120) ;

    m_txt4 = new QLabel ("Copyright © 2019-2021 J.B.Ward", this) ;
    m_txt4->setFont (font2) ;
    x = centerText (m_txt4) ;
    m_txt4->move (x, 520) ;


    QString copyright =
    "<HTML>"
    "<HEAD>"
    "  <META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; charset=iso-8859-1\">"
    "  <META NAME=\"GENERATOR\" CONTENT=\"Mozilla/4.06 [en] (X11; I; Linux 2.0.35 i686) [Netscape]\">"
    "</HEAD>"
    "<BODY TEXT=\"#006000\" BGCOLOR=\"#E8E8F0\" LINK=\"#0000FF\" VLINK=\"#FF0000\" ALINK=\"#000088\">"
    "<BR>"
#if defined (__arm__) || defined (__aarch64__)
    "<span style=\"font-size:10pt; color:black;\">"
#else
    "<span style=\"font-size:11pt; color:black;\">"
#endif
    "ApplePi is free software; you can redistribute it and/or modify "
    "it under the terms of the GNU General Public License as published by "
    "the Free Software Foundation; either version 3 of the License, or "
    "(at your option) any later version.<BR>"
    "ApplePi is distributed in the hope that it will be useful, "
    "but WITHOUT ANY WARRANTY; without even the implied warranty of "
    "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. "
    "See the GNU General Public License for more details."
    "<BR><BR>"
    "You may obtain a copy of the GNU General Public License by writing to:<BR>"
    "The Free Software Foundation, Inc.<BR>"
    "51 Franklin Street, Fifth Floor<BR>"
    "Boston, MA 02110-1335 USA<BR><BR>"
    "Or simply visit:"
    "</span>" ;

    m_html = new QTextBrowser (this) ;
    m_html->setHtml (copyright) ;
    m_html->resize (390, 330) ;
    m_html->move (15, 170) ;

    m_visit = new QPushButton (m_url, this) ;
    m_visit->move (120, 460) ;

    connect (m_visit, &QPushButton::clicked, this, &About::onUrlButtonClicked) ;
    connect (m_close, &QPushButton::clicked, this, &About::onCloseButtonClicked) ;
}


void About::onUrlButtonClicked (void)
{
    QDesktopServices::openUrl (QUrl("https://www.gnu.org/licenses/gpl-3.0.html")) ; 
}


void About::onCloseButtonClicked (void)
{
    this->accept() ;
}


int About::centerText (QLabel* label)
{
    int w, txtX ;
    QFont font = label->font() ;
    QFontMetrics fm (font) ;
    w = fm.width (label->text()) ;
    txtX = (this->width()/2) - w/2 ;
    
    return txtX ;
}

