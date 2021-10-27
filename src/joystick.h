
#ifndef  JOYSTICK_H
#define  JOYSTICK_H


#include <QtGlobal>
#include <QCursor>
#include <QScreen>
#include <QMouseEvent>

#include "machine.h"


class Joystick
{

public:

    Joystick (Machine* mac) ;
    ~Joystick() {} ;

    quint8 readButton (int n) ;
    quint8 readJoystick (int n) ;
    void   reset (void) ;

private:

    Machine* m_mac ;
    bool     m_useMouse ;
    bool     m_buttons[3] ;
    quint64  m_triggerCycles ;
    
    // Maximum CPU cycles for paddle circuit time-out ( = 2805)
    const float m_maxTimeoutCycles = 11*255 ;

} ;

#endif
