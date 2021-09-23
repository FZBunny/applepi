

#include <stdio.h>
#include <QDialog>
#include <QRadioButton>
#include <QPaintEvent>


#include "view_memory.h"
#include "defs.h"
#include "machine.h"
#include "config.h"
#include "debugging_dumps.h"



ViewMemory::ViewMemory(MainWindow* parent) :  QDialog (parent)
{
    this->setWindowTitle ("View Memory") ;

    m_panel = new Panel (this) ;

    int width = 770 ;
    resize (QSize(width,450)) ;
    setMinimumSize (QSize(width,120)) ;
    setMaximumSize (QSize(width,99999)) ;

    m_hexValidator = new QRegExpValidator (QRegExp ("[0123456789ABCDEFabcdef]{1,5}"), this) ;

    m_startBox    = new QLineEdit    ("0000", this) ;
    m_romButton   = new QRadioButton ("Rom", this) ;
    m_mainButton  = new QRadioButton ("Main", this) ;
    m_auxButton   = new QRadioButton ("Aux", this) ;
    m_ssButton    = new QRadioButton ("", this) ;

    m_startBox->move (56,36) ;
    m_romButton->move (174,40) ;
    m_mainButton->move (238,40) ;
    m_auxButton->move (302,40) ;
    m_ssButton->move (362,40) ;

    m_startBox->resize (52,24) ;
    m_startBox->setAlignment (Qt::AlignCenter) ;
    m_startBox->setValidator (m_hexValidator) ;

    m_scrollBar = new QScrollBar (Qt::Vertical, this) ;
    m_scrollBar->resize (20, this->height()-80) ;
    m_scrollBar->move (this->width()-20, 72) ;
    m_scrollBar->setMinimum (0) ;
    m_scrollBar->setMaximum (0xffff) ;
    m_scrollBar->setSingleStep (16) ;

    m_speedSlider = new QSlider (Qt::Horizontal, this) ;
    m_speedSlider->resize (216,30) ;
    m_speedSlider->move (500,25) ;
    m_speedSlider->setMinimum (0) ;
    m_speedSlider->setMaximum (1000) ;
    m_speedSlider->setValue (800) ;

    m_panel->resize (this->width(), this->height()-80) ;
    m_panel->move (8,72) ;
    m_panel->newStartAddress (0) ;

    m_staticText1 = new QLabel("Start Address", this) ;
    m_staticText2 = new QLabel("Refresh Speed", this) ;
    m_staticText3 = new QLabel("Slow", this) ;
    m_staticText4 = new QLabel("Fast", this) ;
    m_staticText5 = new QLabel("Memory Type", this) ;
    m_staticText6 = new QLabel("Soft\nSwitches", this) ;

    m_staticText1->move (40,16) ;
    m_staticText2->move (570,14) ;
    m_staticText3->move (490,48) ;
    m_staticText4->move (696,48) ;
    m_staticText5->move (245,16) ;
    m_staticText6->move (386,32) ;

    connect (m_startBox,    &QLineEdit::editingFinished, this,  &ViewMemory::onStartBoxFinished) ;
    connect (m_startBox,    &QLineEdit::textEdited,      this,  &ViewMemory::onStartBoxEdited) ;
    connect (m_scrollBar,   &QScrollBar::valueChanged,   this,  &ViewMemory::onScrollBar) ;
    connect (m_speedSlider, &QScrollBar::valueChanged,   this,  &ViewMemory::onSpeedSlider) ;

    m_mainButton->setChecked (true) ;
    m_startAddr = 0 ;

    this->show() ;
    m_scrollBar->setPageStep (160) ;  // A bit of a kludge, but necessary...
}


ViewMemory::~ViewMemory (void)
{
 //   m_timer->stop() ;
}


void ViewMemory::resizeEvent (QResizeEvent *e)
{
    int h = e->size().height() ;
    m_panel->onSize (h-80) ;
    m_scrollBar->resize (20, this->height()-80) ;

    int stepSize = 16 * m_panel->lines() ;
    m_scrollBar->setPageStep (stepSize) ;
// printf ("stepSize=%4.4X\n", stepSize) ;
}


void ViewMemory::onScrollBar (void)
{
    int pos = m_scrollBar->value() ;
    pos &= 0xfff0 ;

    char buffer[16] ;
    sprintf (buffer, "%4.4X", pos) ;
    m_startBox->setText (QString(buffer)) ;
//qStdOut() << "onScrollBar " << QString(buffer) << endl ;
    m_panel->newStartAddress (pos) ;
  
}


void ViewMemory::scroll (qint16 delta)
{
    int pos = m_scrollBar->value() ;
    pos += delta ;
    if (pos < 0) pos = 0 ;
    if (pos > 0xfff0) pos = 0xfff0 ;

    char buffer[16] ;
    sprintf (buffer, "%4.4X", pos) ;
    m_startBox->setText (QString(buffer)) ;
    m_panel->newStartAddress (pos) ;
    m_scrollBar->setValue (pos) ;

}


void ViewMemory::page (int direction)
{
    int nLines = m_panel->lines() ;
    int newAddress = m_scrollBar->value() ;
    if (direction < 0) {
        newAddress -= 16*nLines ;
        if (newAddress < 0) newAddress = 0 ;
    } else {
        newAddress += 16*nLines ;
        if (newAddress > 0xfff0) newAddress = 0xfff0 ;
    }

    char buffer[16] ;
    sprintf (buffer, "%4.4X", newAddress) ;
    m_startBox->setText (QString(buffer)) ;
    m_panel->newStartAddress (newAddress) ;
    m_scrollBar->setValue (newAddress) ;

}


void ViewMemory::onStartBoxEdited (void)
{
    editAddress (m_startBox) ;
}


void ViewMemory::editAddress (QLineEdit* box)
{
    QString text = box->text() ;
    int pos = box->cursorPosition() ;
    int len = text.length() ;
    if (pos > 4) {
        text = text.remove (0,1) ;
        box->setText (text) ;
    } else {
        if (len > 4) text = text.remove (len-1,1) ;
        box->setText (text) ;
        box->setCursorPosition (pos) ;
    }
}


QString ViewMemory::finishAddress (QLineEdit* box)
{
    QString text (box->text().toUpper()) ;
    int addr = text.toUInt (NULL, 16) ;
    addr &= 0xfff0 ;
    text.asprintf ("%4.4X", addr) ;
    box->setText (text) ;
    return text ;
}


void ViewMemory::onStartBoxFinished (void)
{
    QString s = finishAddress (m_startBox) ;

    bool ok ;
    int addr = s.toInt (&ok, 16) ;
    addr &= 0xfff0 ;

    m_scrollBar->setValue (addr) ;
    m_panel->newStartAddress (addr) ;
}


void ViewMemory::wheelEvent (QWheelEvent *e)
{
    int delta = e->angleDelta().y() ;
    if (delta < 0) {
        scroll (16) ;
    } else {
        scroll (-16) ;
    }
}


void ViewMemory::onSpeedSlider (void)
{
//printf ("slider value = %i\n", m_speedSlider->value()) ;
    int newDelay = 1000 - m_speedSlider->value() ;
    if (newDelay < 50) newDelay = 50 ;
    m_panel->setTimer (newDelay) ;
}


void ViewMemory::onClose (void)
{
 //   this->Destroy() ;
}


bool ViewMemory::romButtonIsChecked  (void)
{
    return m_romButton->isChecked() ;
}


bool ViewMemory::mainButtonIsChecked (void)
{
    return m_mainButton->isChecked() ;
}


bool ViewMemory::auxButtonIsChecked  (void)
{
    return m_auxButton->isChecked() ;
}


void ViewMemory::keyPressEvent (QKeyEvent *e)
{
    quint16 c = e->nativeVirtualKey() ;
//printf ("c=%4.4x\n", c) ;

    switch (c) {
       case 0xff52:
          scroll (-16) ;
          break ;
       case 0xff54:
          scroll (+16) ;
          break ;
       case 0xff55:
          page (-1) ;
          break ;
       case 0xff56:
          page (1) ;
          break ;
       default:
          break ; 
    }
}



// ----------------------------------------------------------------
//                                                                -
//                           P A N E L                            -
//                                                                -
// ----------------------------------------------------------------

    

Panel::Panel (ViewMemory* parent)
{
    m_parent = parent ;
    this->setParent (parent) ;

    m_width  = 742 ;
    m_height = this->height() ;

    m_timer = new QTimer (this) ;
    m_timer->start (100) ; // // XXXXXXXXXXXXXXXXX temporary XXXXXXXXXXXXXXXXXXXXXXX

    connect (m_timer, &QTimer::timeout, this, &Panel::onTimer) ;

    m_font = QFont ("DejaVu Sans Mono", 12) ;
    QFontMetrics fm (m_font);
    m_lineHeight = fm.height() ;

    this->show() ;
//    this->update (this->rect()) ;

}


void Panel::setTimer (int newDelay)
{
    m_timer->stop() ;
    m_timer->start (newDelay) ;
}


void Panel::newStartAddress (int address)
{
    m_startAddr = address ;
}


void Panel::paintEvent (QPaintEvent* e)
{  
    QPainter painter (this) ;
    painter.drawPixmap (e->rect(), m_screenBuffer, m_screenBuffer.rect()) ;
}


void Panel::onSize (int newHeight)
{
    this->resize (m_width, newHeight) ;
}


int Panel::lines (void)
{
    return m_nLines ;
}


int Panel::lineHeight (void)
{
    return m_lineHeight ;
}


void Panel::onTimer (void)
{
    m_screenBuffer = QPixmap (this->width(), this->height()) ;
    m_screenBuffer.fill (Qt::white) ;

    QPainter painter (&m_screenBuffer) ;   
    painter.setFont (m_font) ;
    m_nLines = this->height() / m_lineHeight ;

    quint8* p ;
    bool ssPage ;
    int start = m_startAddr ;
    if (m_parent->romButtonIsChecked()) {
        p = MAC->m_rom ;
        ssPage = false ;
    } else if (m_parent->mainButtonIsChecked()) {
        p = MAC->m_ram ;
        ssPage = false ;
    } else if (m_parent->auxButtonIsChecked()) {
        p = MAC->m_aux ;
        ssPage = false ;
    } else {
        p = MAC->m_ss ;
        ssPage = true ;
        start = 0 ;
    }

    for (int line=0; line<m_nLines; line++) {

        int ix = start + line*16 ;
        int displayAddress = ix ;
        if (ssPage) displayAddress = ix + 0xC000 ;

        char buffer[80] ;
        int n = sprintf (buffer, 
                 "%4.4X  %2.2X %2.2X %2.2X %2.2X  %2.2X %2.2X %2.2X %2.2X  %2.2X %2.2X %2.2X %2.2X  %2.2X %2.2X %2.2X %2.2X ",
                 displayAddress,
                 p[ix],   p[ix+1], p[ix+2],  p[ix+3],  p[ix+4],  p[ix+5],  p[ix+6],  p[ix+7],
                 p[ix+8], p[ix+9], p[ix+10], p[ix+11], p[ix+12], p[ix+13], p[ix+14], p[ix+15]) ;
        int bix = 0 ; // (bix == 'buffer index')
        for  (int i=0; i<16; i++) {                       // Add characters after hex
            if (i%4 == 0) buffer[n+bix++] = ' ' ;
            char c = p[ix+i] & 0x7f ;
            if (c < 0x20)  c = '.' ;
            if (c == 0x7f) c = '.' ;
            buffer[n+bix++] = c ;
        }
        buffer[n+bix] = 0 ;
        painter.drawText (10,20+line*m_lineHeight, QString((const char*)buffer)) ;

        if (ssPage && line >= 15) break ;
        if (ix > 0xFFE0)  break ;
    }

    QWidget::update (this->rect()) ;
}

