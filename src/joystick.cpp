
#include <stdio.h>
#include <QRect>
#include <QApplication>
#include <QDesktopWidget>

#include "defs.h"
#include "joystick.h"
//#include "mainwindow.h"



Joystick::Joystick (Machine* mac)
{
    m_mac = mac ;
    m_useMouse = true ;
    for (int i=0; i<3; i++) m_buttons[i] = false ;
    m_triggerCycles = mac->getCycles() ;
}


quint8 Joystick::readButton (int )
{
    return 0 ;
}


quint8 Joystick::readJoystick (int n)
{
    QRect r = QApplication::desktop()->screenGeometry() ;
    int w = r.width() ;
    int h = r.height() ;

    QPoint p = QCursor::pos() ;

    int position ;
    float mouseMax ;
    if (n) {
        mouseMax = h ;
        position = p.y() ;
    } else {
        mouseMax = w ;
        position = p.x() ;
    }

    quint64 deltaCycles = m_mac->getCycles() - m_triggerCycles ;
    float cyclesPerPixel = m_maxTimeoutCycles/mouseMax ;
    quint8 value = 0xff ;
    if (deltaCycles > (position*cyclesPerPixel)) value = 0 ;

    return value ;
}


void Joystick::reset (void)
{
    m_triggerCycles = m_mac->getCycles() ;
}
