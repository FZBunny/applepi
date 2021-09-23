
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
