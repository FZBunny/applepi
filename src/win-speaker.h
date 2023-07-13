/*****************************************************************************

    File: "win-speaker.h"

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



#ifndef WIN_SPEAKER_H
#define WIN_SPEAKER_H


#include "xaudio2.h"

#include "mainwindow.h"

class MainWindow ;

class Speaker : public QThread
{
public:

    Speaker (MainWindow* parent) ;
    ~Speaker(void) {} ;

    void setVolume (float value) ;
    void toggleSpeaker (void) ;   // Called in Machine::fetch_sspage upon fetch from $C030

private:

    void submitBuffer (int len) ;
    void run(void) ;

    MainWindow*  m_parent ;

    static const int SND_QUEUE_SIZE = 10000;        // Sound queue (m_queue) size, bytes
    static const int RATE = 10000;                  // Max. samples/second
    static const quint64 SAMPLE_DELTA = 1E6 / RATE; // No. of 6502 cycles between samples

    IXAudio2SourceVoice*  m_sourceVoice ;
    XAUDIO2_BUFFER        m_xAudioBuffer ;

    QMutex   m_qLock;
    qint8    m_queue[SND_QUEUE_SIZE];
    uint     m_qHead;
    uint     m_qTail;
    uint     m_previousValue;
    quint64  m_previousCycles;
    HANDLE   m_timerHandle ;
    uint     m_hi;

} ;

#endif

