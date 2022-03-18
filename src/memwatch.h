/*****************************************************************************

    File: "watchtrace.h"

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


#ifndef TRAPTRACE_H
#define TRAPTRACE_H


#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QRegExp>
#include <QRegExpValidator>

#include "mainwindow.h"
#include "disassembler.h"

class MainWindow ;


class MemWatch: public QDialog
{
    public:

        MemWatch (MainWindow* parent) ;
        virtual ~MemWatch(){} ;

        void setHalted (bool halted) ;
        void reject (void) ;

    private:

        MainWindow* m_parent ;

 //       void  cleanupAddress (QLineEdit* text) ;
        void keyPressEvent(QKeyEvent *) ;

        void  editAddress (QLineEdit* box) ;
        void  onWatch0Edited (void) ;
        void  onWatch1Edited (void) ;
        void  onWatch2Edited (void) ;
        void  onWatch3Edited (void) ;

        QString* finishAddress (QLineEdit* box) ;
        void  onWatch0Finshed (void) ;
        void  onWatch1Finshed (void) ;
        void  onWatch2Finshed (void) ;
        void  onWatch3Finshed (void) ;

        void  onWatch0Entered (void) ;
        void  onWatch1Entered (void) ;
        void  onWatch2Entered (void) ;
        void  onWatch3Entered (void) ;
        void  onLinesToDumpEntered (void) ;

        void  onWatch0CheckBox   (void) ;
        void  onWatch1CheckBox   (void) ;
        void  onWatch2CheckBox   (void) ;
        void  onWatch3CheckBox   (void) ;
        void  onHistoryCheckBox (void) ;

        void  onTraceStartEdited (void) ;
        void  onTraceStartFinshed (void) ;
        void  onTraceEndEdited   (void) ;
        void  onTraceEndFinshed   (void) ;

        void  onTraceButtonClicked (void) ;
        void  onSingleStepClicked  (void) ;
        void  onRunFreeClicked     (void) ;
        void  onHaltClicked        (void) ;
        void  onSs_fetch_checkBox  (void) ;
        void  onSs_store_checkBox  (void) ;

        void  get16bitConfigData (char* key, QLineEdit* box) ;
        void  getBoolConfigData  (char* key, QCheckBox* box) ;

        void  printInstruction (quint16 pc) ;
//        void  showHistory (void) ;

//        QRegExp*          m_hexDigits ;
//        QRegExp*          m_decimalDigits ;
        QRegExpValidator* m_hexValidator ;
        QRegExpValidator* m_decimalValidator ;

        QWindow*     m_panel ;
        QLineEdit*   m_traceStartBox ;
        QLineEdit*   m_traceEndBox ;
        QLineEdit*   m_watch0_AddressBox ;
        QLineEdit*   m_watch1_AddressBox ;
        QLineEdit*   m_watch2_AddressBox ;
        QLineEdit*   m_watch3_AddressBox ;
        QCheckBox*   m_watch0_checkBox ;
        QCheckBox*   m_watch1_checkBox ;
        QCheckBox*   m_watch3_checkBox ;
        QCheckBox*   m_watch2_checkBox ;
        QCheckBox*   m_history_checkBox ;
        QLineEdit*   m_requestedDumpLinesBox ;
        QPushButton* m_singleStepButton ;
        QPushButton* m_runFreeButton ;
        QPushButton* m_haltButton ;
        QPushButton* m_traceButton ;

        quint16       m_startAddr ;
        int           m_lineHeight ;
        int           m_nLines ;
        QFont        *m_panelFont ;
        int           m_historyIndex ;
        Disassembler *m_disassembler ;

} ;

#endif

