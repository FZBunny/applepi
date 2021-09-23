
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

        void  onTrap0Entered (void) ;

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

        void  onTrap1Entered (void) ;
        void  onTrap2Entered (void) ;
        void  onTrap3Entered (void) ;
        void  onLinesToDumpEntered (void) ;

//        void  onTrap0LostFocus (wxFocusEvent& event) ;   //   XXXXX ? XXXXX

        void  onTrap0CheckBox   (void) ;
        void  onTrap1CheckBox   (void) ;
        void  onTrap2CheckBox   (void) ;
        void  onTrap3CheckBox   (void) ;
        void  onHistoryCheckBox (void) ;

        void  onTraceStartEntered (void) ;
        void  onTraceEndEntered   (void) ;

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
        QPushButton* m_singleStepButton ;
        QPushButton* m_runFreeButton ;
        QPushButton* m_haltButton ;
        QPushButton* m_traceButton ;
        QCheckBox*   m_ss_fetch_checkBox ;
        QCheckBox*   m_ss_store_checkBox ;

        quint16       m_startAddr ;
        int           m_lineHeight ;
        int           m_nLines ;
        QFont        *m_panelFont ;
        int           m_historyIndex ;
        Disassembler *m_disassembler ;

} ;

#endif

