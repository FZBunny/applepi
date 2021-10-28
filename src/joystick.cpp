
#include <stdio.h>
#include <QRect>
#include <QApplication>
#include <QDesktopWidget>

#include "defs.h"
#include "joystick.h"
#include "mainwindow.h"



Joystick::Joystick (QWidget* parent) : QWidget (parent)
{
    char* mouseDev = (char*)"/dev/input/mice" ;
    m_parent = (MainWindow*)parent ;
    m_useMouse = true ;
    m_arcadeStyle = true ;
    for (int i=0; i<3; i++) m_buttons[i] = false ;
    m_triggerCycles = MAC->getCycles() ;

    m_mouseTimer = new QTimer (this) ;
    connect (m_mouseTimer, &QTimer::timeout, this, &Joystick::readMouseData) ;
    m_mouseTimer->start (49) ;

    m_mouseFd = open (mouseDev, O_RDONLY|O_NONBLOCK) ;
    if (m_mouseFd < 0) {
        fprintf (stderr, "\n") ;
        fprintf (stderr, "* Could not open \"%s\": %s\n", mouseDev, strerror(errno)) ;
        fprintf (stderr, "* You will not be able to use mouse buttons or 'arcade' style joystick\n") ;
        fprintf (stderr, "* when using the mouse as a substitute joystick.\n") ;
        fprintf (stderr, "* Use 'Help' to learn how to fix this.\n") ;
        fprintf (stderr, "\n") ;
    }
}


//  If style is true, use "arcade" style (coarse) control
//  Else use "flight simulator" (fine) control

void Joystick::setStyle (bool style)
{
    m_arcadeStyle = style ;
}


bool Joystick::getStyle (void)
{
    return m_arcadeStyle ;
}


quint8 Joystick::readButton (int n)
{
    switch (n) {
        case 0:
          return m_buttons[0] ;
        case 2:
          return m_buttons[1] ;
        case 3:
          return m_buttons[2] ;
        default:
          return 0 ;
    }
}


quint8 Joystick::readJoystick (int n)
{
    if (m_arcadeStyle) return readJoystickCoarse (n) ;
    else               return readJoystickFine (n) ;
}


quint8 Joystick::readJoystickFine (int n)
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

    quint64 deltaCycles = MAC->getCycles() - m_triggerCycles ;
    float cyclesPerPixel = m_maxTimeoutCycles/mouseMax ;
    quint8 value = 0xff ;
    if (deltaCycles > (position*cyclesPerPixel)) value = 0 ;

    return value ;
}


quint8 Joystick::readJoystickCoarse (int n)
{
    int mouseDelta ;
    if (n) mouseDelta = m_coarseDeltaY ;
    else   mouseDelta = m_coarseDeltaX ;

    int deltaCycles = MAC->getCycles() - m_triggerCycles ;
    int delay = (m_maxTimeoutCycles/2) + mouseDelta*m_coarseK ;
//printf ("delay=%6i\n", delay) ;
    quint8 value ;
    if (deltaCycles > delay) value = 0 ;
    else                     value = 0xff ;
    return value ;    
}


typedef struct mouseRecord {
    quint8 buttons ;
    quint8 x ;
    quint8 y ;
} mouseRecord ;

//  Mouse movement readings from /dev/input/mice
//
//       left/down  |  right/up   
//     <--fast    slow     fast-->
//     $81  ..  $ff | $01  ..  $7f
//    -127  ..   -1 |  +1  .. +127

void Joystick::readMouseData (void)
{
    mouseRecord mouse ;
    int n = read (m_mouseFd, &mouse, sizeof(mouse)) ;
    if ((n==EAGAIN) || (n==EWOULDBLOCK)) return ; 
//putchar('-'); fflush(stdout) ;
    m_buttons[0] = m_buttons[1] = m_buttons[2] = 0 ;
    if (mouse.buttons & 1)  m_buttons[0] = 0xff ;  ;
    if (mouse.buttons & 2)  m_buttons[1] = 0xff ;  ;
    if (mouse.buttons & 4)  m_buttons[2] = 0xff ;  ;

    m_coarseDeltaX = mouse.x ;
    if (mouse.x & 0x80) m_coarseDeltaX = m_coarseDeltaX - 256 ;
    m_coarseDeltaY = mouse.y ;
    if (mouse.y & 0x80) m_coarseDeltaY = m_coarseDeltaY - 256 ;

//printf ("m_coarseDelta: %4i %4i\n", m_coarseDeltaX, m_coarseDeltaY) ;
}


void Joystick::reset (void)
{
    m_triggerCycles = MAC->getCycles() ;
}
