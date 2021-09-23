
/*****************************************************************************

    File: "tape.cpp"

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

//-----------------------------------------------------------
// For documentation on the format of Apple II casette tapes,
// see:   https://support.apple.com/kb/TA40730
// and:   https://support.apple.com/kb/TA40737
//-----------------------------------------------------------


//   *** Note:  This class currently supports the Apple II MONITOR format, ONLY ***


#ifndef __WXMSW__
#  include <unistd.h>
#endif
#include <sys/types.h> // XXXXXXXXXXXXXXXXXXXXXXXXXXX


#include "machine.h"
#include "tape.h"
#include "debugging_dumps.h"


Tape::Tape (Machine* parent)
{
    m_parent = parent ;

    QString key ;
    key.Printf ("tape_path") ;
    CFG->Get (key, &m_configuredPath) ;

    QFileInfo test ;
    test.Assign (m_configuredPath) ;
    if ( ! test.FileExists()) return ;  // "IsDir()" call doesn't seem to work...
    if ( ! test.IsFileReadable()) return ;

    for (int i=0; i<8; i++) m_bitMasks[i] = 0x80 >> i ;

    m_writeable = test.IsFileWritable() ;
    this->open (m_configuredPath, m_writeable) ;
}



void Tape::initialize (void)
{
    m_inputState = 0 ;
    m_outputState = 1 ;
    m_firstOutputCall = true ;
    m_syncState  = 0 ;
    m_inputHeaderCycleCount = 0 ;
    m_bitNumber = 8;
    m_checksum = 0xff ;
    m_previousBit7Value = 0 ;

}



int Tape::open (QString &path, bool writeable)
{
    m_configuredPath = path ;
    m_writeable = writeable ;

    QFile::OpenMode  mode ;
    if (writeable) mode = QFile::read_write ;
    else           mode = QFile::read ;
    int ok = m_file.Open (m_configuredPath, mode) ;
//printf ("Tape::open    m_configuredPath = '%s', mode=%i  ok=%i\n", m_configuredPath, mode, ok) ;

    if (ok)  m_tapeInDriveFlag = true ;
    else     m_tapeInDriveFlag = false ;

    initialize() ;
    return ok ;
}



void Tape::close (void)
{
    CFG->Get ("tape_path", &m_configuredPath) ;   //     WTF is this all about ?  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

    m_fileName.Assign (m_configuredPath) ;
    m_configuredPath = m_fileName.GetPath() ;
    CFG->Set ("tape_path", m_configuredPath) ;

    m_file.Close() ;

}



int Tape::create (QString &path)
{
 //   int          i, err ;
//    struct stat  status ;

    if (m_file.IsOpened()) {
        errno = EBUSY ;
        return errno ;
    }

    m_writeable = true ;
    m_tapeInDriveFlag = false ;

    bool ok = m_file.Create (path, true) ;
    if ( ! ok) {
        return m_file.GetLastError() ;
    }

	m_configuredPath = path ;
    m_tapeInDriveFlag = true ;

    initialize() ;
    return 0 ;
}



void Tape::setWriteableFlag (bool flag)
{
    m_writeable = flag ;
}



//  Fake the 770 Hz header tone on a tape.
//
//  "readTapeInput" calls this continually until 3 seconds of 770 Hz have been read.
//  (We supply only 1 second of header, which is enough to keep the ROM code happy.)

quint8 Tape::readTapeHeader (void)
{
    quint64 nCycles = MAC->getCycles() ;
    quint64 delta   = nCycles - m_previousCycles ;

    if (delta > 1e6) {
        delta = 0 ;                  // delta will be HUGE the first time this is called
        m_previousCycles = nCycles ; // for a tape, so ignore it this time through.
        return 0 ;
    }

    if (delta>648) {                 // feed a 770 Hz. tone
        m_previousCycles = nCycles ;
        m_previousBit7Value ^= 0x80 ;
        if (m_previousBit7Value == 0) {
            m_inputHeaderCycleCount++ ;
        }
        toggleSpeaker() ;
    }

    return m_previousBit7Value ;
}



//  Fake the one-cyle 'sync' marker on a tape.
//
//  "readTapeInput" calls this once after the header has been read.
//  This cycle consists of a half-cycle of 200 uSec, followed by
//  by a half-cycle of 250 uSec.

quint8 Tape::readSync (void)
{
    quint64 nCycles = MAC->getCycles() ;
    quint64 delta   = nCycles - m_previousCycles ;
//printf ("readSync:  m_syncState=%i  delta=%lli\n", m_syncState, delta) ;
    if (m_syncState == 0) {
        if (delta > 195) {
            m_syncState++ ;
            m_previousBit7Value ^= 0x80 ;
            m_previousCycles = nCycles ;
        }
    } else {
//printf ("readSync:  m_syncState=%i  delta=%lli\n", m_syncState, delta) ;
        if (delta > 245) {
            m_previousBit7Value ^= 0x80 ;
            m_inputHeaderCycleCount++ ;
        }
    }

    return m_previousBit7Value ;
}



//  Read from a fictional cassette tape.
//
//  This is called repeatedly when the ROM
//  code fetches from soft-switch location $C060.
//  Note that this is called for each -bit- on the "tape".
//  See "Machine::fetch_sspage" in fetch.cpp

quint8 Tape::readTapeInput (void)
{
    if (m_tapeInDriveFlag == false) return 0 ;

    if (m_inputHeaderCycleCount < 770) { // (1 second of a 770Hz tone)
        return readTapeHeader() ;
    }

    if (m_inputHeaderCycleCount < 771) {
        return readSync() ;
    }

    quint8 bit = 0 ;
    if (m_bitNumber > 7) {
        int n = m_file.Read (&m_byte, 1) ;
        if (n == wxInvalidOffset) {
            m_byte = m_checksum ;
printf ("\n *** checksum = %2.2x ***\n", m_byte) ;
        } else {
            m_checksum ^= m_byte ;
//printf ("%2.2x %2.2x\n", m_byte, m_checksum) ;
        }
printf ("*** byte=0x%2.2x\n", m_byte) ;
        m_bitNumber = 0 ;
        bit = m_byte & m_bitMasks[m_bitNumber] ;
    }

    quint64 nCycles = MAC->getCycles() ;
    quint64 delta   = nCycles - m_previousCycles ;
//printf ("reading data: m_inputState=%i m_bitNumber=%i bit=0x%2.2x  delta = %lli\n", m_inputState, m_bitNumber, bit, delta) ;
    bool toggle = false ;
    if (bit) {
        if (delta >= m_cyclesFor_1) toggle = true ;
    } else {
        if (delta >= m_cyclesFor_0) toggle = true;
    }

    if (toggle) {
        m_previousCycles = nCycles ;
        m_previousBit7Value ^= 0x80 ;
        if (++m_inputState > 1) {
            m_inputState = 0 ;
            m_bitNumber++ ;
            if (m_bitNumber < 8) bit = m_byte & m_bitMasks[m_bitNumber] ;
        }
        toggleSpeaker() ;
    }

    return m_previousBit7Value ;
}



//  Write to cassette tape.  This is called twice per audio cycle
//  when the ROM code fetches from soft-switch location $C020.
//  See "Machine::fetch_sspage" in fetch.cpp

// Toggle the output (0->1 or 1->0) to our fictional cassette drive,
// actually small disk files with a ".tape' extension.
// One full audio cycle is used to represent a single bit.
// When in state 2 (i.e., when the actual data is being sent), a full
// byte is written to disk every eight audio cycles.

void Tape::toggleTapeOutput (quint64 processorCycles)
{
    if (m_tapeInDriveFlag == false) return ;
    if (m_writeable == false)       return ;

    if (m_firstOutputCall) {
        m_firstOutputCall = false ;
        m_file.Seek(0) ;
    }

 //   bool header = false ;
    int bit = -1 ;
    quint64 delta = processorCycles - m_previousCycles ;
    m_previousCycles = processorCycles ;

    toggleSpeaker() ;

    if ((delta>650) && (delta<660)) {                          // 770 Hz header
        m_outputState = 0 ;
        return ;    //  Do nothing for this.  We only write actual data to the file.
    }               //  (Function 'readTapeHeader' fakes a header & sync to keep the ROM code happy.)

    if ((m_outputState==0) && (delta>190) && (delta<210)) {    // 1st half-cycle of sync bit (200 uSec)
        m_outputState = 1 ;
        return ;
    }

    if ((m_outputState==1) && (delta>245) && (delta<255)) {    // 2nd half-cycle of sync bit (250 uSec)
        m_outputState = 2 ;
        m_fullCycle = true ;
        m_bitNumber = 0 ;
        m_file.Seek (0) ;
        m_byte = 0 ;
        return ;
    }

    if (m_outputState==2) {                                    // data  (One cycle of 2kHz for '0' bit;  One cycle of 1kHz for '1' bit)
        if ((delta>245) && (delta<255)) {
            bit = 0 ;
        } else if ((delta>495) && (delta<505)) {
            bit = 1 ;
        } else {                                               // A delta of around 262 uSec means end-of-data; we're done here.
            m_outputState = 0 ;
            return ;
        }
        m_fullCycle = ! m_fullCycle ;
        if (m_fullCycle) {
            if (shiftIn(bit)) {
printf ("BYTE = $%2.2x\n\n", m_byte) ;
                m_file.Write (&m_byte, 1) ;
                m_byte = 0 ;
            }
        }
    }

}


// "Shift" a bit into the accumulating byte.
//  Bits are received most-significant bit first.
//  Returns 'true' if all 8 bits have been accumulated
//  else returns 'false'.

bool Tape::shiftIn (int bit)
{
    bool done = false ;

    if (bit) m_byte |= m_bitMasks[m_bitNumber] ;
printf ("shiftIn:  bit= %i  m_bitMasks[%i] = $%2.2x\n", bit, m_bitNumber, m_bitMasks[m_bitNumber]) ;
    if (++m_bitNumber > 7) {
        m_bitNumber = 0 ;
        done = true ;
    }

    return done ;
}


void Tape::toggleSpeaker (void)
{
    MAC->m_parent->speaker()->toggleSpeaker () ;
}


