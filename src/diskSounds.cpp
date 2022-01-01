

#include <QSound>
#include <unistd.h>

#include "diskIISounds.h"
#include "debugging_dumps.h"


DiskSounds::DiskSounds (void)
{


}


void DiskSounds::play (quint16 soundNumber)
{

        switch (soundNumber) {
            case DISK144K:
                break ;
            case DISK800K:
                break ;
            default:
                break ;
        }
}


void DiskSounds::run ()
{
    while (true) {
        quint16 soundNumber ;
        read (m_pipe[0], &soundNumber, sizeof(quint16)) ;
    }
}

//printf("DiskSounds::play DISK144K\n") ;m_soundNumber = 0 ;
//                QSound::play (":/sounds/step1.wav") ;
