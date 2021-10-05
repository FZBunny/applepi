/*****************************************************************************

    File: "mainwindow.cpp"

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




using namespace std ;

#include <iostream>

#include <QApplication>
#include <QIcon>
#include <QThread>
#include <QLabel>
#include <QMenuBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QPushButton>
#include <QKeyEvent>
#include <QDebug>

#include "defs.h"
#include "machine.h"
#include "config.h"
#include "mainwindow.h"
#include "hd_controller.h"
#include "floppy_dialog.h"
#include "floppy_controller.h"
#include "volume_dial.h"
//#include "tape_dialog.h"
#include "hd_dialog.h"
#include "paste_to_keyboard.h"
#include "debugging_dumps.h"
#include "xpm_images.h"
#include "rom_apple2.h"
#include "rom_apple2plus.h"
#include "rom_apple2e.h"
#include "rom_apple2e_enhanced.h"


MainWindow* main_window ;


MainWindow::MainWindow (void)
{
    main_window = this ;

    m_cfg = new Config() ;

    const QIcon* icon = new QIcon (QPixmap(xpm_redapple));
    this->setWindowIcon (*icon) ;

    m_minSize = QSize(MIN_MAINWINDOW_WIDTH,MIN_MAINWINDOW_HEIGHT) ;
    m_maxSize = QSize(MAX_MAINWINDOW_WIDTH, MAX_MAINWINDOW_HEIGHT) ;

    m_cfg->Get("window_scale", &m_scale) ;
    if (m_scale == 1) {
        m_screenSize = QSize (564, 388) ;
        this->setMinimumSize (m_minSize) ;
        this->setMaximumSize (m_minSize) ;
        this->resize (m_minSize) ;
    } else {
        m_screenSize = QSize (564*2, 388*2) ;
        this->setMinimumSize (m_maxSize) ;
        this->setMaximumSize (m_maxSize) ;
        this->resize (m_maxSize) ;
    }

    QPoint position ;
    m_cfg->Get ("window_position", &position) ;
    this->move (position) ;

    QFont font = this->font() ;
    font.setBold (true) ;
    this->setFont (font) ;

    font = this->menuBar()->font() ;
    font.setBold (false) ;
    this->menuBar()->setFont (font) ;
    
    font = this->statusBar()->font() ;
    font.setBold (false) ;
    this->statusBar()->setFont (font) ;

    m_romLabel = new QLabel ("") ;
    statusBar()->addPermanentWidget (m_romLabel) ;

    m_mac = new Machine (this) ;
    m_romString = installRom() ;

    m_currentlyPastingText = false ;
    m_upperCaseOnly = true ;

    m_barHeight = this->menuBar()->size().height() ;  // ( height == 30)

    m_screen = new Screen (this) ; 
    m_screen->move (3,m_barHeight) ;
    m_screen->resize (m_screenSize) ;
    
    m_led_bright_red   = QIcon (QPixmap(xpm_led_bright_red)) ;
    m_led_dim_red      = QIcon (QPixmap(xpm_led_dim_red)) ;
    m_led_bright_green = QIcon (QPixmap(xpm_led_bright_green)) ;
    m_led_dim_green    = QIcon (QPixmap(xpm_led_dim_green)) ;

    m_buttonSize = QSize (105, 30) ;
    m_tapeButtonSize = QSize (90, 30) ;
    
    m_powerButton   = new ApplepiButton (m_led_dim_green, "POWER", this) ;
    m_floppy1Button = new ApplepiButton (m_led_dim_red, "Floppy 1", this) ;
    m_floppy2Button = new ApplepiButton (m_led_dim_red, "Floppy 2", this) ;
    m_tapeButton    = new ApplepiButton (m_led_dim_red, "Tape", this) ;
    m_hd1Button     = new ApplepiButton (m_led_dim_red, "HD 1", this) ;
    m_hd2Button     = new ApplepiButton (m_led_dim_red, "HD 2", this) ;
    m_resetButton   = new ApplepiButton ("RESET", this) ;
    
    QString scaleButtonText ;
    if (m_scale == 1) scaleButtonText = "Big Screen" ;
    else              scaleButtonText = "Small Screen" ;
    m_scaleButton = new ApplepiButton (scaleButtonText, this) ;

    m_scaleButtonKludge = 2 ;                                  // Choose which kludge to apply to the sceeen-scale
                                                               // button; on some distros, the text overflows
    if (m_scaleButtonKludge == 1) {
        QFont sbFont = m_scaleButton->font() ;                 // 1... Embiggen the button to fit the text
        QFontMetrics fm (sbFont) ;
        int stringWidth = fm.horizontalAdvance("Small Screen") ;
        m_scaleButton->resize (stringWidth+6, m_buttonSize.height()) ;
    } else {
        int stringWidth ;                                      // 2... Find the largest font that will fit the button.
        int buttonWidth = m_buttonSize.width() ;
        QFont sbFont = m_scaleButton->font() ;
        int pointSize = sbFont.pointSize() ;
        do {
            sbFont.setPointSize (pointSize--) ;
            QFontMetrics fm (sbFont) ;
            stringWidth = fm.horizontalAdvance("Small Screen") ;
        } while (stringWidth > buttonWidth) ;
        m_scaleButton->setFont (sbFont) ;
    }

    connect (m_powerButton,   &ApplepiButton::clicked, this, &MainWindow::onPowerButton) ;
    connect (m_floppy1Button, &ApplepiButton::clicked, this, &MainWindow::onFloppy1Button) ;
    connect (m_floppy2Button, &ApplepiButton::clicked, this, &MainWindow::onFloppy2Button) ;
    connect (m_tapeButton,    &ApplepiButton::clicked, this, &MainWindow::onTapeButton) ;
    connect (m_hd1Button,     &ApplepiButton::clicked, this, &MainWindow::onHd1Button) ;
    connect (m_hd2Button,     &ApplepiButton::clicked, this, &MainWindow::onHd2Button) ;
    connect (m_resetButton,   &ApplepiButton::clicked, this, &MainWindow::onResetButton) ;
    connect (m_scaleButton,   &ApplepiButton::clicked, this, &MainWindow::onScaleButton) ;
    
    createMenus() ;
    m_powerButtonState = false ;

    this->setFocus() ;

    m_volText  = new QLabel ("Volume", this) ;
       
    QFont font2 ("courier", 8) ;
    font2.setBold (false) ;

    m_floppy1Label = new QLabel ("", this) ;
    m_floppy1Label->setFont(font2) ;
    m_floppy1Label->setAlignment (Qt::AlignHCenter|Qt::AlignVCenter) ;
    m_floppy2Label = new QLabel ("", this) ;
    m_floppy2Label->setFont(font2) ;
    m_floppy2Label->setAlignment (Qt::AlignHCenter|Qt::AlignVCenter) ;
    
    m_hd1Label = new QLabel ("", this) ;
    m_hd1Label->setFont(font2) ;
    m_hd1Label->setAlignment (Qt::AlignHCenter|Qt::AlignVCenter) ;
    m_hd2Label = new QLabel ("", this) ;
    m_hd2Label->setFont(font2) ;
    m_hd2Label->setAlignment (Qt::AlignHCenter|Qt::AlignVCenter) ;

    m_floppyCheckTimer = new QTimer() ;
    connect (m_floppyCheckTimer, &QTimer::timeout, this, &MainWindow::onDiskDriveCheckTimer) ;

    m_floppyCheckTimer->start (200) ;

    m_floppyMotorCountDown[0] = 0 ;
    m_floppyMotorCountDown[1] = 0 ;
    m_HDActivityCountDown[0] = 0 ;
    m_HDActivityCountDown[1] = 0 ;

    uint volume ;
    m_cfg->Get ("speaker_volume", &volume) ;
    m_speaker = new Speaker (this) ;
    m_speaker->setVolume (float(volume)) ;
    m_speaker->start() ;

    m_dial = new VolumeDial (this, 60, 60) ;
    
    m_mac->initialize(false) ;
    m_mac->start() ;

    setHDLabel (1) ;      // Write volume names under HD & 5.25" floppy buttons
    setHDLabel (2) ;
    setFloppyLabel (1) ;
    setFloppyLabel (2) ;

    m_firstTimerEvent = true ;

    m_tapeButton->setEnabled (false) ;   // ***  Remove this statement after fixing tape.cpp ***


    resizeWindow() ;
}


MainWindow::~MainWindow()
{
    ::exit(0) ;
}


// Copy a ROM image into the machine's ROM array.
// The image might an internal image (held in one of the ROM_apple2* arrays), or an external file.

QString MainWindow::installRom (void)
{
    memset (m_mac->m_rom, 0xff, 0x10000) ;

    QString romLabel ;
    QString useInternalROM ;
    m_cfg->Get ("use_internal_rom", &useInternalROM) ;
    if (useInternalROM == "yes") {
        m_cfg->Get ("internal_rom_number", &m_internalRomNumber) ;
        if (m_internalRomNumber > APPLE2E_ENHANCED) m_internalRomNumber = APPLE2E_ENHANCED ;
        quint8* rom = NULL ;
        int length = 0 ;
        switch (m_internalRomNumber) {
          case APPLE2:
            rom = ROM_apple2 ;
            romLabel = "Apple II" ;
            length = sizeof (ROM_apple2) ;
            break ;
          case APPLE2PLUS:
            rom = ROM_apple2plus ;
            romLabel = "Apple II+" ;
            length = sizeof (ROM_apple2plus) ;
            break ;
          case APPLE2E:
            rom = ROM_apple2e ;
            romLabel = "Apple IIe" ;
            length = sizeof (ROM_apple2e) ;
            break ;
          case APPLE2E_ENHANCED:
            rom = ROM_apple2e_enhanced ;
            romLabel = "Apple IIe enhanced" ;
            length = sizeof (ROM_apple2e_enhanced) ;
            break ;
          default:
            fprintf (stderr, "Configuration file contains illegal value (%i) for ROM number."
                     "  Defaulting to number for Apple IIe\n", m_internalRomNumber) ;
            m_internalRomNumber = APPLE2E ;
            break ;
        }
        memcpy (m_mac->m_rom+0x10000-length, rom, length) ;

    } else {
        m_internalRomNumber = APPLE_UNKNOWN ;
        romLabel = "Unknown ROM" ;
        m_cfg->Get ("rom_path", &m_romPath) ;
        QFileInfo testRomPath (m_romPath) ;
        if (testRomPath.exists()) {
            readRomFile (m_romPath) ;
        } else {
            QString text (
                "\n\nAn external ROM file has been specfied, but can no\n"
                "longer be found.\n"
                "Use the menu:    'File->Select AppleII Model...'\n"
                "to select a new one.\n"
                "Or just select one of the built-in ROMs.\n\n"
                "            --- We can't run without a ROM ---\n"
            ) ;
            QMessageBox* errorMsg = new QMessageBox (QMessageBox::Critical,
                                                     "Missing File",
                                                     text,
                                                     QMessageBox::Ok,
                                                     this) ;
            errorMsg->exec() ;
        }
    }

    m_romLabel->setText (QString("Model:  ") + romLabel) ;
    return romLabel ;
}


uint MainWindow::romNumber (void)
{
    return m_internalRomNumber ;
}


bool MainWindow::readRomFile (QString path)
{
    QFile file (path) ;
    bool ok = false ;

    if (file.open(QIODevice::ReadOnly))  {
        int size = file.size() ;           // Check that size <= 64K
        if (size > 0xFFFF) size = 0x10000 ;
        int offset = 0x10000 - size ;
        int n = file.read ((char*)m_mac->m_rom+offset, size) ;
        if (n != size) {
//            char* s = (char*)file.errorString().toUtf8() ;
//            fprintf (stderr, "Can't read file '%s': %s\n", path, ) ;   // XXXXXXX  Need a nice error window here. ** strerror is not portable!
            cerr << "Can't read file " << path.toStdString() << ": " << file.errorString().toStdString() << endl ;
            ok = false ;
        } else {
            m_romPath = path ;
//            QFileInfo romPath (m_romPath) ;
            m_cfg->Set ("rom_path", path) ;
            QString no ("no") ;
            m_cfg->Set ("use_internal_rom", no) ;
            ok = true ;
        }
        file.close() ;
    } else {
        cerr << "Can't open file " << path.toStdString() << ": " << file.errorString().toStdString() << endl ;
        ok = false ;
    }
    return ok ;
}


bool MainWindow::powerIsOn (void)
{
    return m_powerButtonState ;
}


// Change the screen size from big->small or small->big

void MainWindow::toggleScale (void)
{
//printf ("MainWindow::toggleScale entered.\n") ;
    if (m_scale == 1) {
        m_scale = 2 ;
        this->setMaximumSize (m_maxSize) ;
        this->setBaseSize    (m_maxSize) ;
        this->setMinimumSize (m_maxSize) ;
        m_scaleButton->setText ("Small Screen") ;
    } else {
        m_scale = 1 ;
        this->setMaximumSize (m_minSize) ;
        this->setBaseSize    (m_minSize) ;
        this->setMinimumSize (m_minSize) ;
        m_scaleButton->setText ("Big Screen") ;
    }
    resizeWindow() ;
}


const uint HD_ACTIVITY_OFF_DELAY  = 200 ;    // milliseconds
const uint FLOPPY_MOTOR_OFF_DELAY = 1000 ;   // milliseconds
const uint DISK_CHECK_INTERVAL    = 100 ;    // milliseconds

void MainWindow::HDActivityStarted (int driveIndex)
{
    if (driveIndex) {
        if (m_HDActivityCountDown[driveIndex] == 0 ) m_hd2Button->setIcon (m_led_bright_red) ;
    } else  {
        if (m_HDActivityCountDown[driveIndex] == 0 ) m_hd1Button->setIcon (m_led_bright_red) ;
    }

    m_HDActivityCountDown[driveIndex] = HD_ACTIVITY_OFF_DELAY ;
}


void MainWindow::floppyMotorHasStarted (int driveIndex)
{
    if (driveIndex) m_floppy2Button->setIcon (m_led_bright_red) ;
    else            m_floppy1Button->setIcon (m_led_bright_red) ;

    m_floppyMotorCountDown[driveIndex] = FLOPPY_MOTOR_OFF_DELAY ;
}


// Set label under an HD button to the volume name

void MainWindow::setHDLabel (uint drive, QString volumeName)
{
    if (drive==1) m_hd1Label->setText (volumeName) ;
    else          m_hd2Label->setText (volumeName) ;
}


// Set label under an HD button to vol.name or an 'empty' or error message

void MainWindow::setHDLabel (uint drive)
{
    QString key ;
    QString pathStr ;
    QString label ;
    QString slashLabel ;

    QTextStream (&key) << "hd" << drive << "_volume_path" ;
    m_cfg->Get (key, &pathStr) ;
    QFileInfo file (pathStr) ;
    if (file.exists() == false) {
        QDir dir = file.dir() ;
        pathStr = dir.path() ;
        m_cfg->Set (key, pathStr) ;
    }

    if (file.isDir()) {
        setHDLabel (drive, "- empty drive -") ;
    } else {
        if (file.exists()) {                  // (File was opened when HD controller object was created.)
            quint8 buffer[BLOCKSIZE] ;
            m_mac->m_hardDrive->readBlock (buffer, 2, drive-1) ;
            int nameLen = buffer[4] & 0x0f ;  // (volume name starts at 6th byte of block 2)
            buffer[nameLen+5] = 0 ;
            quint8 tmp[16] ;
            strcpy ((char*)tmp, (char*)buffer+5) ;
            label = QString ((const char*)tmp) ;
            slashLabel = "/" ;
            slashLabel += label ;
        } else {
            slashLabel = "* can't find file *" ;
        }

        setHDLabel (drive, slashLabel) ;
    }
}


void MainWindow::setFloppyLabel (uint drive, QString volumeName)
{
    if (drive==1) m_floppy1Label->setText (volumeName) ;
    else          m_floppy2Label->setText (volumeName) ;
}


void MainWindow::setFloppyLabel (uint drive)
{
    QString key ;
    QString pathStr ;
    QString label ;
    QString slashLabel ;
    quint8  buffer[SECTORSIZE] ;

    QTextStream (&key) << "floppy" << drive << "_path" ;
    m_cfg->Get (key, &pathStr) ;
    QFileInfo file (pathStr) ;
    if (file.exists() == false) {
        pathStr = file.path() ;
        m_cfg->Set (key, pathStr) ;
    }

    if (file.isDir()) {
        slashLabel = "- empty drive -" ;
    } else if (m_mac->m_floppy->OSType(drive) == DOS3) {     // --- DOS disk
        int n = m_mac->m_floppy->readSector (buffer, drive, 17, 0) ;    // (DOS VTOC is on track 17, sector 0)
        if (n == SECTORSIZE) {
            int releaseNumber = buffer[3] ;
            int volumeNumber  = buffer[6] ;
            QTextStream (&slashLabel) << "DOS" << releaseNumber << " Vol." << volumeNumber ;
        } else {
            slashLabel = " *** I/O Error *** " ;
        }
    } else {                                                 //--- ProDOS disk
        int n = m_mac->m_floppy->readSector (buffer, drive, 0, 11) ;    // Track 0, Sector 11 contains beginning 
        if (n == SECTORSIZE) {                                          // of volume dir on a 5.25" ProDos floppy
            int nameLen = buffer[4] & 0x0f ;  // (volume name starts at 6th byte of block 2)
            buffer[nameLen+5] = 0 ;
            quint8 tmp[16] ;
            strcpy ((char*)tmp, (char*)buffer+5) ;
            label = QString ((const char*)tmp) ;
            slashLabel = "/" ;
            slashLabel += label ;
        } else {
            slashLabel = " *** I/O Error *** " ;
        }
    }

    setFloppyLabel (drive, slashLabel) ;

}


// Called when a control-V is encountered on the keyboard.

void MainWindow::pasteToKeyboard (void)
{
    if (m_currentlyPastingText == false) {
        const QClipboard *clipboard = QApplication::clipboard() ;
        QString text = clipboard->text() ;
//qStdOut() << "MainWindow::  text to paste: " << text << endl ;
        Paste* p = new Paste (this, text) ;
        p->start() ;
    }
}


// 'pastingText' is called by the Paste thread to indicate when
//  it is pasting (a slow operation), and when it has finished.

void MainWindow::pastingText (bool pasting)
{
    m_currentlyPastingText = pasting ;
}


Speaker* MainWindow::speaker (void)
{
    return m_speaker ;
}


// ----------------------------------------------------------------------------
// -                                                                          -
// -                           Events & Signals                               -
// -                                                                          -
// ----------------------------------------------------------------------------


void MainWindow::keyPressEvent (QKeyEvent *e)
{
#ifdef Q_PROCESSOR_ARM
    const quint16 ctrlMod = 0x04 ;
#else
    const quint16 ctrlMod = 0x14 ;
#endif
    const quint16 passThese[3] = {0xff08,    // Backspace
                                  0xff0d,    // Enter (carriage return)
                                  0xffff} ;  // Delete
    quint16 c = e->nativeVirtualKey() ;
    quint16 mods = e->nativeModifiers() ;
// printf ("mods=%8.8x  c=%2.2x\n", mods, c) ;
    if ((c & 0xff00) == 0xff00) {
        switch (c) {
            case 0xff51:
                c = 0x08 ;   // backspace      (left-arrow)
                break ;
            case 0xff1b:
                c = 0x1b ;   // escape         
                break ;
            case 0xff52:
                c = 0x0b ;   // vertical tab   (up-arrow)
                break ;
            case 0xff53:
                c = 0x15 ;   // shift in       (right-arrow)
                break ;
            case 0xff54:
                c = 0x0a ;   // newline        (down-arrow)
                break ;
            default:
                int i ;
                for (i=2; i>=0; i--) if (c == passThese[i]) break ;
                if (i < 0) return ;
                break ;
        }
    }

    if (mods == ctrlMod) c &= 0x1f ;
    else                 c &= 0x7f ;
    if (m_upperCaseOnly && (c>0x60) && (c<0x7b)) c &= 0xdf ;
// printf ("               c=%2.2x\n", c) ;
    if (c == 0x16) pasteToKeyboard() ;
    else           MAC->m_ss[0] =  c | 0x80 ;

}


void MainWindow::onDiskDriveCheckTimer (void)
{
//putchar('.'); fflush (stdout) ;
    if (m_firstTimerEvent) {
        m_firstTimerEvent = false ;
        m_powerButton->setIcon (m_led_dim_green) ;
        m_floppy1Button->setIcon (m_led_dim_red) ;
        m_floppy2Button->setIcon (m_led_dim_red) ;
        m_tapeButton->setIcon (m_led_dim_red) ;
        m_hd1Button->setIcon (m_led_dim_red) ;
        m_hd2Button->setIcon (m_led_dim_red) ;
    }

    if (m_floppyMotorCountDown[0]) {
        m_floppyMotorCountDown[0] -= DISK_CHECK_INTERVAL ;
        if (m_floppyMotorCountDown[0] < 0) m_floppyMotorCountDown[0] = 0 ;
        if (m_floppyMotorCountDown[0] == 0) m_floppy1Button->setIcon (m_led_dim_red) ;
    }

    if (m_floppyMotorCountDown[1]) {
        m_floppyMotorCountDown[1] -= DISK_CHECK_INTERVAL ;
        if (m_floppyMotorCountDown[1] < 0) m_floppyMotorCountDown[1] = 0 ;
        if (m_floppyMotorCountDown[1] == 0) m_floppy2Button->setIcon (m_led_dim_red) ;
    }

    if (m_HDActivityCountDown[0]) {
        m_HDActivityCountDown[0] -= DISK_CHECK_INTERVAL ;
        if (m_HDActivityCountDown[0] < 0) m_HDActivityCountDown[0] = 0 ;
        if (m_HDActivityCountDown[0] == 0) m_hd1Button->setIcon (m_led_dim_red) ;
    }

    if (m_HDActivityCountDown[1]) {
        m_HDActivityCountDown[1] -= DISK_CHECK_INTERVAL ;
        if (m_HDActivityCountDown[1] < 0) m_HDActivityCountDown[1] = 0 ;
        if (m_HDActivityCountDown[1] == 0) m_hd2Button->setIcon (m_led_dim_red) ;
    }

}


void MainWindow::onScreenScale (void)
{
// fprintf ("MainWindow::onScreenScale\n") ;
    toggleScale() ;
}


void MainWindow::resizeWindow (void)
{
    m_cfg->Set ("window_scale", m_scale) ;

    int row1ButtonY ;
    int row2ButtonY ;
    int volumeX ;
    int volumeY ;
    int floppy2ButtonX ;
    int buttonWidth = m_buttonSize.width() ;
    int piXAdjust = 0 ;

    if (m_scale == 1) {                                                              // Small screen
        this->resize (MIN_MAINWINDOW_WIDTH, MIN_MAINWINDOW_HEIGHT) ;
        floppy2ButtonX = this->size().width()/2 - buttonWidth/2 ;
        m_screen->setScale(1) ;
        row1ButtonY = MIN_MAINWINDOW_HEIGHT - 140 ;
        row2ButtonY = MIN_MAINWINDOW_HEIGHT - 80 ;
        volumeX = 430 ;
        volumeY = 475 ;

        m_powerButton->move (5, row1ButtonY) ;
        if (m_scaleButtonKludge == 1) m_scaleButton->move (3, row2ButtonY) ;
        else                          m_scaleButton->move (5, row2ButtonY) ;
        m_floppy1Button->move (floppy2ButtonX-buttonWidth-5, row1ButtonY) ;
        m_tapeButton->move (floppy2ButtonX+buttonWidth+10, row1ButtonY) ;
        m_hd1Button->move (floppy2ButtonX-buttonWidth-5, row2ButtonY) ;
        m_hd2Button->move (floppy2ButtonX, row2ButtonY) ;
        m_floppy2Button->move (floppy2ButtonX, row1ButtonY) ;
        m_resetButton->move (MIN_MAINWINDOW_WIDTH-buttonWidth-15, row1ButtonY) ;
        
        m_floppy1Label->move (floppy2ButtonX-buttonWidth-5+piXAdjust, row1ButtonY+25) ;
        m_floppy2Label->move (floppy2ButtonX+piXAdjust, row1ButtonY+25) ;
        m_hd1Label->move (floppy2ButtonX-buttonWidth-5+piXAdjust, row2ButtonY+25) ;
        m_hd2Label->move (floppy2ButtonX+piXAdjust, row2ButtonY+25) ;
    } else {                                                                         // Big screen
        this->resize (MAX_MAINWINDOW_WIDTH, MAX_MAINWINDOW_HEIGHT) ;
        floppy2ButtonX = this->size().width()/2 - buttonWidth/2 ;
        m_screen->setScale(2) ;
        row1ButtonY = MAX_MAINWINDOW_HEIGHT - 140 ;
        row2ButtonY = MAX_MAINWINDOW_HEIGHT - 80 ;

        volumeX = 950 ;
        volumeY = 860 ;
        m_powerButton->move (20, row1ButtonY) ;
        m_scaleButton->move (20, row2ButtonY) ;
        m_floppy1Button->move (floppy2ButtonX-buttonWidth-60, row1ButtonY) ;
        m_floppy2Button->move (floppy2ButtonX, row1ButtonY) ;
        m_tapeButton->move (floppy2ButtonX+buttonWidth+60, row1ButtonY) ;
        m_resetButton->move (MAX_MAINWINDOW_WIDTH-buttonWidth-35, row1ButtonY) ;
        m_hd1Button->move (floppy2ButtonX-buttonWidth-60, row2ButtonY) ;
        m_hd2Button->move (floppy2ButtonX, row2ButtonY) ;
        
        m_floppy1Label->move (floppy2ButtonX-buttonWidth-55+piXAdjust, row1ButtonY+25) ;
        m_floppy2Label->move (floppy2ButtonX+piXAdjust, row1ButtonY+25) ;
        m_hd1Label->move (floppy2ButtonX-buttonWidth-60+piXAdjust, row2ButtonY+25) ;
        m_hd2Label->move (floppy2ButtonX+piXAdjust, row2ButtonY+25) ;
    }
    m_dial->move (volumeX, volumeY) ;
    m_volText->move (volumeX+5, volumeY+55) ;
  //  m_minText->move (volumeX-25, volumeY+40) ;
  //  m_maxText->move (volumeX+60, volumeY+40) ;

}


void MainWindow::moveEvent (QMoveEvent*)
{
    m_cfg->Set (QString("window_position"), this->pos()) ;
}


void MainWindow::onPowerButton (void)
{
    m_powerButtonState = ! m_powerButtonState ;
    if (m_powerButtonState) {
        m_mac->initialize (true) ;
        m_selectRomFile->setEnabled (false) ;
        m_powerButton->setIcon (m_led_bright_green) ;
    } else {
        m_mac->initialize (false) ;
        m_selectRomFile->setEnabled (true) ;
        m_powerButton->setIcon (m_led_dim_green) ;
        m_floppy1Button->setIcon (m_led_dim_red) ;
        m_floppy2Button->setIcon (m_led_dim_red) ;
        m_hd1Button->setIcon (m_led_dim_red) ;
        m_hd2Button->setIcon (m_led_dim_red) ;
        m_tapeButton->setIcon (m_led_dim_red) ;
        m_screen->initialize() ;
    }

}


void MainWindow::onScaleButton (void)
{
//printf ("MainWindow::onScaleButton entered.\n") ;
    this->toggleScale() ;
}


void MainWindow::onFloppy1Button (void)
{
    FloppyDialog dlg (this, 0) ;
    if (dlg.exec()) {

    }
}


void MainWindow::onFloppy2Button (void)
{
    FloppyDialog dlg (this, 1) ;
    if (dlg.exec()) {

    }
}


void MainWindow::onTapeButton (void)
{
//   TapeDialog dlg (this, 1) ;
//   dlg.ShowModal() ;
}


void MainWindow::onHd1Button (void)
{
    HardDriveDialog dlg (this, 1) ;
    dlg.exec() ;
}


void MainWindow::onHd2Button (void)
{
    HardDriveDialog dlg (this, 2) ;
    dlg.exec() ;
}


void MainWindow::onResetButton (void)
{
//printf ("onResetButton\n") ;
    if (m_mac->powerIsOn()) {
        m_mac->reset() ;
    }
}


void MainWindow::setVolume (int volume)
{
    m_speaker->setVolume (float(volume)) ;
}

