
#ifndef  PADDLES_H
#define  PADDLES_H


#include <QtGlobal>
#include <QCursor>
#include <QScreen>
#include <QMouseEvent>

#include "machine.h"


class Paddles : QMouseEvent
{

public:

    Paddles (Machine* mac) ;
    ~Paddles() {} ;

    quint8 readButton (int n) ;
    quint8 readPaddle (int n) ;
    void   reset (void) ;

private:

    Machine* m_mac ;
    bool     m_useMouse ;
    quint64  m_triggerCycles ;

    const float m_maxTimeoutCycles = 11*255 ; // Maximum CPU cycles for paddle circuit time-out ( = 2805)

} ;

#endif
