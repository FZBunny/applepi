
#ifndef VOLUMEDIAL_H
#define VOLUMEDIAL_H


#include <QDial>

#include "mainwindow.h"


class VolumeDial : public QDial
{
public:

    VolumeDial (MainWindow* parent, int width, int height) ;
    ~VolumeDial() {} ;

protected:

    void sliderChange (QAbstractSlider::SliderChange) ;

private:

    MainWindow* m_parent ;
    uint        m_value ;

} ;


#endif
