/*****************************************************************************

    File: "rom_dialog.cpp"

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


#include <QFileDialog>
#include <QStatusBar>

#include "defs.h"
#include "rom_dialog.h"
#include "mainwindow.h"




RomDialog::RomDialog (MainWindow* parent) : QDialog (parent)
{
    m_parent = parent ;
    int windowWidth  = 300 ;
    int windowHeight = 280 ;

    m_parent = parent ;
    resize (windowWidth,windowHeight) ;
    QPoint pPos = parent->pos() ;
    QSize pSize = parent->size() ;
    int x = ((pPos.x()+pSize.width()/2) - windowWidth/2) ;
    int y = ((pPos.y()+pSize.height()/2) - windowHeight/2) ;
	move (x,y) ;

    QSize windowSize (windowWidth, windowHeight) ;
    QSize buttonSize (95, 30) ;
    QSize panelSize (230, 65) ;
    m_buttonX = 50 ;
    m_buttonY = 60 ;
    QString text (
        "  Choose the model of Apple II, or\n"
        " select an external ROM image file"
    ) ;

    resize (windowSize) ;
    setMinimumSize (windowSize) ;
    setMaximumSize (windowSize) ;

    m_text = new QLabel (text, (QWidget*)this) ;
    m_text->move(40,15) ;
    m_text->show() ;

    m_frame = new QFrame (this, Qt::Widget) ;
    m_text  = new QLabel (m_frame) ;

    m_apple2          = new QRadioButton ("Apple][    (Original)", this) ;
    m_apple2Plus      = new QRadioButton ("Apple][+",this) ;
    m_apple2e         = new QRadioButton ("Apple//e", this) ;
    m_apple2eEnhanced = new QRadioButton ("Apple//e enhanced", this) ;
    m_externalFile    = new QRadioButton ("Select an external ROM file", this) ;

    m_apple2->move (m_buttonX, m_buttonY) ;
    m_apple2->resize (200,20) ;
    m_apple2Plus->move (m_buttonX, m_buttonY+20) ;
    m_apple2Plus->resize (200,20) ;
    m_apple2e->move (m_buttonX, m_buttonY+40) ;
    m_apple2e->resize (200,20) ;
    m_apple2eEnhanced->move (m_buttonX, m_buttonY+60) ;
    m_apple2eEnhanced->resize (200,20) ;
    m_externalFile->move (m_buttonX, m_buttonY+80) ;
    m_externalFile->resize (200,20) ;

    m_romFileName = new QLabel ("", this) ;
    m_romFileName->resize (windowWidth, 24) ;

    m_fileButton   = new QPushButton ("Open File", this) ;
    m_cancelButton = new QPushButton ("Cancel", this) ;
    m_doneButton   = new QPushButton ("Done", this) ;

    m_fileButton->resize (buttonSize) ;
    m_cancelButton->resize (buttonSize) ;
    m_doneButton->resize (buttonSize) ;

    m_fileButton->move(m_buttonX+50,m_buttonY+110) ;
    int bX = 10 ;
    int bY = windowHeight - buttonSize.height() - 10 ;
    m_cancelButton->move (bX, bY) ;
    bX = windowWidth - buttonSize.width() - 12 ;
    m_doneButton->move (bX, bY) ;

    CFG->Get ("use_internal_rom", &m_useInternalRom) ;
    CFG->Get ("internal_rom_number", &m_internalRomNumber) ;
    CFG->Get ("rom_path", &m_romPath) ;
    QFileInfo fInfo (m_romPath) ;
    QString name (fInfo.fileName()) ;
    m_romFileName->setText (name) ;
    m_romFileName->resize (windowWidth, 24) ;
    m_romFileName->move (0, m_buttonY+140) ;
    m_romFileName->setAlignment (Qt::AlignCenter) ;

    if (m_useInternalRom == "yes") {
        m_fileButton->setEnabled(false) ;
        m_romFileName->setEnabled(false) ;
        switch (m_internalRomNumber) {
          case APPLE2:
            m_apple2->setChecked(true) ;
            m_buttonOnEntry = m_apple2 ;
            break ;
          case APPLE2PLUS:
            m_apple2Plus->setChecked(true) ;
            m_buttonOnEntry = m_apple2Plus ;
            break ;
          case APPLE2E:
            m_apple2e->setChecked(true) ;
            m_buttonOnEntry = m_apple2e ;
            break ;
          case APPLE2E_ENHANCED:
            m_apple2eEnhanced->setChecked(true) ;
            m_buttonOnEntry = m_apple2eEnhanced ;
            break ;
          default:
            m_apple2e->setChecked(true) ;
             m_buttonOnEntry = m_apple2e ;
            break ;
        }
    } else {
        m_fileButton->setEnabled(true) ;
        m_externalFile->setChecked(true) ;
        m_buttonOnEntry = m_externalFile ;
    }

    connect (m_apple2,          &QRadioButton::toggled, this, &RomDialog::onApple2Original) ;
    connect (m_apple2Plus,      &QRadioButton::toggled, this, &RomDialog::onApple2Plus) ;
    connect (m_apple2e,         &QRadioButton::toggled, this, &RomDialog::onApple2e) ;
    connect (m_apple2eEnhanced, &QRadioButton::toggled, this, &RomDialog::onApple2eEnhanced) ;
    connect (m_externalFile,    &QRadioButton::toggled, this, &RomDialog::onUseExternalROM) ;
    connect (m_fileButton,      &QPushButton::clicked,  this, &RomDialog::onSelectRomFile) ;
    connect (m_cancelButton,    &QPushButton::clicked,  this, &RomDialog::onCancel) ;
    connect (m_doneButton,      &QPushButton::clicked,  this, &RomDialog::onDone) ;

}


void RomDialog::onApple2Original (void)
{
    m_useInternalRom = "yes" ;
    m_internalRomNumber = APPLE2 ;
    m_fileButton->setEnabled(false) ;
    m_romFileName->setEnabled(false) ;
}


void RomDialog::onApple2Plus (void)
{
    m_useInternalRom = "yes" ;
    m_internalRomNumber = APPLE2PLUS ;
    m_fileButton->setEnabled(false) ;
    m_romFileName->setEnabled(false) ;
}


void RomDialog::onApple2e (void)
{
    m_useInternalRom = "yes" ;
    m_internalRomNumber = APPLE2E ;
    m_fileButton->setEnabled(false) ;
    m_romFileName->setEnabled(false) ;
}


void RomDialog::onApple2eEnhanced (void)
{
    m_useInternalRom = "yes" ;
    m_internalRomNumber = APPLE2E_ENHANCED;
    m_fileButton->setEnabled(false) ;
    m_romFileName->setEnabled(false) ;
}


void RomDialog::onUseExternalROM (void)
{
    m_useInternalRom = "no" ;
    m_fileButton->setEnabled(true) ;
    m_romFileName->setEnabled(true) ;
}


void RomDialog::onSelectRomFile (void)
{
    QString filter ("ROM files (*.ROM *.rom *.BIN *.bin);;Any *") ;
    QString test = QFileDialog::getOpenFileName (this, "Open a 5.25\" floppy disk image", m_romPath, filter) ;
    if (test.length()) {
//printf ("test.length()=%i\n", test.length()) ;
        m_romPath = test ;
        QFileInfo fInfo (m_romPath) ;
        QString name (fInfo.fileName()) ;
        m_romFileName->setText (name) ;
    } else {
        m_buttonOnEntry->setChecked (true) ;
    }
}


void RomDialog::onCancel (void)
{
    this->accept() ;
}


void RomDialog::onDone (void)
{
    QString no ("no") ;
    QString yes ("yes") ;
    if (m_externalFile->isChecked()) {
        CFG->Set ("use_internal_rom", no) ;
        CFG->Set ("rom_path", m_romPath) ;
    } else {
        CFG->Set ("use_internal_rom", yes) ;
        CFG->Set ("internal_rom_number", m_internalRomNumber) ;
    }
    m_parent->installRom() ;

    this->accept() ;
}
