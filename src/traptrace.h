/*****************************************************************************

    File: "traptrace.h"

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
//#include <QColor>

#include "mainwindow.h"
#include "disassembler.h"
#include "applepi_button.h"

class MainWindow ;


class TrapTrace: public QDialog
{
    public:

        TrapTrace (MainWindow* parent) ;
        virtual ~TrapTrace(){} ;

        void setHalted (bool halted) ;
        void reject (void) ;

    private:

        MainWindow* m_parent ;

 //       void  cleanupAddress (QLineEdit* text) ;
        void keyPressEvent(QKeyEvent *) ;

        void  editAddress (QLineEdit* box) ;
        void  onTrap0Edited (void) ;
        void  onTrap1Edited (void) ;
        void  onTrap2Edited (void) ;
        void  onTrap3Edited (void) ;

        QString* finishAddress (QLineEdit* box) ;
        void  onTrap0Finshed (void) ;
        void  onTrap1Finshed (void) ;
        void  onTrap2Finshed (void) ;
        void  onTrap3Finshed (void) ;

        void  onTrap0Entered (void) ;
        void  onTrap1Entered (void) ;
        void  onTrap2Entered (void) ;
        void  onTrap3Entered (void) ;
        void  onLinesToDumpEntered (void) ;

        void  onTrap0CheckBox   (void) ;
        void  onTrap1CheckBox   (void) ;
        void  onTrap2CheckBox   (void) ;
        void  onTrap3CheckBox   (void) ;
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
        QLineEdit*   m_trap0_AddressBox ;
        QLineEdit*   m_trap1_AddressBox ;
        QLineEdit*   m_trap2_AddressBox ;
        QLineEdit*   m_trap3_AddressBox ;
        QCheckBox*   m_trap0_checkBox ;
        QCheckBox*   m_trap1_checkBox ;
        QCheckBox*   m_trap3_checkBox ;
        QCheckBox*   m_trap2_checkBox ;
        QCheckBox*   m_history_checkBox ;
        QLineEdit*   m_requestedDumpLinesBox ;
        QCheckBox*   m_ss_fetch_checkBox ;
        QCheckBox*   m_ss_store_checkBox ;
        ApplepiButton* m_singleStepButton ;
        ApplepiButton* m_runFreeButton ;
        ApplepiButton* m_haltButton ;
        ApplepiButton* m_traceButton ;

        quint16       m_startAddr ;
        int           m_lineHeight ;
        int           m_nLines ;
        QFont        *m_panelFont ;
        int           m_historyIndex ;
        Disassembler *m_disassembler ;

} ;

#endif

