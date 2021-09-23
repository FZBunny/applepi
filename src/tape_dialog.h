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
