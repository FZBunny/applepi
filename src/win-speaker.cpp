

#include "win-speaker.h"
#include "mainwindow.h"


Speaker::Speaker (MainWindow* parent)
{
    m_parent = parent ;
}

void Speaker::toggleSpeaker (quint64 processorCycles)
{

    
}

void* Speaker::Entry (void)
{
    for (;;) {
        Sleep(1) ;
    }
}


void Speaker::setVolume (float value)
{
    if (value > 100) value = 100 ;
    if (value < 0)   value = 0 ;

    float volume = 1.28 * (value*value / 100) ;

    m_hi = MID + volume ;
    m_lo = MID - volume ;
//printf ("MID=%i  value=%f  volume=%f m_hi=%i  m_lo=%i\n", MID, value, volume, m_hi, m_lo) ;
}
