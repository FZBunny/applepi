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

#include "xaudio2.h"
#include "roapi.h"


//  Constructor is called in MainWindow::MainWindow

Speaker::Speaker (MainWindow* parent)
{
}


//  Make a square wave
// 'toggleSpeaker' is called by Machine::fetch_sspage (in fetch.cpp) when
//  a fetch is done from address $C030.
//  Its job is to fill a buffer, "m_tmpBuffer", with a number of bytes containing
//  a value (high or low of a square wave), held in "m_previousValue".
//  The number of bytes is determined by the number of CPU cycles elapsed 
//  since the last call.  It then changes the value (hi->lo or lo->hi)
//  held in m_previousValue.
//  

void Speaker::toggleSpeaker (void)
{
    quint64 processorCycles = MAC->getCycles() ;
    if (m_previousCycles == 0) m_previousCycles = processorCycles ;

    if (m_previousCycles > processorCycles) {
        m_previousCycles = processorCycles ;
        return ;
    }
    int cyclesDifference = processorCycles - m_previousCycles ;

    int len = cyclesDifference / SAMPLE_DELTA ;
    if (len > BUFFER_LEN) {
        m_bufferPtr = 0 ;
        m_previousValue = (m_previousValue == m_lo) ? m_hi : m_lo ;
        m_previousCycles = processorCycles ;
        return ;
    }

    int newIndex = m_bufferPtr + len ;
    if (newIndex >= BUFFER_LEN) newIndex = BUFFER_LEN - 1 ;

    m_bufferLock.lock() ;
    for (int i= m_bufferPtr; i<newIndex; i++) m_tmpBuffer[i] = m_previousValue ;
    m_bufferPtr = newIndex;
    m_bufferLock.unlock() ;

    m_previousCycles = processorCycles ;
    m_previousValue = (m_previousValue == m_lo) ? m_hi : m_lo ;
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


//--------------------------------------------------------------------------------
//               ---  Entry point to the speaker thread  ---
// This thread reads from the head of the sound queue, and writes to the speaker.
// Run by MainWindow::MainWindow
//--------------------------------------------------------------------------------

void Speaker::run (void)
{
    HRESULT hr ;
    IXAudio2*               xAudio2 ;
    IXAudio2MasteringVoice* masterVoice ;
    IXAudio2SourceVoice*    sourceVoice ;
//    char dummyBuffer[16] ;


// waveFmt holds a WAVE header in our fictitious RIFF file
    WAVEFORMATEX waveFmt;              // #1: These two variables must be kept together in this
    quint8       pcmData[BUFFER_LEN];  // #2: order. (i.e, wave structure followed by PCM data.)

    waveFmt.wFormatTag = WAVE_FORMAT_PCM; // See https://www-mmsp.ece.mcgill.ca/Documents/AudioFormats/WAVE/WAVE.html
    waveFmt.nChannels = 1;                // and https://learn.microsoft.com/en-us/windows/win32/xaudio2/xaudio2-apis-portal
    waveFmt.nSamplesPerSec = RATE;
    waveFmt.nAvgBytesPerSec = RATE;
    waveFmt.nBlockAlign = 1;
    waveFmt.wBitsPerSample = 8;
    waveFmt.cbSize = 0;
    

    XAUDIO2_BUFFER  xAudio2Buffer;
    xAudio2Buffer.Flags = 0;
    //    xAudio2Buffer.Flags = XAUDIO2_END_OF_STREAM;
    xAudio2Buffer.pAudioData = (const BYTE*)&waveFmt;
    xAudio2Buffer.AudioBytes = 0;
    xAudio2Buffer.PlayBegin = 0;
    xAudio2Buffer.PlayLength = 0;
    xAudio2Buffer.LoopBegin = 0;
    xAudio2Buffer.LoopLength = 0;
    xAudio2Buffer.LoopCount = 0;
    xAudio2Buffer.pContext = nullptr;

    m_previousCycles = 0 ;
    m_previousValue = MID ;
    m_bufferPtr = 0 ;
    memset (pcmData, MID, sizeof(pcmData)) ;
    const char* noSound = "***  applepi-win won't be able to produce sounds from the Apple II speaker.\n\n" ;

    hr = CoInitializeEx (nullptr, COINIT_MULTITHREADED) ;
    if (FAILED(hr)) goto error1 ;

    hr = XAudio2Create (&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR); 
//    hr = XAudio2Create (&xAudio2, 0, COINIT_APARTMENTTHREADED) ;
    if (FAILED(hr)) goto error2 ;

    hr = xAudio2->CreateMasteringVoice (&masterVoice) ;
    if (FAILED(hr)) goto error3 ;

    hr = xAudio2->CreateSourceVoice (&sourceVoice, (WAVEFORMATEX*)&waveFmt) ;
    if (FAILED(hr)) goto error4 ;

    hr = sourceVoice->Start() ;
    if (FAILED(hr)) goto error5 ;

    while (true) {
        if (m_bufferPtr) {
            m_bufferLock.lock() ;
            memmove (pcmData, m_tmpBuffer, m_bufferPtr) ;
int len = m_bufferPtr;
            xAudio2Buffer.AudioBytes = m_bufferPtr ;
            xAudio2Buffer.PlayLength = m_bufferPtr ;
            m_bufferPtr = 0 ;
            m_bufferLock.unlock() ;
//xdump (pcmData, len, 0) ;
            hr = sourceVoice->SubmitSourceBuffer(&xAudio2Buffer, nullptr) ;
            if (FAILED(hr)) printf ("SubmitSourceBuffer returned err code 0x%8.8x\n", hr) ;
        }

        Sleep (100) ;
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



