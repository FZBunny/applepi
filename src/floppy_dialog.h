
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
