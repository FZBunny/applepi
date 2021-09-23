/*****************************************************************************


    File: "about.h"

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



#ifndef ABOUT_H
#define ABOUT_H

#include <QLabel>
#include <QWidget>
#include <QWindow>
#include <QDialog>
#include <QPushButton>
#include <QTextBrowser>


class About : public QDialog
{
public:

    About (QWindow* parent) ;
    ~About() {} ;

private:

    void onCloseButtonClicked (void) ;
    void onUrlButtonClicked (void) ;
    int  centerText (QLabel* label) ;

    const int WIDTH = 420 ;
    const int HEIGHT = 600 ;
    
    QLabel* m_txt1 ;
    QLabel* m_txt2 ;
    QLabel* m_txt3 ;
    QLabel* m_txt4 ;

    const QString m_url = "www.gnu.org/licenses/gpl-3.0.html" ;

    QWidget*       m_panel1 ;
    QWindow*       m_panel2 ;
    QTextBrowser*  m_html ;
    QPushButton*   m_close ;
    QPushButton*   m_visit ;
} ;

#endif
