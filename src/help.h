/*****************************************************************************


    File: "help.h"

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



#ifndef Help_H
#define Help_H

#include <QLabel>
#include <QWidget>
#include <QMainWindow>
#include <QDialog>
#include <QPushButton>
#include <QTextBrowser>


class Help : public QDialog
{
public:

    Help (QMainWindow* parent) ;
    ~Help() {} ;

private:

    void onCloseButtonClicked (void) ;
    void onBackButtonClicked (void) ;
    void onFwdButtonClicked (void) ;
    void onHomeButtonClicked (void) ;
    void resizeEvent (QResizeEvent* e) ;
    void moveEvent (QMoveEvent* e) ;
    int  centerText (QLabel* label) ;

    QString m_index ;
    QLabel* m_txt1 ;
    QLabel* m_txt2 ; 
       
    QWidget*       m_panel1 ;
    QWindow*       m_panel2 ;
    QTextBrowser*  m_html ;
    QPushButton*   m_close ;
    QPushButton*   m_back ;
    QPushButton*   m_fwd ;
    QPushButton*   m_home ;
} ;

#endif
