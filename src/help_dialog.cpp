
//  *****************  UNFINISHED  ***********************

#include <QHBoxLayout>


#include "defs.h"
#include "help_dialog.h"


Help::Help (MainWindow *parent) : QDialog(parent)
{
    m_parent = parent ;

    this->setMinimumSize (300,300) ;
    this->resize (450,520) ;

    m_browser = new QTextBrowser (this) ;
    m_browser->move (0,30) ;

    m_homeButton  = new QPushButton ("&Home", this) ;
    m_backButton  = new QPushButton ("&Back", this) ;
    m_closeButton = new QPushButton ("Close", this) ;
    m_closeButton->setShortcut (QKeySequence("Esc")) ;

    connect (m_homeButton,  &QPushButton::clicked, this, &Help::onHome) ;
    connect (m_backButton,  &QPushButton::clicked, this, &Help::onBack) ;
    connect (m_closeButton, &QPushButton::clicked, this, &Help::onClose) ;
  //  connect (m_browser, SIGNAL(sourceChanged(const QUrl &)), this, &Help::updateWindowTitle) ;

    m_index = QString ("ApplePiHelp.html") ;
    m_browser->setSearchPaths (QStringList() << SHARE_DIR) ;
    m_browser->setSource (m_index) ;
}


void Help::onHome (void)
{

}


void Help::onBack (void)
{

}


void Help::onClose (void)
{

}


void Help::resizeEvent (QResizeEvent*)
{
    int w = this->width() ;
    int h = this->height() ;
    m_homeButton->move (10,0) ;
    m_backButton->move (100,0) ;
    int cw = m_closeButton->width() ;
    m_closeButton->move (w-cw,0) ;

    m_browser->resize (w, h-30) ;
}

