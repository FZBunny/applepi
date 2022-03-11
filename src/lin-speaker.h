/*****************************************************************************

    File: "lin-speaker.h"

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



#ifndef SPEAKER_H
#define SPEAKER_H


#include <sys/param.h>
#include <sys/types.h>

#include <alsa/asoundlib.h>
#include <alsa/pcm.h>


#include "mainwindow.h"
#include "defs.h"

class MainWindow ;



class Speaker : public QThread
{

public:

    Speaker (MainWindow* parent) ;
    ~Speaker(void) {} ;

    void toggleSpeaker (void) ;   // Called in Machine::fetch_sspage upon fetch from $C030
    void setVolume (float volume) ;

    void playRecal (void) ;
    void playStep  (void) ;

private:

    void run (void) ;
    void initialize_ALSA (void) ;
    int  writeQueue (int len) ;

    static const int SND_QUEUE_SIZE    = 10000 ;        // Sound queue size, bytes
    static const int RATE              = 10000 ;        // Max. samples/second
    static const quint64 SAMPLE_DELTA  = 1E6 / RATE ;   // No. of 6502 cycles between samples
    static const int MID               = 0x80 ;
    static const int ERROR_SLEEP       = 10 ;           // Sleep this many seconds on snd_pcm_writei error.

 //   uint  m_volume ;
    uint  m_lo ;
    uint  m_hi ;

    snd_pcm_t  *m_soundHandle ;
    int         m_nDescriptors ;
    int         m_nFilledDescriptors ;

    qint32   m_pcmLen ;
    quint8*  m_pcmSound ;

    snd_pcm_uframes_t  m_buffer_size ;

    QMutex   m_qLock ;
    qint8    m_queue [SND_QUEUE_SIZE] ;
    uint     m_qHead ;
    uint     m_qTail ;
    uint     m_previousValue ;
    quint64  m_previousCycles ;

    MainWindow* m_parent ;

} ;


#endif // SPEAKER_H
