/*****************************************************************************

    File: "win-speaker.cpp"

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


#include "win-speaker.h"
#include "mainwindow.h"
#include "debugging_dumps.h"

#include "roapi.h"


//  Constructor is called in MainWindow::MainWindow

Speaker::Speaker (MainWindow* parent)
{
}


// 'toggleSpeaker' is called by Machine::fetch_sspage (in fetch.cpp) when
//  a fetch is done from address $C030.
//  Its job is to fill a queue, "m_queue", with a number of bytes containing
//  a value (high or low of a square wave), held in "m_previousValue".
//  The number of bytes is determined by the number of CPU cycles elapsed 
//  since the last call.  It then changes the value (hi->lo or lo->hi)
//  held in m_previousValue.
//  

void Speaker::toggleSpeaker (void)
{
    quint64 processorCycles = MAC->getCycles();
    if (m_previousCycles == 0) m_previousCycles = processorCycles;

    if (m_previousCycles > processorCycles) {
        m_previousCycles = processorCycles;
        return;
    }
    int cyclesDifference = processorCycles - m_previousCycles;

    m_qLock.lock();
    if (cyclesDifference > SND_QUEUE_SIZE) {
        m_qHead = m_qTail = 0;
        m_previousCycles = processorCycles;
        m_previousValue = (m_previousValue == 0) ? m_hi : 0 ;
        m_qLock.unlock();
        return;
    }

    int oldTail = m_qTail;
    m_qTail += cyclesDifference / SAMPLE_DELTA ;

    if (m_qTail >= SND_QUEUE_SIZE) {
        for (int i = oldTail; i < SND_QUEUE_SIZE; i++) m_queue[i] = m_previousValue ;
        m_qTail -= SND_QUEUE_SIZE ;
        for (uint i = 0; i < m_qTail; i++) m_queue[i] = m_previousValue ;
        if (m_qTail > m_qHead) m_qHead = m_qTail ;   // Just let queue overruns happen.
    }
    else {
        for (uint i = oldTail; i < m_qTail; i++) m_queue[i] = m_previousValue ;
    }
    m_qLock.unlock();

    m_previousCycles = processorCycles;
    m_previousValue = (m_previousValue == 0) ? m_hi : 0;
}


void Speaker::setVolume (float value)
{
    if (value > 100) value = 100 ;
    if (value < 0)   value = 0 ;

    float volume = 1.28 * (value*value / 100) ;
    m_hi = volume ;
}


void Speaker::submitBuffer (int len)
{
    HRESULT hr ;
    const char* submitBuffer = "SubmitSourceBuffer returned err code 0x%8.8x\n" ;

    m_xAudioBuffer.AudioBytes = len ;
    m_xAudioBuffer.PlayLength = len ;
    hr = m_sourceVoice->SubmitSourceBuffer (&m_xAudioBuffer, nullptr) ;
    if (FAILED(hr)) ::printf (submitBuffer, hr) ; ::fflush(stdout) ;
}



//--------------------------------------------------------------------------------
//               ---  Entry point to the speaker thread  ---
// This thread reads from the head of the sound queue, and writes to the speaker.
// Run by MainWindow::MainWindow
//--------------------------------------------------------------------------------

void Speaker::run (void)
{
/***
    HANDLE tHandle = ::GetCurrentThread() ;
    BOOL ok = ::SetThreadPriority (tHandle,  HIGH_PRIORITY_CLASS) ;
    if (! ok) ::printf ("\n*** Can't set speaker thread priority.  Sound will be poor.\n\n") ; ::fflush(stdout) ;
***/
    HRESULT hr ;
    IXAudio2*               xAudio2 ;
    IXAudio2MasteringVoice* masterVoice ;

    m_xAudioBuffer = { 0 } ;

    WAVEFORMATEX waveFmt {   // Holds a WAVE header in our fictitious RIFF file
        WAVE_FORMAT_PCM,     // wFormatTag
        1,                   // nChannels
        RATE,                // nSamplesPerSec
        RATE,                // nAvgBytesPerSec
        1,                   // nBlockAlign
        8,                   // wBitsPerSample
        0                    // cbSize
    } ;

    m_qHead = 0 ;
    m_qTail = 0 ;
    m_previousCycles = 0 ;
    m_previousValue = 0 ;
    memset (m_queue, 0, SND_QUEUE_SIZE) ;
    const char* noSound = "***  applepi-win won't be able to produce sounds from the Apple II speaker.\n\n" ;

    hr = CoInitializeEx (nullptr, COINIT_MULTITHREADED) ;
    if (FAILED(hr)) goto error1 ;

    hr = XAudio2Create (&xAudio2) ;
    if (FAILED(hr)) goto error2 ;

    hr = xAudio2->CreateMasteringVoice (&masterVoice) ;
    if (FAILED(hr)) goto error3 ;

    hr = xAudio2->CreateSourceVoice (&m_sourceVoice, &waveFmt) ;
    if (FAILED(hr)) goto error4 ;

    hr = m_sourceVoice->Start() ;
    if (FAILED(hr)) goto error5 ;

    const int sleepMS = 10 ;
    quint8 dummyBuffer [(10*sleepMS)+10] ;

    while (true) {
        int len ;
        if (m_qHead < m_qTail) {
            m_qLock.lock() ;
            len = m_qTail - m_qHead ;
            m_xAudioBuffer.pAudioData = (const BYTE*)m_queue + m_qHead ;
            m_qHead = m_qTail ;
            m_qLock.unlock() ;
            submitBuffer (len) ;
        } else if (m_qHead > m_qTail) {
            m_qLock.lock() ;
            len = SND_QUEUE_SIZE - m_qHead ;
            m_xAudioBuffer.pAudioData = (const BYTE*)m_queue + m_qHead ;
            m_qHead = 0 ;
            m_qLock.unlock() ;
            submitBuffer (len) ;
        } else {
            XAUDIO2_VOICE_STATE state ;
            m_sourceVoice->GetState (&state) ;
            if (state.BuffersQueued < 2) {
                int dummyLen = sizeof (dummyBuffer) ;
                memset (dummyBuffer, m_previousValue, dummyLen) ;
                m_xAudioBuffer.pAudioData = (const BYTE*)dummyBuffer ;
                submitBuffer (10*sleepMS) ;
            }
            Sleep (sleepMS-2) ;
        }

    }

error1:
    printf("\n*** 'CoInitializeEx' call failed in Speaker initialization. Error code=%8.8x\n", hr) ;
    puts(noSound) ;
    return ;
error2:
    printf("\n*** 'XAudio2Create' call failed in Speaker initialization. Error code=%8.8x\n", hr) ;
    puts(noSound) ;
    return ;
error3:
    printf("\n*** 'CreateMasteringVoice' call failed in Speaker initialization. Error code=%8.8x\n", hr) ;
    puts(noSound) ;
    return ;
error4:
    printf("\n*** 'CreateSourceVoice' call failed in Speaker initialization. Error code=%8.8x\n", hr) ;
    puts(noSound) ;
    return ;
error5:
    printf("\n*** source voice 'Start' call failed in Speaker initialization. Error code=%8.8x\n", hr) ;
    puts(noSound) ;
    return ;

}



