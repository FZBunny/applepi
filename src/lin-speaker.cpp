
/*****************************************************************************

    File: "speaker.cpp"

    Copyright (C) 2003,2019 Bruce Ward

    This file is part of ApplePi, an Apple2 Emulator.
    (Apple2 is a trademark of Apple, Inc.)

    "ApplePi" is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    "ApplePi" is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You may obtain a copy of the GNU General Public License by writing to:

    The Free Software Foundation, Inc.
    51 Franklin Street, Fifth Floor
    Boston, MA 02110-1335 USA

    Or simply visit: https://www.gnu.org/licenses/gpl-3.0.html

*****************************************************************************/


#ifndef __WXMSW__
  #include <unistd.h>
#endif
//#include <fcntl.h>
#include <sys/types.h>
#include <sys/param.h>
//#include <sys/ioctl.h>
//#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <poll.h>
#include <signal.h>

#include <alsa/asoundlib.h>
#include <alsa/pcm.h>


#include "mainwindow.h"
//#include "speaker.h"
//#include "sound-recal.h"
//#include "sound-step.h"
#include "defs.h"
#include "lin-speaker.h"
#include "debugging_dumps.h"



Speaker::Speaker (MainWindow* parent)
{
    m_parent = parent ;
}



void Speaker::initialize_ALSA (void)
{

    uint rate = RATE ;
    m_soundHandle = NULL;
    int   err ;
    snd_pcm_hw_params_t   *hw_params ;
    char*   deviceName = (char*)"default" ;

    m_pcmLen = 0 ;
    m_buffer_size = 1024 ;
    snd_pcm_uframes_t  period_size = 128 ;

    if ((err = snd_pcm_open (&m_soundHandle, deviceName, SND_PCM_STREAM_PLAYBACK, 0)))   goto error1 ;
    if ((err = snd_pcm_hw_params_malloc (&hw_params)))   goto error2 ;
    if ((err = snd_pcm_hw_params_any (m_soundHandle, hw_params) < 0))   goto error3 ;
    if ((err = snd_pcm_hw_params_set_access (m_soundHandle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)))   goto error4 ;
    if ((err = snd_pcm_hw_params_set_format (m_soundHandle, hw_params, SND_PCM_FORMAT_U8)))   goto error5 ;
    if ((err = snd_pcm_hw_params_set_rate_near (m_soundHandle, hw_params, &rate, NULL)))   goto error6 ;
    if ((err = snd_pcm_hw_params_set_channels (m_soundHandle, hw_params, 1)))   goto error7 ;
    if ((err = snd_pcm_hw_params_set_buffer_size_near (m_soundHandle, hw_params, &m_buffer_size)))   goto error8 ;
    if ((err = snd_pcm_hw_params_set_period_size_near (m_soundHandle, hw_params, &period_size, NULL)))   goto error9 ;
    if ((err = snd_pcm_hw_params (m_soundHandle, hw_params)))  goto error10 ;
    snd_pcm_hw_params_free (hw_params) ;

    snd_pcm_sw_params_t *sw_params ;
    if ((err = snd_pcm_sw_params_malloc (&sw_params)))   goto error11 ;
    if ((err = snd_pcm_sw_params_current (m_soundHandle, sw_params)))   goto error12 ;
    if ((err = snd_pcm_sw_params_set_start_threshold (m_soundHandle, sw_params, m_buffer_size-period_size)))   goto error13 ;
    if ((err = snd_pcm_sw_params_set_avail_min (m_soundHandle, sw_params, period_size)))   goto error14 ;
    if ((err = snd_pcm_sw_params(m_soundHandle, sw_params)))   goto error15  ;
    snd_pcm_sw_params_free (sw_params) ;

    if ((err = snd_pcm_prepare (m_soundHandle)))   goto error16  ;

    return ;

//   ------------------------------------------------------------------------------------------------------

error1:
    fprintf (stderr, "SOUND ERROR %i; Can't open audio device \"%s\" (%s)\n", err, deviceName, snd_strerror (err)) ;
    m_soundHandle = NULL ;
    return ;
error2:
    fprintf (stderr, "SOUND ERROR %i; Out of memory: %s\n", err, snd_strerror (err)) ;
    return ;
error3:
    fprintf (stderr, "SOUND ERROR %i; Broken configuration: no configurations available: %s\n", err, snd_strerror(err)) ;
    return ;
error4:
    fprintf (stderr, "SOUND ERROR %i; Access type not available: %s\n", err, snd_strerror(err)) ;
    return ;
error5:
    fprintf (stderr, "SOUND ERROR %i; Sample format not available: %s\n", err, snd_strerror(err)) ;
    return ;
error6:
    fprintf (stderr, "SOUND ERROR %i; Rate %uHz not available: %s\n", err, rate, snd_strerror(err)) ;
    return ;
error7:
    fprintf (stderr, "SOUND ERROR %i; Channels count (1) not available: %s\n", err, snd_strerror(err)) ;
    return ;
error8:
    fprintf (stderr, "SOUND ERROR %i; Buffer size (%lui bytes) not available %s\n", err, m_buffer_size, snd_strerror(err)) ;
    return ;
error9:
    fprintf (stderr, "SOUND ERROR %i; Period size (%lui) not available: %s\n", err, period_size, snd_strerror(err)) ;
    return ;
error10:
    fprintf (stderr, "SOUND ERROR %i; Unable to set hardware params: %s\n", err, snd_strerror (err)) ;
    return ;
error11:
    fprintf (stderr, "SOUND ERROR %i; Out of memory: %s\n", err, snd_strerror (err)) ;
    return ;
error12:
    fprintf (stderr, "SOUND ERROR %i; Unable to determine current software params: %s\n", err, snd_strerror (err)) ;
    return ;
error13:
    fprintf (stderr, "SOUND ERROR %i; Unable to set start threshold mode: %s\n", err, snd_strerror (err)) ;
    return ;
error14:
    fprintf (stderr, "SOUND ERROR %i; Call to \"snd_pcm_sw_params_set_avail_min\" failed: %s\n", err, snd_strerror (err)) ;
    return ;
error15:
    fprintf (stderr, "SOUND ERROR %i; Unable to set software params: %s\n", err, snd_strerror (err)) ;
    return ;
error16:
    fprintf (stderr, "SOUND ERROR %i; Call to \"snd_pcm_prepare\" failed: %s\n", err, snd_strerror (err)) ;
    return ;

}



// Write sound data to the tail of the queue.
// This function is called in Machine::fetch_sspage when a "fetch" is done from address $C030

void Speaker::toggleSpeaker (void)
{
//putchar('.') ; fflush(stdout) ;
    quint64 processorCycles = MAC->getCycles() ;
    if (m_previousCycles == 0) m_previousCycles = processorCycles ;

    if (m_previousCycles > processorCycles) {
        m_previousCycles = processorCycles ;
        return ;
    }
    int cyclesDifference = processorCycles - m_previousCycles ;

//putchar('1') ; fflush(stdout) ;
    m_qLock.lock() ;
//putchar('2') ; fflush(stdout) ;
    if (cyclesDifference > SND_QUEUE_SIZE) {
        m_qHead = m_qTail = 0 ;
        m_previousCycles = processorCycles ;
        m_previousValue = (m_previousValue==m_lo) ? m_hi : m_lo ;
        m_qLock.unlock() ;
        return ;
    }

    int oldTail = m_qTail ;
    m_qTail += cyclesDifference / SAMPLE_DELTA ;

    if (m_qTail >= SND_QUEUE_SIZE) {
        for (int i=oldTail; i<SND_QUEUE_SIZE; i++) m_queue[i] = m_previousValue ;
        m_qTail -= SND_QUEUE_SIZE ;
        for (uint i=0; i<m_qTail; i++) m_queue[i] = m_previousValue ;
        if (m_qTail > m_qHead) m_qHead = m_qTail ;   // Just let queue overruns happen.
    } else {
        for (uint i=oldTail; i<m_qTail; i++) m_queue[i] = m_previousValue ;
    }
//putchar('3') ; fflush(stdout) ;
    m_qLock.unlock() ;
//putchar('4') ; fflush(stdout) ;

    m_previousCycles = processorCycles ;
    m_previousValue = (m_previousValue==m_lo) ? m_hi : m_lo ;

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

/***
void Speaker::playRecal (void)
{
    m_pcmLen = recal_pcm_len ;
    m_pcmSound = recal_pcm ;
}


void Speaker::playStep (void)
{
    m_pcmLen = step_pcm_len ;
    m_pcmSound = step_pcm ;
}
***/


// ---  Entry point to the speaker thread  ---
// This thread reads from the head of the sound queue, and writes to the speaker.


void Speaker::run (void)
{

    char dummyBuffer[100] ;

    initialize_ALSA() ;

    for (int i=0; i<SND_QUEUE_SIZE; i++) m_queue[i] = MID ;

    m_qHead = 0 ;
    m_qTail = 0 ;
    m_previousCycles = 0 ;
    m_previousValue = MID ;

    while (true) {
        int len, oldHead ;
        if (m_qHead < m_qTail) {
//putchar('a') ; fflush(stdout) ;
            m_qLock.lock() ;
//putchar('b') ; fflush(stdout) ;
            oldHead = m_qHead ;
            len = m_qTail - oldHead ;
            m_qHead = m_qTail ;
//putchar('c') ; fflush(stdout) ;
            m_qLock.unlock() ;
//putchar('d') ; fflush(stdout) ;
            snd_pcm_writei (m_soundHandle, m_queue+oldHead, len) ;
//putchar('1') ; fflush(stdout) ;
        } else if (m_qHead > m_qTail) {
            m_qLock.lock() ;
            len = SND_QUEUE_SIZE - m_qHead ;
            m_qHead = 0 ;
            m_qLock.unlock() ;
putchar('d') ; fflush(stdout) ;
            snd_pcm_writei (m_soundHandle, m_queue+m_qHead, len) ;
//putchar('2') ; fflush(stdout) ;
        } else {
            memset (dummyBuffer, m_previousValue, sizeof(dummyBuffer)) ;
            snd_pcm_writei (m_soundHandle, dummyBuffer, sizeof(dummyBuffer)) ;
//putchar('3') ; fflush(stdout) ;
        }
                        // ( About the sleep-&-repeat hackery... poll calls refuse to work,)
        usleep (20) ;   // ( and ALSA callbacks are not implemented on some Linux distros. )

    }
}
