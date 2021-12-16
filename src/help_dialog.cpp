

/******************************************************************

    This code has been temporarily abandoned due to missing 
    'QWebEngineView' support in Pi OS.

******************************************************************/



#include <QWebEngineView>

#include "defs.h"
#include "help_dialog.h"


Help::Help (MainWindow *parent) : QDialog(parent)
{
    m_parent = parent ;

    this->setMinimumSize (300,300) ;
    this->resize (600,680) ;

    m_browser = new QWebEngineView (this) ;
    m_browser->move (0,0) ;

    m_index = QString ("file:///usr/share/applepi/ApplePiHelp.html") ;
    m_browser->load (m_index) ;
    m_browser->show() ;
}


void Help::resizeEvent (QResizeEvent*)
{
    int w = this->width() ;
    int h = this->height() ;

    m_browser->resize (w, h) ;
}

