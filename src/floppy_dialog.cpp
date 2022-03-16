/*****************************************************************************

    File: "floppy_dialog.cpp"

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
#include <QMessageBox>
#include <QDebug>

#include "floppy_dialog.h"
#include "mainwindow.h"
#include "machine.h"
#include "config.h"
#include "defs.h"


FloppyDialog* floppyDialogPtr ;

void floppyOnEjectInsertButtonClick(void) ;
void floppyOnNewButtonClick(void) ;
void floppyOnDoneButtonClick(void) ;



FloppyDialog::FloppyDialog (MainWindow* parent, int driveIndex) : QDialog ()
{
    floppyDialogPtr = this ;
    m_parent = parent ; 

    int width = 420 ;
    int height = 258 ;
    int x, y ;
    this->setMinimumSize (width,height) ;
    this->setMaximumSize (width,height) ;
    CENTER_DIALOG

	m_driveIndex = driveIndex ;
    if (driveIndex) m_floppyPathKey = "floppy2_path" ;
    else            m_floppyPathKey = "floppy1_path" ;

    CFG->Get(m_floppyPathKey, &m_floppyPath) ;

	QString title ;
    QTextStream(&title) << "  Disk Drive " << driveIndex+1 << "  " ;
	this->setWindowTitle(title) ;

    m_ejectInsertButton = new QPushButton ("Insert a Floppy", this) ;
    m_doneButton = new QPushButton ("Done", this) ;
    m_newFloppyButton = new QPushButton ("Make a New Floppy", this) ;

    int midX = width/2 ;
    m_filenameField = new QLineEdit(this) ;
    QSize size = m_filenameField->size() ;
    size.rwidth() = 380 ;
    m_filenameField->resize (size) ;
    x = midX - m_filenameField->size().width()/2 ;
    m_filenameField->move (x,40) ;

    m_noWriteWarning = new QLabel ("* File is Write Protected *", this) ;
    m_StaticText2 = new QLabel ("Current File or Directory", this) ;

    size = m_ejectInsertButton->size() ;
    size.rwidth() = 150 ;
    m_ejectInsertButton->resize(size) ;
    m_newFloppyButton->resize(size) ;

    x = midX - m_doneButton->size().width()/2 ;
    m_doneButton->move (x,192) ;
    x = midX - m_newFloppyButton->size().width()/2 ;
    m_ejectInsertButton->move (x,100) ;
    m_newFloppyButton->move (x,144) ;
    m_noWriteWarning->move (130,72) ;
    m_StaticText2->move (120,16) ;

	connect (m_ejectInsertButton, &QPushButton::clicked, ::floppyOnEjectInsertButtonClick) ;
	connect (m_doneButton,        &QPushButton::clicked, ::floppyOnDoneButtonClick) ;
	connect (m_newFloppyButton,   &QPushButton::clicked, ::floppyOnNewButtonClick) ;

    m_filenameField->insert (m_floppyPath) ;
    m_fileInfo = new QFileInfo(m_floppyPath) ;

    if (m_fileInfo->isDir() || m_floppyPath.isNull()) {
        m_ejectInsertButton->setText ("Insert a Floppy") ;
        m_newFloppyButton->setEnabled(true) ;
        m_noWriteWarning->hide() ;
    } else {
        m_ejectInsertButton->setText ("Eject") ;
        m_newFloppyButton->setEnabled(false) ;
        if (m_fileInfo->isWritable()) {
            m_noWriteWarning->hide() ;
        } else {
            m_noWriteWarning->show() ;
        }
    }

}


FloppyDialog::~FloppyDialog()
{
    delete m_fileInfo ;
}


void FloppyDialog::ejectFloppy (void)
{

//    if (isDirectory(m_floppyPath)) return ;
    if (m_fileInfo->isDir()) {
        return ;
    }

    MAC->m_floppy->close (m_driveIndex) ;

    m_floppyPath = m_fileInfo->dir().path() ;
    m_filenameField->selectAll() ;
    m_filenameField->del() ;
    m_filenameField->insert (m_floppyPath) ;
    m_fileInfo->setFile (m_floppyPath) ;
    CFG->Set (m_floppyPathKey, m_floppyPath) ;

    m_ejectInsertButton->setText ("Insert a Floppy") ;
    m_newFloppyButton->setEnabled(true) ;
    m_noWriteWarning->hide() ;

    m_parent->setFloppyLabel (m_driveIndex+1, "- empty drive -") ;
}


void FloppyDialog::insertFloppy (void)
{
    QString newPath ;
    QString filter ("Disk Files (*.dsk *.DSK *.po *.PO);;Any *") ;
    QString originalPath = m_filenameField->text() ;

    while (true) {

        newPath = QFileDialog::getOpenFileName (this, "Open a 5.25\" floppy disk image", m_floppyPath, filter) ;
        if ( ! newPath.isEmpty()) {
            m_filenameField->setText (newPath) ;
            m_fileInfo->setFile (newPath) ;
        }
        if (m_fileInfo->isDir()) {
             m_ejectInsertButton->setText ("Insert a Floppy") ;
             m_newFloppyButton->setEnabled (true) ;
        } else {
            int size = m_fileInfo->size() ;
            if ((size<143300) || (size>143400)) {
                QString msgText ;
                if (size>143400) {
                    QTextStream (&msgText) << "\"" << m_fileInfo->fileName() << "\" is too large to be a 5.25\" floppy." ;
                } else {
                    QTextStream (&msgText) << "\"" << m_fileInfo->fileName() << "\" is too small to be a 5.25\" floppy." ;
                }
                QMessageBox msg (QMessageBox::Warning, "Error", msgText, QMessageBox::Ok, this) ;
                msg.exec() ;
                m_filenameField->setText (originalPath) ;
                break ;
            }
            m_ejectInsertButton->setText ("Eject") ;
            m_newFloppyButton->setEnabled (false) ;
        }

        bool writeable = m_fileInfo->isWritable() ;
        if (writeable) m_noWriteWarning->hide() ;
        else           m_noWriteWarning->show() ;

        bool ok = MAC->m_floppy->open (m_driveIndex, newPath, ! writeable) ;
        m_ejectInsertButton->setText ("Eject") ;
        if (ok) {
            CFG->Set (m_floppyPathKey, newPath) ;
            m_parent->setFloppyLabel (m_driveIndex+1) ;
            this->accept() ;
        }
        break ;
    }
}


void FloppyDialog::onNewButtonClick (void)
{
    QString filter ("Disk Files (*.dsk *.DSK)|*.dsk;*.DSK|Any (*.*)|*.*") ;
    QFileDialog dlg (this, "Create new Disk", m_floppyPath, filter) ;

    int ret = dlg.exec() ;

    if (ret == QDialog::Accepted) {
        m_floppyPath = dlg.labelText (QFileDialog::FileName) ;
//        int err = MAC->m_floppy->create (m_driveIndex, m_floppyPath) ;
int err = 0  ;                                      // XXXXXXXXXXXXXXXXXXXXX FIXME XXXXXXXXXXXXXXXXXXXXXXXXXXXX
        if (err) {
            QMessageBox msg (QMessageBox::Warning,
                             "Error",
                             "Can't create the file.",
                             QMessageBox::Ok,
                             this) ;
            msg.exec() ;
        } else {
            CFG->Set (m_floppyPathKey, m_floppyPath) ;
        }
    }
}


void FloppyDialog::onDoneButtonClick (void)
{
    this->accept() ;
}


void FloppyDialog::onEjectInsertButtonClick (void)
{
    if (m_fileInfo->isDir()) insertFloppy() ;
    else                     ejectFloppy() ;
}


void floppyOnEjectInsertButtonClick(void) 
{
    floppyDialogPtr->onEjectInsertButtonClick() ;
}


void floppyOnNewButtonClick(void)
{
    floppyDialogPtr->onNewButtonClick() ;
}


void floppyOnDoneButtonClick(void)
{
    floppyDialogPtr->onDoneButtonClick() ;  
}
