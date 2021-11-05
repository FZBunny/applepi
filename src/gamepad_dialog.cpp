/*****************************************************************************

    File: "gamepad_dialog.cpp"

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


#include <QTextStream>

#include "defs.h"
#include "gamepad_dialog.h"



gamepadDialog::gamepadDialog (MainWindow* parent)
{
    m_parent = parent ;

    int width = 360 ;
    int height = 170 ;
    int x, y ;
    CENTER_DIALOG
    setWindowTitle ("Select Controller") ;

    m_listWidget = new QListWidget (this) ;
    m_listWidget->move (20,20) ;
    m_listWidget->resize (320,80) ;

    m_saveButton = new QPushButton ("Save", this) ;
    m_saveButton->move (260,130) ;
    m_cancelButton = new QPushButton ("Cancel", this) ;
    m_cancelButton->move (20,130) ;

    CFG->Get ("game_controller_name", &m_currentControllerName) ;

    m_controllerIDs = QGamepadManager::instance()->connectedGamepads() ;
    gameController* controller ;

    for (int i=0; i<m_controllerIDs.length(); i++) {         // Add all gamepads / joystcks to
        int id = m_controllerIDs.at(i) ;                     // controller list & list widget
        m_gamepad = new QGamepad (id) ;
        controller = new gameController() ;
        controller->type = GAMEPAD ;
        controller->id = id ;
        controller->name = m_gamepad->name() ;
        m_listWidget->addItem (controller->name) ;
        m_controllerList.append (*controller) ;
        delete controller ;
    }

    for (int i=0;;i++) {                                   // Add all mice to lists
        QString mouseDev ;
        QTextStream(&mouseDev) << "/dev/input/mouse" << i ;
        if (QFileInfo::exists(mouseDev)) {
            m_listWidget->addItem (mouseDev) ;
            controller = new gameController() ;
            controller->type = MOUSE ;
            controller->id = 0 ;
            controller->name = mouseDev ;
            m_controllerList.append (*controller) ;
            delete controller ;
        } else {
            break ;
        }
    }

    for (int i=0; i<m_controllerList.length(); i++) {      // Highlight current controller on the list
        if (m_controllerList.at(i).name == m_currentControllerName) {
            m_listWidget->setCurrentRow (i, QItemSelectionModel::SelectCurrent) ;
            m_selectedRow = i ;
            break ;
        }
    }

    connect (m_saveButton,   &QPushButton::clicked,     this, &gamepadDialog::onSaveButton) ;
    connect (m_cancelButton, &QPushButton::clicked,     this, &gamepadDialog::onCancelButton) ;
    connect (m_listWidget,   &QListWidget::itemClicked, this, &gamepadDialog::onListWidget) ;

    m_saveButton->setEnabled (false) ;

}


void gamepadDialog::onListWidget (QListWidgetItem *item)
{
    m_selectedRow = m_listWidget->currentRow() ;
    m_selectedID = m_controllerList.at(m_selectedRow).id ;
    m_selectedControllerName = item->text() ;

    m_saveButton->setEnabled (true) ;
}


void gamepadDialog::onSaveButton (void)
{
    CFG->Set ("game_controller_name", m_selectedControllerName) ;
    CFG->Set ("game_controller_id", m_selectedID) ;
    m_parent->m_gamepad->openController() ;

    this->accept() ;
}


void gamepadDialog::onCancelButton (void)
{
    this->accept() ;
}

