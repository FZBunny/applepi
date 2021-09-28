/*****************************************************************************

    File: "hd_dialog.cpp"

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





#include <sys/types.h>
#include <sys/stat.h>
#ifndef Q_OS_WINDOWS
#  include <unistd.h>
#endif

#include <QMessageBox>

#include "hd_dialog.h"
#include "mainwindow.h"
#include "machine.h"
#include "config.h"
#include "defs.h"
#include "debugging_dumps.h"


HardDriveDialog* hardDriveDialogPtr ;

void hdOnEjectInsertButtonClick(void) ;
void hdOnNewButtonClick(void) ;
void hdOnDoneButtonClick(void) ;


HardDriveDialog::HardDriveDialog (MainWindow* parent, int drive) : QDialog ()
{
    m_parent = parent ;

    hardDriveDialogPtr = this ;

	m_driveIndex = drive - 1 ;
    if (m_driveIndex < 0) m_driveIndex = 0 ;
    if (m_driveIndex > 1) m_driveIndex = 1 ;

    int width = 420 ;
    int height = 258 ;
    int x, y ;
    CENTER_DIALOG

    if (m_driveIndex)  m_volumePathKey = "hd2_volume_path" ;
    else               m_volumePathKey = "hd1_volume_path" ;

    CFG->Get (m_volumePathKey, &m_volumePath) ;
// qStdOut() << "drive " << drive << "  m_volumePath = " << m_volumePath << endl ;
	QString title = "3.5\" floppy or Hard Drive Volume" ;
	this->setWindowTitle (title) ;

	m_filenameField  = new QLineEdit (this) ;
	m_noWriteWarning = new QLabel ("* File is Write Protected *", this) ;
	m_StaticText2    = new QLabel ("Current File or Directory", this) ;
    m_fileInfo       = new QFileInfo() ;
    m_ejectInsertButton = new QPushButton ("Choose a Volume", this) ;
    m_newVolumeButton   = new QPushButton ("Make a New Volume", this) ;
    m_doneButton        = new QPushButton ("Done", this) ;
    
	connect (m_ejectInsertButton, &QPushButton::clicked, ::hdOnEjectInsertButtonClick) ;
	connect (m_doneButton,        &QPushButton::clicked, ::hdOnDoneButtonClick) ;
	connect (m_newVolumeButton,   &QPushButton::clicked, ::hdOnNewButtonClick) ;

    m_filenameField->move (16,40) ;        m_filenameField->resize (384,26) ;
    m_noWriteWarning->move (135,72) ;      m_noWriteWarning->resize (160,20) ;
    m_StaticText2->move (120,16) ;         m_StaticText2->resize (180,20) ;
    m_ejectInsertButton->move (135,100) ;  m_ejectInsertButton->resize (152,34) ;
    m_newVolumeButton->move (135,144) ;    m_newVolumeButton->resize (152,34) ;
    m_doneButton->move (165,192) ;

    m_filenameField->insert (m_volumePath) ;
    m_fileInfo->setFile (m_volumePath) ;

    if (m_fileInfo->isDir() || m_volumePath.isEmpty()) {
        m_ejectInsertButton->setText ("Insert a Volume") ;
        m_newVolumeButton->setEnabled(true) ;
        m_noWriteWarning->hide() ;
    } else {
        m_ejectInsertButton->setText ("Eject") ;
        m_newVolumeButton->setEnabled(false) ;
        if (m_fileInfo->isWritable()) {
            m_noWriteWarning->hide() ;
        } else {
            m_noWriteWarning->show() ;
        }
    }

    m_newVolumeButton->setEnabled(false) ; // XXXXXXXXXXXXXXXXXXXXXXXX formatting code is  unfinished as of 2021-07-20 XXXXXXXXXXXXXXXX

}


HardDriveDialog::~HardDriveDialog()
{
    delete m_fileInfo ;
}


//        ================================
//        =   B U T T O N    S L O T S   =
//        ================================


void HardDriveDialog::onDoneButtonClick (void)
{
    this->accept() ;
}


void HardDriveDialog::onEjectInsertButtonClick (void)
{
    if (m_fileInfo->isDir() || m_volumePath.isEmpty()) insertVolume() ;
    else                                               ejectVolume() ;
}


void HardDriveDialog::insertVolume (void)
{
    QString newPath ;
    QString filter ("Disk Files (*.img *.IMG);;Any *") ;
    QString originalPath = m_filenameField->text() ;

    while (true) {

        newPath = QFileDialog::getOpenFileName (this, "Open a 3.5\" floppy or HD volume image", m_volumePath, filter) ;
        if ( ! newPath.isEmpty()) {
            m_volumePath = newPath ;
            m_filenameField->setText (newPath) ;
            m_fileInfo->setFile (newPath) ;
        }
        if (m_fileInfo->isDir()) {
             m_ejectInsertButton->setText ("Insert a Floppy") ;
             m_newVolumeButton->setEnabled (true) ;
        } else {
            if (m_fileInfo->size()<819200) {
                QString msgText ;
                QTextStream (&msgText) << "\"" << m_fileInfo->fileName() << "\" is too small to be a 3.5\" floppy or a hard drive." ;
                QMessageBox msg (QMessageBox::Warning, "Error", msgText, QMessageBox::Ok, this) ;
                msg.exec() ;
                m_filenameField->setText (originalPath) ;
                break ;
            }
            m_ejectInsertButton->setText ("Eject") ;
            m_newVolumeButton->setEnabled (false) ;
        }
                                                                //  XXXXXX CHECK RETURN VALUE!  FIXME XXX
        MAC->m_hardDrive->open (m_volumePath, m_driveIndex) ;   // Set label under HD button with the voume name 
        quint8 buffer[BLOCKSIZE] ;
        MAC->m_hardDrive->readBlock (buffer, 2, m_driveIndex) ;
        int nameLen = buffer[4] & 0x0f ;  // (volume name starts at 6th byte of block 2)
        buffer[nameLen+5] = 0 ;
        quint8 tmp[16] ;
        QString label ;
        strcpy ((char*)tmp, (char*)buffer+5) ;
        label = QString ((const char*)tmp) ;
        QString slashLabel ("/") ;
        slashLabel += label ;
        m_parent->setHDLabel (m_driveIndex+1, slashLabel) ;
        CFG->Set (m_volumePathKey, newPath) ;
        break ;
    }

}


void HardDriveDialog::ejectVolume (void)  // XXXXXX  DOESN'T WORK IF FILE DNE XXXXXXXXXXXX
{
// printf ("HardDriveDialog::ejectVolume   m_driveIndex %i\n", m_driveIndex) ;
    if (m_fileInfo->isDir()) return ;
    m_volumePath = m_fileInfo->dir().path() ;
    m_filenameField->clear() ;
    m_filenameField->insert (m_volumePath) ;
    m_fileInfo->setFile (m_volumePath) ;
    CFG->Set (m_volumePathKey, m_volumePath) ;

    m_ejectInsertButton->setText ("Insert an HD Volume") ;
    m_newVolumeButton->setEnabled (true) ;
    m_noWriteWarning->hide() ;

    MAC->m_hardDrive->close (m_driveIndex) ;
    m_parent->setHDLabel (m_driveIndex+1, "- empty drive -") ;
    
    m_newVolumeButton->setEnabled (false) ; // XXXXXXXXXXXXXXXXXXXXXXXX formatting code is  unfinished as of 2021-07-20 XXXXXXXXXXXXXXXX

}


//  XXXXXXXXXXXXXXXXXXXXXXXXXXXXX UNFINISHED XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  * must ask if a floppy or 32MB hard drive is to be created
//    ( choose: 3.5" or HD (32MB) )
//  * must have message directing user to use utility disk to format new disk.
//  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void HardDriveDialog::onNewButtonClick (void) {} ;

/***
void HardDriveDialog::onNewButtonClick (wxCommandEvent&)
{
    QString filter ("Any (*)|*") ;
    QFileDialog dlg (this, "Create new HD volume", m_volumePath, "", filter, wxFD_SAVE|wxFD_OVERWRITE_PROMPT) ;
    dlg.SetDirectory (m_volumePath) ;

    int ret = dlg.ShowModal() ;

    if (ret == wxID_OK) {
        m_volumePath = dlg.GetPath() ;

        int err = MAC->m_hardDrive->create (m_volumePath, m_driveIndex, 1600) ;  //  XXXXX FIX THIS CONSTANT ! XXXXXXXXXXXXXX
        if (err) {
            wxMessageDialog msg (this, "Can't create the file.",
                                 "Error", wxOK|wxCENTRE|wxICON_ERROR) ;
            msg.ShowModal() ;
        } else {
            CFG->Set (m_volumePathKey, m_volumePath) ;
        }
        this->EndModal (true) ;
        m_ejectInsertButton->SetLabel ("Eject") ;
    }

}
***/

QString HardDriveDialog::GetPath(void)
{
    return m_volumePath ;
}


void hdOnEjectInsertButtonClick (void)
{
    hardDriveDialogPtr->onEjectInsertButtonClick() ;
}


void hdOnNewButtonClick (void)
{
    hardDriveDialogPtr->onNewButtonClick() ;
}


void hdOnDoneButtonClick (void)
{
    hardDriveDialogPtr->onDoneButtonClick() ;
}
