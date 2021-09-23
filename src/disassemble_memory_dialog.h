
#ifndef DISASSEMBLE_MEMORY_H
#define DISASSEMBLE_MEMORY_H


#include <QDialog>
#include <QLineEdit>
#include <QRadioButton>
#include <QRegExpValidator>


#include "machine.h"
#include "disassembler.h"



class DisassembleMemory: public QDialog
{
    public:

        DisassembleMemory (QWidget* parent) ;
        virtual ~DisassembleMemory() {} ;


    private:

     //   void  fixTextField   (QLineEdit* ctrl, wxKeyEvent& event, char* cfgKey) ;

        void  cleanupAddress (QLineEdit* text) ;

        void  onStartEntered (void) ;
        void  onEndEntered   (void) ;
        void  onDoItButtonClicked (void) ;

        void  get16bitConfigData (char* key, QLineEdit* box) ;
        void  getBoolConfigData  (char* key, QCheckBox* box) ;
        
        QRegExpValidator* m_hexValidator ;
        QWidget*      m_radioBox1 ;
        QRadioButton* m_mainButton ;
        QRadioButton* m_auxButton ;
        QRadioButton* m_romButton ;
        QRadioButton* m_nIstructions ;
        QRadioButton* m_nBytes ;
        QRadioButton* m_thisAddress ;

        QWidget*  m_radioBox2 ;

        QPushButton* m_doItButton ;

        QLineEdit*  m_startBox ;
        QLineEdit*  m_endBox ;


        QLabel*     m_startText ;
        QLabel*     m_endText ;

        quint16     m_startAddr ;
        int         m_lineHeight ;
        int         m_historyIndex ;
        int         m_nLines ;
        QFont      *m_panelFont ;
        QPixmap     m_bufferBits ;

        Disassembler *m_disassembler ;

};

#endif

