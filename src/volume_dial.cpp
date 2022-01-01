/*****************************************************************************

    File: "volume_dial.cpp"

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


#include <QAudioDeviceInfo>
#include <QAudioOutput>

#include "config.h"
#include "volume_dial.h"


VolumeDial::VolumeDial (MainWindow* parent, int width, int height) : QDial (parent)
{
    m_parent = parent;
    resize (width, height) ;
    setNotchesVisible (true) ;
    setWrapping (false) ;
    CFG->Get ("speaker_volume", &m_value) ;
    setValue (m_value) ;
    this->show() ;
}


void VolumeDial::sliderChange (QAbstractSlider::SliderChange) 
{
    int newValue = value() ;

    if ((m_value==99) && (newValue<50)) newValue = 99 ;
    if ((m_value==0)  && (newValue>50)) newValue = 0 ;
    m_value = newValue ;

    setValue (newValue) ;
    m_parent->setVolume (newValue) ;
    CFG->Set ("speaker_volume", (uint)m_value) ;
    repaint () ;

//    QAudioDeviceInfo info (QAudioDeviceInfo::defaultOutputDevice()) ;
}


void VolumeDial::keyPressEvent (QKeyEvent *e)  // Ignore all keyboard events
{
    e->ignore() ;
}


void VolumeDial::keyReleaseEvent (QKeyEvent *e) // ditto
{
    e->ignore() ;
}

