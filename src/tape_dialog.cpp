

#include <wx/string.h>
#include <wx/intl.h>
#include <wx/filedlg.h>
#include <wx/msgdlg.h>

#include <sys/types.h>
#include <sys/stat.h>
#ifndef __WXMSW__
#  include <unistd.h>
#endif


#include "tape_dialog.h"
#include "machine.h"
#include "config.h"
#include "defs.h"


// extern Machine* wxTheApp->MAC ;
// extern Config*  exTheApp->CFG ;



const long TapeDialog::ID_EJECTINSERT = wxNewId();
const long TapeDialog::ID_DONE = wxNewId();
const long TapeDialog::ID_NEWTAPE = wxNewId();
const long TapeDialog::ID_TEXTCTRL = wxNewId();
const long TapeDialog::ID_NOWRITE = wxNewId();
const long TapeDialog::ID_LABLE1 = wxNewId();


// XXXXXXXXXXXx   ADD A REWIND BUTTON! XXXXXXXXXXXXXXXXXXXXXXXX

TapeDialog::TapeDialog (wxWindow*      parent,
                        int            driveIndex,
                        wxWindowID     id,
                        const QPoint& pos,
                        const QSize&  size)   : wxDialog ()
{

	Create (parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id")) ;
	SetClientSize (QSize(419,258)) ;
	Move (wxDefaultPosition) ;

    m_tapePathKey = "tape_path" ;

    CFG->Get(m_tapePathKey, &m_tapePath) ;

	QString title = _("Cassette drive");
	this->SetTitle(title) ;

	m_ejectInsertButton = new QPushButton (this, ID_EJECTINSERT, _("Insert a Tape"), QPoint(150,100), QSize(128,34)) ;
	m_doneButton = new QPushButton (this, ID_DONE, _("Done"), QPoint(168,192), wxDefaultSize) ;
	m_newTapeButton = new QPushButton (this, ID_NEWTAPE, _("Make a New Tape"), QPoint(150,144), QSize(128,34)) ;
	m_filenameField = new wxTextCtrl (this, ID_TEXTCTRL, wxEmptyString, QPoint(16,40), QSize(384,26)) ;
	m_noWriteWarning = new QLabel (this, ID_NOWRITE, _("File is Write Protected"), QPoint(144,72), QSize(160,16)) ;
	m_StaticText2 = new QLabel (this, ID_LABLE1, _("Current File or Directory"), QPoint(138,16), QSize(160,16)) ;
    m_fileName = new QFileInfo() ;

	Connect (ID_EJECTINSERT, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&TapeDialog::onEjectInsertButtonClick) ;
	Connect (ID_NEWTAPE, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&TapeDialog::onNewButtonClick) ;
	Connect (ID_DONE, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&TapeDialog::OnDoneButtonClick) ;

    m_filenameField->SetValue (m_tapePath) ;
    m_fileName->Assign (m_tapePath) ;


    if (isDirectory(m_tapePath) || m_tapePath.IsEmpty()) {  //  QFileInfo function IsDir() always returns FALSE (!)
        m_ejectInsertButton->SetLabel (_("Insert a Tape")) ;
        m_newTapeButton->Enable() ;
        m_noWriteWarning->Hide() ;
    } else {
        m_ejectInsertButton->SetLabel (_("Eject")) ;
        m_newTapeButton->Disable() ;
        if (m_fileName->IsFileWritable()) {
            m_noWriteWarning->Hide() ;
        } else {
            m_noWriteWarning->Show() ;
        }
    }

}



TapeDialog::~TapeDialog()
{
    delete m_ejectInsertButton ;
    delete m_doneButton ;
    delete m_newTapeButton ;
    delete m_fileName ;
    delete m_noWriteWarning ;
    delete m_StaticText2 ;
}



//        ================================
//        =   B U T T O N    S L O T S   =
//        ================================



void TapeDialog::OnDoneButtonClick (wxCommandEvent& event)
{
    this->EndModal (false) ;
}



void TapeDialog::onEjectInsertButtonClick (wxCommandEvent& event)
{
    if (isDirectory(m_tapePath) || m_tapePath.IsEmpty()) insertTape() ;
    else                                                 ejectTape() ;
}



void TapeDialog::insertTape (void)
{
    QString newPath ;

//    + "|Any (" + QFileSelectorDefaultWildcardStr + ")|" + QFileSelectorDefaultWildcardStr) ;  // FIX THIS XXXXXXXXXXXXX
    QString filter ("Tape Files (*.tape *.TAPE)|*.tape;*.TAPE|Any (*.*)|*") ;
    QFileDialog dlg (this, "Open Tape file", m_tapePath, "", filter, wxFD_OPEN|wxFD_FILE_MUST_EXIST) ;

    dlg.SetDirectory (m_tapePath) ;

    int ret = dlg.ShowModal() ;

    if (ret == wxID_OK) {
        newPath = dlg.GetPath() ;
        CFG->Set (m_tapePathKey, newPath) ;
        m_tapePath = newPath ;
        *m_filenameField << newPath ;
        m_fileName->Assign (m_tapePath) ;

        if (isDirectory(m_tapePath)) return ;  // This probably can't happen...

        bool writeable = m_fileName->IsFileWritable() ;
        if (writeable) m_noWriteWarning->Hide() ;
        else           m_noWriteWarning->Show() ;

        MAC->m_tape->open (m_tapePath, writeable) ;  //   XXXXX need to check return value XXXXXXXXXXXXXXXXXXXXXX

        this->EndModal (true) ;
        m_ejectInsertButton->SetLabel (_("Eject")) ;
    }

}



void TapeDialog::ejectTape (void)
{

    if (isDirectory(m_tapePath)) return ;

    MAC->m_tape->close() ;

    m_tapePath = m_fileName->GetPath() ;
    m_filenameField->SetValue (m_tapePath) ;
    m_fileName->Assign (m_tapePath) ;
    CFG->Set (m_tapePathKey, m_tapePath) ;

    m_ejectInsertButton->SetLabel (_("Insert a Tape")) ;
    m_newTapeButton->Enable() ;
    m_noWriteWarning->Hide() ;
}



void TapeDialog::onNewButtonClick (wxCommandEvent&)
{
    QString filter ("Tape Files (*.tape *.TAPE)|*.tape;*.TAPE|Any (*)|*") ;
    QFileDialog dlg (this, "Create new Tape", m_tapePath, "", filter, wxFD_SAVE|wxFD_OVERWRITE_PROMPT) ;
    dlg.SetDirectory (m_tapePath) ;

    int ret = dlg.ShowModal() ;

    if (ret == wxID_OK) {
        m_tapePath = dlg.GetPath() ;
        int err = MAC->m_tape->create (m_tapePath) ;
        if (err) {
            wxMessageDialog msg (this, "Can't create the file.",
                                 "Error", wxOK|wxCENTRE|wxICON_ERROR) ;
            msg.ShowModal() ;
        } else {
            CFG->Set (m_tapePathKey, m_tapePath) ;
        }
        this->EndModal (true) ;
        m_ejectInsertButton->SetLabel (_("Eject")) ;
    }

}


/***
//  This function exists only because (in wxWidgets 3.0 under Mint 19.2) QFileInfo fn. "IsDir" always returns false.
//  This function is, of course, not multi-platform.... crap!

bool TapeDialog::isDirectory (QString path)
{
    struct stat status ;
    char pathBuffer [10000] ;

    strncpy (pathBuffer, path.ToAscii(), 9998) ;
//printf ("pathBuffer = '%s'\n", pathBuffer) ;
    stat (pathBuffer, &status) ;
    if (status.st_mode & S_IFDIR) return true ;
    else                          return false ;
}
***/
