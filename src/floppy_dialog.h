/*****************************************************************************

    File: "floppy_dialog.h"

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


#ifndef FLOPPYDIALOG_H
#define FLOPPYDIALOG_H

#include <QDialog>
#include <QLineEdit>

#include "mainwindow.h"



class FloppyDialog: public QDialog
{
	public:

		FloppyDialog (MainWindow* parent, int driveIndex) ;
		~FloppyDialog() ;
		void onDoneButtonClick (void) ;
		void onEjectInsertButtonClick (void) ;
		void onNewButtonClick (void) ;

		static FloppyDialog* dialog ;
		
	private:

		void insertFloppy (void) ;
		void ejectFloppy (void) ;

		MainWindow*  m_parent ;
		int          m_driveIndex ;
		QLabel*      m_noWriteWarning ;
		QLineEdit*   m_filenameField ;
		QPushButton* m_newFloppyButton ;
		QPushButton* m_doneButton ;
		QPushButton* m_ejectInsertButton ;
		QLabel*      m_StaticText2 ;
        QFileInfo*   m_fileInfo ;

		QString      m_floppyPath ;
		QString      m_floppyPathKey ;

};

#endif
