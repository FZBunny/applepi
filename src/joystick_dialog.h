
#ifndef JOYSTCK_DIALOG_H
#define JOYSTCK_DIALOG_H


#include <QDialog>
#include <QGroupBox>
#include <QRadioButton>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>


//#include <>
//#include <>


class joystickDialog : public QDialog
{
public:


private:

    QListWidget*  m_list ;

    QRadioButton* m_mouse ;
    QRadioButton* m_joystick ;

    QSpinBox*     m_sensitivityBox ;
    QSpinBox*     m_deadbandBox ;

    QPushButton*  m_doneButton ;
    QLabel*       m_lable1 ;
    QLabel*       m_lable2 ;
    


} ;

#endif
