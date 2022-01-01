

#ifndef DISKSOUNDS_H
#define DISKSOUNDS_H

#include "mainwindow.h"
#include "defs.h"


class DiskSounds
{
public:

    static const quint16 DISK144K = 1 ;
    static const quint16 DISK800K = 2 ;

    DiskSounds (void) ;
    void play (quint16 soundNumber) ;

private:

} ;

#endif
