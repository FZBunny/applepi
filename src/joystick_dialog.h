/*****************************************************************************

    File: "joystick_dialog.h"

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


#ifndef JOYSTCK_DIALOG_H
#define JOYSTCK_DIALOG_H


#include <QDialog>
#include <QGroupBox>
#include <QRadioButton>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>

#include "mainwindow.h"


class gameController
{
public:
    gameController() {} ;
    ~gameController() {} ;

    int type ;
    int id ;
    QString name ;
} ;



class joystickDialog : public QDialog
{
public:

    joystickDialog (MainWindow* parent) ;
    ~joystickDialog() {} ;

private:

    void onSaveButton (void) ;
    void onCancelButton (void) ;
    void onMapButton (void) ;
    void onListWidget (QListWidgetItem *item) ;
    void openGamepad (void) ;

    void buttonBChanged(bool value) ;

    const int GAMEPAD = 0 ;
    const int MOUSE   = 1 ;

    MainWindow*    m_parent ;
    QListWidget*   m_listWidget ;
    QGamepad*      m_gamepad ;
    QString        m_currentControllerName ;
    QString        m_selectedControllerName ;
    int            m_selectedRow ;
    uint           m_selectedID ;

    QList<int>             m_controllerIDs ;
    QList<gameController>  m_controllerList ;

    QPushButton*  m_saveButton ;
    QPushButton*  m_cancelButton ;

} ;

#endif
