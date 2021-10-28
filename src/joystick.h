
#ifndef  JOYSTICK_H
#define  JOYSTICK_H


#include <QtGlobal>
#include <QCursor>
#include <QScreen>
#include <QTimer>
#include <QMouseEvent>

#include "mainwindow.h"

class MainWindow ;


class Joystick : public QWidget
{

public:

    Joystick (QWidget* parent) ;
    ~Joystick() {} ;

    quint8 readButton (int n) ;
    quint8 readJoystick (int n) ;
    quint8 readJoystickFine (int n) ;
    quint8 readJoystickCoarse (int n) ;
    void   reset (void) ;
    void   setStyle (bool style) ;
    bool   getStyle (void) ;

private:

    MainWindow* m_parent ;

    void     readMouseData (void) ;

    bool     m_useMouse ;
    int      m_mouseFd ;
    bool     m_arcadeStyle ;
    bool     m_buttons[3] ;
    int      m_coarseDeltaX ;
    int      m_coarseDeltaY ;
    quint64  m_triggerCycles ;
    QTimer*  m_mouseTimer ;
    
   
    const float m_maxTimeoutCycles = 11*255 ; // Maximum CPU cycles for paddle circuit time-out ( = 2805)
    const float m_coarseK = 11*255 / 127 ;    // /dev/input/mice position-to-timeout conversion  constant

} ;

#endif
