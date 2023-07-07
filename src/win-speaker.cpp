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

#include "mmdeviceapi.h"
#include "audioclient.h"  // XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX FUCK! 
#include "roapi.h"


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

    m_qLock.lock() ;
    if (cyclesDifference > SND_QUEUE_SIZE) {
        m_qHead = m_qTail = 0 ;
        m_previousCycles = processorCycles ;
        m_previousValue = (m_previousValue == 0) ? m_hi : 0 ;
        m_qLock.unlock() ;
        return ;
    }

    int oldTail = m_qTail ;
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
    m_qLock.unlock() ;

    m_previousCycles = processorCycles ;
    m_previousValue = (m_previousValue == 0) ? m_hi : 0 ;

}


void Speaker::setVolume (float value)
{
    if (value > 100) value = 100 ;
    if (value < 0)   value = 0 ;

    float volume = 1.28 * (value*value / 100) ;

    m_hi = volume ;

//printf ("MID=%i  value=%f  volume=%f m_hi=%i  m_lo=%i\n", MID, value, volume, m_hi, m_lo) ;
}


//--------------------------------------------------------------------------------
//               ---  Entry point to the speaker thread  ---
// This thread reads from the head of the sound queue, and writes to the speaker.
// Run by MainWindow::MainWindow
//--------------------------------------------------------------------------------

void Speaker::run (void)
{
    const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator) ;
    const IID IID_IMMDeviceEnumerator =    __uuidof(IMMDeviceEnumerator) ;
    const IID IID_IAudioClient =           __uuidof(IAudioClient) ;
    const IID IID_IAudioRenderClient =     __uuidof(IAudioRenderClient) ;
    const char* noSound = "***  applepi-win won't be able to produce sounds from the Apple II speaker.\n";

    HRESULT hr;
    WAVEFORMATEX* waveFmt ;
    IMMDevice* device ;
    IAudioClient* audioClient ;
    IMMDeviceEnumerator* enumerator ;

    char    dummyBuffer[16] ;
    UINT32  bufferFrameCount ;
    m_previousCycles = 0 ;
    m_previousValue = 0;
    m_qHead = 0 ;
    m_qTail = 0 ;

    hr = CoInitialize (NULL) ;
    hr = CoCreateInstance (CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void**)&enumerator) ;
    if (FAILED(hr)) goto error1 ;

    hr = enumerator->GetDefaultAudioEndpoint (eRender, eConsole, &device) ;
    if (FAILED(hr)) goto error2 ;

    hr = device->Activate (IID_IAudioClient, CLSCTX_ALL, NULL, (void**)&audioClient) ;
    if (FAILED(hr)) goto error3 ;

    hr = audioClient->GetMixFormat (&waveFmt) ;
    if (FAILED(hr)) goto error4 ;
    waveFmt->wFormatTag = WAVE_FORMAT_PCM ;  // See https://www-mmsp.ece.mcgill.ca/Documents/AudioFormats/WAVE/WAVE.html
    waveFmt->nChannels = 1 ;
    waveFmt->nSamplesPerSec = RATE ;
    waveFmt->nAvgBytesPerSec = RATE ;
    waveFmt->nBlockAlign = 1 ;
    waveFmt->wBitsPerSample = 8 ;
    waveFmt->cbSize = 0 ;

// "reference times" are 100 nano-second chunks, or 10,000 chunks/mSec; we want to request 20 millisecond buffers.
    REFERENCE_TIME requestBufferDuration = 20 * 1E4 ;

    hr = audioClient->Initialize (AUDCLNT_SHAREMODE_SHARED, 0, requestBufferDuration, 0, waveFmt, NULL) ;
    if (FAILED(hr)) goto error5 ;

    hr = audioClient->GetBufferSize (&bufferFrameCount) ;
    if (FAILED(hr)) goto error6 ;
printf("bufferFrameCount = %d\n", bufferFrameCount) ;
exit(0) ;

    while (true) {
        int len, oldHead;
        if (m_qHead < m_qTail) {
            m_qLock.lock() ;
            oldHead = m_qHead ;
            len = m_qTail - oldHead ;
            m_qHead = m_qTail ;
            m_qLock.unlock() ;
//            snd_pcm_writei(m_soundHandle, m_queue + oldHead, len);
        }
        else if (m_qHead > m_qTail) {
            m_qLock.lock() ;
            len = SND_QUEUE_SIZE - m_qHead ;
            m_qHead = 0 ;
            m_qLock.unlock() ;
//            snd_pcm_writei(m_soundHandle, m_queue + m_qHead, len);
        }
        else {
//            memset(dummyBuffer, m_previousValue, sizeof(dummyBuffer));
//            snd_pcm_writei(m_soundHandle, dummyBuffer, sizeof(dummyBuffer));
        }
        // ( About the sleep-&-repeat hackery... poll calls refuse to work,)
        Sleep (20) ;   // ( and ALSA callbacks are not implemented on some Linux distros. )

    }

error1:
    printf ("\n*** 'CoCreateInstance' call failed in Speaker initialization. Error code=%8.8x\n", hr) ;
    printf ("%s\n", noSound) ;
    return ;
error2:
    printf ("\n*** 'GetDefaultAudioEndpoint' call failed in Speaker initialization. Error code=%8.8x\n", hr) ;
    printf ("%s\n", noSound) ;
    return ;
error3:
    printf ("\n*** 'Activate' call failed in Speaker initialization. Error code=%8.8x\n", hr) ;
    printf ("%s\n", noSound) ;
    return ;
error4:
    printf ("\n*** 'GetMixFormat' call failed in Speaker initialization. Error code=%8.8x\n", hr) ;
    printf ("%s\n", noSound) ;
    return ;
error5:
    printf ("\n*** 'Initialize' call failed in Speaker initialization. Error code=%8.8x\n", hr);
    printf ("%s\n", noSound);
    return ;
error6:
    printf ("\n*** 'GetBufferSize' call failed in Speaker initialization. Error code=%8.8x\n", hr);
    printf ("%s\n", noSound);
    return ;
error7:
    printf ("\n*** '' call failed in Speaker initialization. Error code=%8.8x\n", hr);
    printf ("%s\n", noSound);
    return ;
error8:
    printf ("\n*** '' call failed in Speaker initialization. Error code=%8.8x\n", hr);
    printf ("%s\n", noSound);
    return ;
error9:
    printf ("\n*** '' call failed in Speaker initialization. Error code=%8.8x\n", hr);
    printf ("%s\n", noSound);
    return ;
error10:
    printf ("\n*** '' call failed in Speaker initialization. Error code=%8.8x\n", hr);
    printf ("%s\n", noSound);
    return ;
}

