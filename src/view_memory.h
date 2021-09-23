/*****************************************************************************

    File: "view_memory.h"

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


#ifndef VIEW_MEMORY_H
#define VIEW_MEMORY_H


#include <QString>
#include <QDialog>
#include <QRadioButton>
#include <QLineEdit>
#include <QPushButton>
#include <QSlider>
#include <QScrollBar>
#include <QTimer>
#include <QEvent>
#include <QTimerEvent>
#include <QWheelEvent>
#include <QPaintEvent>
//#include <>
//#include <>
//#include <>
//#include <>

#include "mainwindow.h"

class Panel ;


class ViewMemory : public QDialog
{
public:

    ViewMemory (MainWindow* parent) ;
    virtual ~ViewMemory(void) ;

    bool romButtonIsChecked  (void) ;
    bool mainButtonIsChecked (void) ;
    bool auxButtonIsChecked  (void) ;

private:

//    void paintEvent (QPaintEvent* e) ;
    
    void OnRadioBox1Select  (void) ;
    void OnSetButtonClick   (void) ;
    //   void onSliderThumbUp (wxScrollEvent& event) ;
    void onSliderEvent      (void) ;
    void onRomButtonSelect  (void) ;
    void onMainButtonSelect (void) ;
    void onAuxButtonSelect  (void) ;
    void onSSButtonSelect   (void) ;
    void onTxtCtrlEntered   (void) ;
    void onSetButtonClicked (void) ;
    void onStartBoxFinished (void) ;
    void onStartBoxEdited   (void) ;
    void onSpeedSlider      (void) ;

    void keyPressEvent (QKeyEvent *e) ;
    void wheelEvent (QWheelEvent *e) ;

    void resizeEvent (QResizeEvent *event) ;
    QString finishAddress (QLineEdit* box) ;
    void editAddress (QLineEdit* box) ;
    
    void onScrollBar (void) ;
    void scroll    (qint16 delta) ;
    void page (int direction) ;
    void onPage    (void) ;
    void eventSize (void) ;
    void onClose   (void) ;

    void fixStartAddressText (void) ;

    Panel*  m_panel ;

    QRegExpValidator* m_hexValidator ;

    QString*      m_hexDigits ;
    QLineEdit*    m_startBox ;
 //   QPushButton*  m_setButton ;
    QRadioButton* m_mainButton ;
    QRadioButton* m_auxButton ;
    QRadioButton* m_ssButton ;
    QRadioButton* m_romButton ;
    QSlider*      m_speedSlider ;
    QScrollBar*   m_scrollBar ;

    quint8*   m_memPtr ;
    int       m_msDelay ;
    qint32    m_startAddr ;

    QLabel* m_staticText1 ;
    QLabel* m_staticText2 ;
    QLabel* m_staticText3 ;
    QLabel* m_staticText4 ;
    QLabel* m_staticText5 ;
    QLabel* m_staticText6 ;
} ;




class Panel : public QWidget
{
public:

    Panel (ViewMemory* parent) ;
    ~Panel() {} ;

    void onSize (int height) ;
    void newStartAddress (int address) ;
    void setTimer (int sliderValue) ;
    int  lines (void) ;
    int  lineHeight (void) ;

private:

    void onTimer (void) ;
    void paintEvent (QPaintEvent* e) ;

    ViewMemory* m_parent ;
    QFont       m_font ;
    QPixmap     m_screenBuffer ;
    QTimer*     m_timer ;
    qint32      m_startAddr ;
    int         m_width ;
    int         m_height ;
    int         m_nLines ;
    int         m_lineHeight ;

} ;

#endif
