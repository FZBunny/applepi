/*****************************************************************************

    File: "disassemble_memory_dialog.h"

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

    void cleanupAddress (QLineEdit* text) ;

    void onDoItButtonClicked (void) ;

    void editAddress (QLineEdit* box) ;
    void onStartEdited   (void) ;
    void onStartFinished (void) ;
    void onEndEdited     (void) ;
    void onEndFinished   (void) ;

    void get16bitConfigData (char* key, QLineEdit* box) ;
    void getBoolConfigData  (char* key, QCheckBox* box) ;
    
    QRegExpValidator* m_hexValidator ;
    QWidget*      m_radioBox1 ;
    QRadioButton* m_mainButton ;
    QRadioButton* m_auxButton ;
    QRadioButton* m_romButton ;
    QRadioButton* m_nIstructions ;
    QRadioButton* m_nBytes ;
    QRadioButton* m_thisAddress ;

    QWidget*      m_radioBox2 ;

    ApplepiButton* m_doItButton ;

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

} ;

#endif

