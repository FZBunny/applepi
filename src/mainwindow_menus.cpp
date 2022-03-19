/*****************************************************************************

    File: "mainwindow_menus.cpp"

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



#include <Qt>
#include <QMenuBar>
#include <QFileDialog>
#include <QPrinter>
#include <QPrinterInfo>
#include <QPrintDialog>
#include <QMessageBox>
#include <QErrorMessage>

#include "about.h"
#include "machine.h"
#include "mainwindow.h"
#include "config.h"
#include "help.h"

#include "traptrace.h"
#include "watchaddr.h"

#include "gamepad_dialog.h"
#include "rom_dialog.h"
#include "view_memory.h"
//#include "about.h"
#include "disassemble_memory_dialog.h"



extern "C" void trim (char* buffer, int len) ;


void MainWindow::createMenus (void)
{
    m_printToFilePrompt = new QString (tr("Print to File...")) ;
    m_closePrintToFilePrompt = new QString ("Close Printer file") ;

    m_fileMenu        = menuBar()->addMenu (tr("&File")) ;
    m_editMenu        = menuBar()->addMenu (tr("&Edit")) ;
    m_preferencesMenu = menuBar()->addMenu (tr("&Preferences")) ;
    m_debugMenu       = menuBar()->addMenu (tr("&Debug")) ;
    m_helpMenu        = menuBar()->addMenu (tr("&Help")) ;

// "File" menu  ------------------------------------

    m_selectRomFile = new QAction (tr("Select AppleII Model..."), this) ;
    m_fileMenu->addAction (m_selectRomFile) ;
    connect (m_selectRomFile, &QAction::triggered, this, &MainWindow::onSelectRom) ;

//    m_printPrinter = m_fileMenu->addAction (tr("Print...")) ;
    m_filePrint = m_fileMenu->addMenu (tr("Print to File")) ;
    m_printPDF = new QAction ("Print a PDF...") ;
    m_printText    = new QAction ("Print Text...") ;
    m_filePrint->addAction (m_printText) ;
    m_filePrint->addAction (m_printPDF) ;
//    connect (m_printPrinter, &QAction::triggered, this, &MainWindow::onPrintPrinter) ;
    connect (m_printText,    &QAction::triggered, this, &MainWindow::onPrintText) ;
    connect (m_printPDF,     &QAction::triggered, this, &MainWindow::onPrintPDF) ;

    m_exit = new QAction (tr("Exit"), this) ;
    m_fileMenu->addAction (m_exit) ;
    connect (m_exit, &QAction::triggered, this, &MainWindow::onExit) ;

    m_fileMenu->insertSeparator (m_exit) ;

// "Edit" menu  ------------------------------------

    m_copyText = new QAction (tr("Copy Screen Text to Clipboard"), this) ;
    m_editMenu->addAction (m_copyText) ;
    connect (m_copyText, &QAction::triggered, this, &MainWindow::onCopyScreen) ;

    m_pasteTextToKeyboardInput = new QAction (tr("Paste Text to Keyboard"), this) ;
    m_pasteTextToKeyboardInput->setShortcut (QKeySequence(tr("Ctrl+v"))) ;
    m_editMenu->addAction (m_pasteTextToKeyboardInput) ;
    connect (m_pasteTextToKeyboardInput, &QAction::triggered, this, &MainWindow::pasteToKeyboard) ;

// "Preferences" menu  ------------------------------------

 //   m_gameController = new QAction (tr("Select Game Controller...")) ;
 //   m_preferencesMenu->addAction (m_gameController) ;
 //   connect (m_gameController, &QAction::triggered, this, &MainWindow::onGameController) ;

  
    m_keyboardcase = new QAction (tr("Use Mixed-Case Keyboard"), this) ;
    m_preferencesMenu->addAction (m_keyboardcase) ;
    connect (m_keyboardcase, &QAction::triggered, this, &MainWindow::onKeyboardcase) ;

    m_echoToTerminal = new QAction (tr("Echo Screen Text to Terminal"), this) ;
    m_preferencesMenu->addAction (m_echoToTerminal) ;
    connect (m_echoToTerminal, &QAction::triggered, this, &MainWindow::onEchoToTerminal) ;

    m_echoToFile = new QAction (tr("Echo Screen Text to File..."), this) ;
    m_preferencesMenu->addAction (m_echoToFile) ;
    connect (m_echoToFile, &QAction::triggered, this, &MainWindow::onEchoToFile) ;

// "Debug" menu  ------------------------------------

    m_viewMemory = new QAction (tr("View Memory..."), this) ;
    m_debugMenu->addAction (m_viewMemory) ;
    connect (m_viewMemory, &QAction::triggered, this, &MainWindow::onViewMemory) ;

    m_disassembleMemory = new QAction (tr("Disassemble Memory..."), this) ;
    m_debugMenu->addAction (m_disassembleMemory) ;
    connect (m_disassembleMemory, &QAction::triggered, this, &MainWindow::onDisassembleMemory) ;

    m_trapTrace = new QAction (tr("Trap and Trace..."), this) ;
    m_debugMenu->addAction (m_trapTrace) ;
    connect (m_trapTrace, &QAction::triggered, this, &MainWindow::onTrapTrace) ;

//    m_watchAddr = new QAction (tr("Watch Addresses..."), this) ;
//    m_debugMenu->addAction (m_watchAddr) ;
//    connect (m_watchAddr, &QAction::triggered, this, &MainWindow::onWatchAddr) ;

// ------ "Help" menu  ------

    m_help = new QAction (tr("Help..."), this) ;
    m_helpMenu->addAction (m_help) ;
    connect (m_help, &QAction::triggered, this, &MainWindow::onHelp) ;

    m_about = new QAction (tr("About..."), this) ;
    m_helpMenu->addAction (m_about) ;
    connect (m_about, &QAction::triggered, this, &MainWindow::onAbout) ;

    m_trapTraceIsRunning = false ;
}


//   ------  File menus  ------


void MainWindow::onSelectRom (void)
{
    RomDialog* dlg = new RomDialog (this) ;
    dlg->exec() ;
    delete dlg ;
}

/***
#ifdef Q_OS_WINDOWS

#else

void MainWindow::onPrintPrinter (void)
{
    QPrinter printer ;
    QPrintDialog dlg (&printer, this) ;
    if (dlg.exec()) {
printf ("OK\n") ;
        QPrinterInfo printerInfo (printer) ;
qStdOut() << "printer info = \"" << printerInfo.description() << "\"" << endl ;
qStdOut() << "printer name = \"" << printerInfo.printerName() << "\"" << endl ;
        int pid = fork() ;
        char* dummyArg = NULL ;
        if (pid == 0) {
//            m_closePrinterButton->show() ;
//            
        }
    }
}

#endif
***/

void MainWindow::onPrintText (void)
{
    openPrintFile (".txt") ;
}


void MainWindow::onPrintPDF (void)
{
    openPrintFile (".pdf") ;
}


void MainWindow::openPrintFile (QString suffix)
{
    QString fileKey = "directory_for_slot1_print" ;        
    QString dir1 ;
    QString dir2 ;

    if (MAC->m_printer->haveOpenFile()) {      // Already printing to a file.
        return ;                               // (This should never happen...)
    } else {                                   // Currently printing to stdout. Open a file.   
        CFG->Get (fileKey, &dir1) ;
        QString caption ;
        QTextStream (&caption) << "Print to " << suffix ; //.remove (0,1) ; 
        QString filter ;
        if (suffix == ".pdf") filter = "PDF File (*.pdf *.PDF);;Any *" ;
        else                  filter = "Text File (*.txt *.TXT);;Any *" ; ;  // XXXXXXXXXX  BUG if no suffix in selected file FIXME XXXXXX
        QString filename = QFileDialog::getSaveFileName (this, caption, dir1, filter) ;

        if (filename.length()) {
            int dotIx = filename.indexOf ('.') ;
            if (dotIx < 0) filename += QString (suffix) ;
            QFileInfo info (filename) ;
            bool ok = MAC->m_printer->open (filename) ;
            if (ok) {
                dir2 = info.absoluteDir().absolutePath() ;
                CFG->Set (fileKey, dir2) ;
                m_closePrinterButton->show() ;
//qStdOut() << "suffix = " << suffix << endl ;
                m_filePrint->setEnabled (false) ;
            } else {
                QErrorMessage* msg = new QErrorMessage (this) ;
                QString text ;
                QTextStream (&text) << "Can't open " << filename << " for printing:   " << MAC->m_printer->error() ;
                msg->showMessage (text) ;
            }
        }
    }
//qStdOut() << "dir2=" << dir2 << "; name=" << filename << endl ;
}


void MainWindow::onExit (void)
{
    ::exit(0) ;
}


//   ------  Edit menus  ------



static const int textBufferOffsets[24] = {
    0x000, 0x080, 0x100, 0x180,
    0x200, 0x280, 0x300, 0x380,
    0x028, 0x0a8, 0x128, 0x1a8,
    0x228, 0x2a8, 0x328, 0x3a8,
    0x050, 0x0d0, 0x150, 0x1d0,
    0x250, 0x2d0, 0x350, 0x3d0
} ;


void MainWindow::onCopyScreen (void)
{
#ifdef Q_OS_WINDOWS
    QString newLine("\r\n") ;
#else
    QString newLine("\n") ;
#endif
    quint8 page2   = MAC->m_ss[0x01c] ;
    quint8 rdText  = MAC->m_ss[0x01a] ;
    quint8 rd80Col = MAC->m_ss[0x01f] ;

    if (rdText == 0) return ;              // XXXXX Remove when code for graphics is included. XXXXX

//    quint8 rdHiRes = MAC->m_ss[0x01d] ;
//    quint8 rdMixed = MAC->m_ss[0x01b] ;

    QString string ;
    char  buffer[82] ;

    int screenBase ;
    if (page2) screenBase = 0x800 ;
    else       screenBase = 0x400 ;

    QClipboard *clipboard = QGuiApplication::clipboard() ;
    int lineBase ;

    if (rd80Col) {
        for (int line=0; line<24; line++) {
            lineBase = screenBase + textBufferOffsets[line] ;
            int i ;
            for (i=0; i<80; i++) {
                if (i&1) buffer[i] = MAC->m_ram[lineBase+(i/2)] & 0x7f ;
                else     buffer[i] = MAC->m_aux[lineBase+(i/2)] & 0x7f ;
                if (buffer[i] < 0x20) buffer[i] |= 0x40 ;
            }
            trim (buffer, 80) ;
         //   QString tmpString ;
            string.append((const char*)buffer) ;
            string.append (newLine) ;
        }
    } else {
        for (int line=0; line<24; line++) {
            lineBase = screenBase + textBufferOffsets[line] ;
            int i ;
            for (i=0; i<40; i++) {
                buffer[i] = MAC->m_ram[lineBase+i] & 0x7f ;
                if (buffer[i] < 0x20) buffer[i] |= 0x40 ;
            }
            trim (buffer, 40) ;
         //   QString tmpString ;
            string.append ((const char*)buffer) ;
            string.append (newLine) ;
        }

        clipboard->setText (string) ;
    }
}


//   ------  Preferences menus  ------


void MainWindow::onGameController(void)
{
    gamepadDialog* dlg = new gamepadDialog (this) ;
    dlg->exec() ;
}


void MainWindow::onKeyboardcase (void)
{
    m_upperCaseOnly = ! m_upperCaseOnly ;
    if (m_upperCaseOnly) m_keyboardcase->setText ("Use Mixed-Case Keyboard") ;
    else                 m_keyboardcase->setText ("Use Uppercase-Only Keyboard") ;
}


void MainWindow::onEchoToTerminal (void)
{
    if (MAC->toggleEchoToTerminal()) {
        m_echoToTerminal->setText ("Stop Echo to Terminal") ;
    } else {
        m_echoToTerminal->setText ("Echo Screen Text to Terminal") ;
    }
}


void MainWindow::onEchoToFile (void)
{
    if (MAC->isEchoingToFile() == false) {

        CFG->Get ("text_echo_path", &m_textEchoPath) ;
        QString fileTypes ("Text files (*.TXT, *.txt)||Any (*.*)|*.*") ;
        QFileDialog dialog  (this, tr("Create a new capture file"), m_textEchoPath, fileTypes) ;
        QString dir = QFileInfo (m_textEchoPath).absolutePath() ;
        dialog.setDirectory (dir) ;
        dialog.setModal(true) ;
        dialog.exec() ;
        if (dialog.result() == QDialog::Accepted) {
            QString path = dialog.selectedFiles().at(0) ;
            QString check = path.right(4) ;                    // Make sure pathname ends with ".txt". 
            if (QString::compare(check,".txt",Qt::CaseInsensitive) != 0) {
                path.append (".txt") ;
            }
            m_textEchoFile = new QFile (path, this) ;
            bool ok = m_textEchoFile->open (QIODevice::WriteOnly) ;
            if (ok) {
                MAC->echoToFile (m_textEchoFile) ;
                CFG->Set ("text_echo_path", path) ;
                m_echoToFile->setText ("Stop Echo to File") ;
            } else {
                QString msg ;
                QTextStream (&msg) << "Can't open " << path << " for writing: " << m_textEchoFile->errorString() ;
/*
QMessageBox::QMessageBox ( QMessageBox::Icon icon,
                           const QString &title,
                           const QString &text,
                           QMessageBox::StandardButtons buttons = NoButton,
                           QWidget *parent = nullptr,
                           Qt::WindowFlags f = Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint)
*/
                QMessageBox msgBox (QMessageBox::Warning, "Can't open the file", msg) ;
                msgBox.exec() ;
                delete  m_textEchoFile ;
            }
          
        }

    } else {
        MAC->closeEchoFile() ;
        m_echoToFile->setText ("Echo Screen Text to File") ;
    }

}


//   ------  Debug menus  ------


void MainWindow::onViewMemory (void)
{
    ViewMemory *view = new ViewMemory (this) ;
    view->show() ;
}


void MainWindow::onTrapTrace (void)
{
    if (m_trapTraceIsRunning) return ;  // probably can't happen, but WTF.

    m_trapTrace->setEnabled (false) ;   // Only allow one instance of a TrapTrace object.
    m_trapTraceIsRunning = true ;

    TrapTrace *trapTrace = new TrapTrace (this) ;
    trapTrace->show() ;

}


void MainWindow::trapTraceIsStopping (void)  // (Called in "TrapTrace::done" just before it exits.)
{
    m_trapTraceIsRunning = false ;
    m_trapTrace->setEnabled (true) ;
}


void MainWindow::onWatchAddr (void)
{
    if (m_watchAddrIsRunning) return ;  // Again, probably can't happen, but WTF.

    m_watchAddr->setEnabled (false) ;   // Only allow one instance of a WatchAddr object.
    m_watchAddrIsRunning = true ;

    WatchAddr *watchAddr = new WatchAddr (this) ;
    watchAddr->show() ;

}


void MainWindow::watchAddrIsStopping (void)  // (Called in "WatchAddr::done" just before it exits.)
{
    m_watchAddrIsRunning = false ;
    m_watchAddr->setEnabled (true) ;
}


void MainWindow::onDisassembleMemory (void)
{
    DisassembleMemory* dis = new DisassembleMemory (this) ;
    dis->show() ;
}


//   ------  Help menus  ------


void MainWindow::onHelp (void)
{
// For the time being, just kick off the default web browser.
// Support for displaying HTML is currently missing from the R.Pi port of Qt5.

//    QDesktopServices::openUrl (QUrl("file:///usr/share/applepi/ApplePiHelp.html")) ; 
    Help* help = new Help (this) ;
    help->exec() ;
}


void MainWindow::onAbout (void)
{
    About* about = new About ((QWindow*)this) ;
    about->exec() ;
}

