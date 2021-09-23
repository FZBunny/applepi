
#ifndef HD_DIALOG_H
#define HD_DIALOG_H


#include <QDialog>
#include <QLineEdit>
#include <QFileDialog>

#include "mainwindow.h"


class HardDriveDialog: public QDialog
{
	public:
/**
		HardDriveDialog (wxWindow*      parent,
                         wxWindowID     id=wxID_ANY,
                         const QPoint& pos=wxDefaultPosition,
                         const QSize&  size=wxDefaultSize) ;
**/
		HardDriveDialog (MainWindow* parent, int drive) ;
		~HardDriveDialog() ;

		void onDoneButtonClick (void) ;
		void onEjectInsertButtonClick (void) ;
		void onNewButtonClick (void) ;

        QString  GetPath(void) ;

	private:

		void insertVolume (void) ;
		void ejectVolume (void) ;

		MainWindow*  m_parent ;
		int          m_driveIndex ;
		QLabel*      m_noWriteWarning ;
		QLineEdit*   m_filenameField ;
		QPushButton* m_newVolumeButton ;
		QPushButton* m_doneButton ;
		QPushButton* m_ejectInsertButton ;
		QLabel*      m_StaticText2 ;
        QFileInfo*   m_fileInfo ;

		QString      m_volumePath ;
		QString      m_volumePathKey ;

};

#endif
