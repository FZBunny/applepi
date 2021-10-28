/*****************************************************************************

    File: "mainwindow.h"

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


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QMainWindow>
#include <QLabel>
#include <QSlider>
#include <QDial>
#include <QPushButton>

#include "screen.h"
#include "traptrace.h"
#include "machine.h"
#include "applepi_button.h"
#include "volume_dial.h"
#include "joystick.h"
#include "config.h"
#include "defs.h"



#ifdef Q_OS_WINDOWS
#  include "win-speaker.h"
#else
#  include "lin-speaker.h"
#endif


class Machine ;
class VolumeDial ;


class MainWindow : public QMainWindow
{

public:

    MainWindow (void) ;
    ~MainWindow();

//   static MainWindow* main_window ;

    Machine*   m_mac ;
    Config*    m_cfg ;
    Joystick*  m_joystick ;

    QString installRom (void) ;
    
    bool readRomFile (QString path) ;
    uint romNumber (void) ;
    void resizeWindow (void) ;
    bool powerIsOn (void) ;
    void floppyMotorHasStarted (int driveIndex) ;
    void HDActivityStarted (int driveIndex) ;
    void setHDLabel (uint drive, QString label) ;
    void setHDLabel (uint drive) ;
    void setFloppyLabel (uint drive, QString label) ;
    void setFloppyLabel (uint drive) ;
    void paste (void) ;
    void pastingText (bool pasting) ;

    void trapTraceIsStopping (void) ;
    
    Speaker* speaker (void) ;

    void onPowerButton   (void) ;
    void onScaleButton   (void) ;
    void onFloppy1Button (void) ;
    void onFloppy2Button (void) ;
    void onTapeButton    (void) ;
    void onResetButton   (void) ;
    void onHd1Button     (void) ;
    void onHd2Button     (void) ;
    void setVolume       (int volume) ;
    void onDialChanged   (void) ;

private:

    void moveEvent (QMoveEvent *event) ;

    virtual void  keyPressEvent (QKeyEvent *event) override ;

    Speaker*   m_speaker ;
    Screen*    m_screen ;
    
    bool    m_powerButtonState ;
    bool    m_currentlyPastingText ;
    bool    m_upperCaseOnly ;
    bool    m_capsLockOn ;

    QString m_romString ;
    QLabel* m_romLabel ;

    QTimer*  m_floppyCheckTimer ;
    QTimer*  m_oneSecondTimer ;

    int    m_floppyMotorCountDown[2] ;
    int    m_HDActivityCountDown[2] ;

    uint    m_scale ;
    uint    m_internalRomNumber ;
    QSize   m_screenSize ;
    QSize   m_minSize ;
    QSize   m_maxSize ;
    int     m_barHeight ;
    int     m_ledWidth ;
    bool    m_firstTimerEvent ;
    int     m_scaleButtonKludge ;

    ApplepiButton* m_powerButton ;
    ApplepiButton* m_floppy1Button ;
    ApplepiButton* m_floppy2Button ;
    ApplepiButton* m_tapeButton ;
    ApplepiButton* m_hd1Button ;
    ApplepiButton* m_hd2Button ;

    ApplepiButton* m_scaleButton ;
    ApplepiButton* m_resetButton ;
    
    QIcon   m_led_dim_red ;
    QIcon   m_led_bright_red ;
    QIcon   m_led_dim_green ;
    QIcon   m_led_bright_green ;
    
    QLabel* m_romLabelText ;

    QLabel* m_floppy1Label ;
    QLabel* m_floppy2Label ;
    
    QLabel*  m_hd1Label ;
    QLabel*  m_hd2Label ;

// Menus:
    QMenu*    m_fileMenu ;
    QMenu*    m_editMenu ;
    QMenu*    m_preferencesMenu ;
    QMenu*    m_debugMenu ;
    QMenu*    m_helpMenu ;

    QString* m_printToFilePrompt ;
    QString* m_closePrintToFilePrompt ;

// File actions:
    QAction* m_selectRomFile ;
    QAction* m_exit;

// Edit actions:
    QAction* m_screenSnapshot;
    QAction* m_copyText;
    QAction* m_pasteTextToKeyboardInput;

// Preferences  menu:
    QAction* m_keyboardcase ;
    QAction* m_selectPrint ;
    QAction* m_echoToConsole ;
    QAction* m_echoToFile ;

// Debug actions:
    QAction* m_viewMemory ;
    QAction* m_trapTrace ;
    QAction* m_disassembleMemory ;

// Help actions:
    QAction* m_help;
    QAction* m_about;

    QString  m_romPath ;
    QString  m_hdPath ;

    QString  m_textEchoPath ;
    QFile*   m_textEchoFile ;
//    QSlider* m_volumeSlider ;
    VolumeDial* m_dial ;

    QSize    m_buttonSize ;
    QSize    m_tapeButtonSize ;

    QLabel*  m_volText ;
    QLabel*  m_minText ;
    QLabel*  m_maxText ;

    bool m_trapTraceIsRunning ;

    void createMenus (void) ;
    void setCPuTimer (void) ;

    void onDiskDriveCheckTimer (void) ;
    void setProdosDateTime (void) ;

    void onSelectRom      (void) ;
    void onUseInternalRom (void) ;
    void onExit       (void) ;

    void onCopyScreen    (void) ;
    void pasteToKeyboard (void) ;

    void toggleScale (void) ;
    void onScreenScale   (void) ;

    void onKeyboardcase  (void) ;
    void onSelectPrinter (void) ;
    void onEchoToConsole (void) ;
    void onEchoToFile    (void) ;

    void onViewMemory    (void) ;
    void onTrapTrace     (void) ;
    void onDisassembleMemory (void) ;

    void onHelp  (void) ;
    void onAbout (void) ;

} ;


extern MainWindow* main_window ;


#endif // MAINWINDOW_H
