

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
}

