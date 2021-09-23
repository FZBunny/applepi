/*****************************************************************************

    File: "rom_dialog.h"

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


#ifndef ROM_DIALOG_H
#define ROM_DIALOG_H

#include <QDialog>
#include <QFrame>
#include <QRadioButton>
#include <QLabel>
#include <QString>

#include "mainwindow.h"


class RomDialog : public QDialog
{

public:

    RomDialog (MainWindow* parent) ;
    ~RomDialog(){} ;

private:

    void onApple2Original  (void) ;
    void onApple2e         (void) ;
    void onApple2Plus      (void) ;
    void onApple2eEnhanced (void) ;
    void onUseExternalROM  (void) ;
    void onSelectRomFile   (void) ;
    void onCancel          (void) ;
    void onDone            (void) ;

    void centerFileName (void) ;

    MainWindow*    m_parent ;
    QFrame*        m_frame ;
    QLabel*        m_text ;
    QLabel*        m_romFileName ;
    QRadioButton*  m_apple2 ;
    QRadioButton*  m_apple2Plus ;
    QRadioButton*  m_apple2e ;
    QRadioButton*  m_apple2eEnhanced ;
    QRadioButton*  m_externalFile ;

    QPushButton*   m_fileButton ;
    QPushButton*   m_cancelButton ;
    QPushButton*   m_doneButton ;

    QString  m_romPath ;
    QString  m_useInternalRom ;
    uint     m_internalRomNumber ;

    int      m_buttonX, m_buttonY ;
} ;

#endif
