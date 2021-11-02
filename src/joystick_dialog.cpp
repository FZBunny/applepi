

#include "joystick_dialog.h"


joystickDialog::joystickDialog (MainWindow* parent)
{
    m_parent = parent ;

    int width = 360 ;
    int height = 360 ;
    int x, y ;
    CENTER_DIALOG

    m_list = new QListWidget (this) ;
    m_list->move (10,10) ;
    m_list->resize (340,80) ;

}

