

#include <QtCore/QString>
#include <QtGui/QClipboard>

#include "defs.h"
#include "machine.h"
#include "mainwindow.h"
#include "config.h"
#include "paste_to_keyboard.h"


Paste::Paste (MainWindow* parent, QString string)
{
    m_parent = parent ;
    m_string = string ;
}


void Paste::run (void)
{
    m_parent->pastingText(true) ;
//qStdOut() << "Paste::  text to paste: " << m_string << endl ;
    char c = 0 ;
    int  strLen = m_string.length() ;
    for (int i=0; i<strLen; i++) {
        c = m_string.at(i).cell() ;
        if ((c < 0x20) && (c != 0x0a) && (c != 0x0d)) c |= 0x40 ;
        if (c == 0x0a) c = 0x0d ;            // Convert <LF> to <CR>, which the Apple expects.
        MAC->m_ss[0] = c | 0x80 ;
        while (MAC->m_ss[0] & 0x80) usleep (10) ;
        if (MAC->m_ss[0] == 0x03) {         // Quit if <ctrl-C> was entered while we were sleeping.
            MAC->m_ss[0] = 0x8d ;
            break ;
        }
    }
    
    m_parent->pastingText(false) ;

    return ;

}
