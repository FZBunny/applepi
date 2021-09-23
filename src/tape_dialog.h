/*****************************************************************************

    File: "tape_dialog.h"

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



#ifndef TAPEDIALOG_H
#define TAPEDIALOG_H


#include <wx/dialog.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/filename.h>


class TapeDialog: public wxDialog
{
	public:

		TapeDialog (wxWindow*      parent,
                    int            driveIndex,
                    wxWindowID     id=wxID_ANY,
                    const QPoint& pos=wxDefaultPosition,
                    const QSize&  size=wxDefaultSize) ;

		~TapeDialog() ;

//		bool isDirectory (QString path) ;

	protected:

		static const long ID_EJECTINSERT ;
		static const long ID_DONE ;
		static const long ID_NEWTAPE ;
		static const long ID_TEXTCTRL ;
		static const long ID_NOWRITE ;
		static const long ID_LABLE1 ;

	private:

		void OnDoneButtonClick (wxCommandEvent& event) ;
		void onEjectInsertButtonClick (wxCommandEvent& event) ;
		void onNewButtonClick (wxCommandEvent& event) ;
		void insertTape (void) ;
		void ejectTape (void) ;

		QLabel* m_noWriteWarning ;
		wxTextCtrl*   m_filenameField ;
		QPushButton*     m_newTapeButton ;
		QPushButton*     m_doneButton ;
		QPushButton*     m_ejectInsertButton ;
		QLabel* m_StaticText2 ;
        QFileInfo   *m_fileName ;

		QString      m_tapePath ;
		QString      m_tapePathKey ;

};

#endif
