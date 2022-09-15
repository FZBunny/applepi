/*****************************************************************************

    File: "disassemble_memory_dialog.cpp"

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


#include <QDialog>
#include <QRegExp>
#include <QRegExpValidator>

//#include "disassembler.h"
#include "applepi_button.h"
#include "disassemble_memory_dialog.h"



DisassembleMemory::DisassembleMemory (QWidget* parent) :  QDialog (parent)
{
    this->setWindowTitle ("Disassemble Memory") ;
    int windowWidth = 306 ;
    int windowHeight = 280 ;

    resize (windowWidth,windowHeight) ;
    setMinimumSize (windowWidth,windowHeight) ;
    setMaximumSize (windowWidth,windowHeight) ;

    m_hexValidator = new QRegExpValidator (QRegExp ("[0123456789ABCDEFabcdef]{1,5}"), this) ;

    int box1_X = 160 ;
    int box1_Y = 30 ;

    int box2_X = 160 ;
    int box2_Y = 130 ;

    m_doItButton = new ApplepiButton ("Disassemble", this) ;
    m_doItButton->move (100,windowHeight-50) ;
    m_doItButton->setFocusPolicy (Qt::NoFocus) ;

    QFrame* box1 = new QFrame (this) ;
    box1->resize (120, 80) ;
    box1->move (box1_X, box1_Y) ;
    box1->setFrameStyle (QFrame::Panel | QFrame::Raised) ;
    box1->setLineWidth(1) ;
    box1->show() ;

    QFrame* box2 = new QFrame (this) ;
    box2->resize (120, 80) ;
    box2->move (box2_X, box2_Y) ;
    box2->setFrameStyle (QFrame::Panel | QFrame::Raised) ;
    box2->setLineWidth(1) ;
    box2->show() ;

    const QSize buttonSize (100, 30) ;
    const QSize textSize (52,26) ;

    m_startBox = new QLineEdit ("0000", this) ;
    m_startBox->move (box1_X-60, box1_Y+25) ;
    m_startBox->resize (45,24) ;
    m_startBox->setValidator (m_hexValidator) ;
    get16bitConfigData ((char*)"disassemble_start_address", m_startBox) ;

    m_startText = new QLabel ("Start At",  this) ;
    m_startText->move (box1_X-120, box1_Y+28) ;

    m_endBox = new QLineEdit ("0000", this) ;
    m_endBox->move (box2_X-60, box2_Y+25) ;
    m_endBox->resize (45,24) ;
    m_endBox->setValidator (m_hexValidator) ;
    get16bitConfigData ((char*)"disassemble_end", m_endBox) ;

    m_endText = new QLabel ("End After", this) ;
    m_endText->move (box2_X-130, box2_Y+28) ;

    m_mainButton = new QRadioButton ("Main RAM", box1) ;
    m_auxButton  = new QRadioButton ("Aux RAM", box1) ;
    m_romButton  = new QRadioButton ("ROM", box1) ;
    m_mainButton->move (10, 10) ;
    m_auxButton->move  (10, 30) ;
    m_romButton->move  (10, 50) ;

    m_nInstructions = new QRadioButton ("Instructions", box2)  ;
    m_nBytes        = new QRadioButton ("Bytes", box2) ;
    m_thisAddress   = new QRadioButton ("Address", box2)  ;
    m_nInstructions->move (10, 10) ;
    m_nBytes->move        (10, 30) ;
    m_thisAddress->move   (10, 50) ;

 //XXXXX    m_nInstructions->setChecked (true) ; // XXXXX
    uint  memType ;
    CFG->Get ("disassemble_mem_type", &memType) ;
    switch (memType) {
      case 0:
         m_mainButton->setChecked (true) ;
         break ;
      case 1:
         m_auxButton->setChecked (true) ;
         break ;
      case 2:
         m_romButton->setChecked (true) ;
         break ;
      default:
         m_mainButton->setChecked (true) ;
    }

    uint  endCriterion ;
    CFG->Get ("disassemble_end_criterion", &endCriterion) ;
    switch (endCriterion) {
      case 0:
         m_nInstructions->setChecked (true) ;
         break ;
      case 1:
         m_nBytes->setChecked (true) ;
         break ;
      case 2:
         m_thisAddress->setChecked (true) ;
         break ;
      default:
         m_nInstructions->setChecked (true) ;
    }




    connect (m_doItButton, &ApplepiButton::clicked, this, &DisassembleMemory::onDoItButtonClicked) ;

    connect (m_mainButton, &QRadioButton::toggled, this, &DisassembleMemory::onMainButton) ;
    connect (m_auxButton,  &QRadioButton::toggled, this, &DisassembleMemory::onAuxButton) ;
    connect (m_romButton,  &QRadioButton::toggled, this, &DisassembleMemory::onRomButton) ;

    connect (m_nInstructions, &QRadioButton::toggled, this, &DisassembleMemory::onNInstructions) ;
    connect (m_nBytes,        &QRadioButton::toggled, this, &DisassembleMemory::onNBytes) ;
    connect (m_thisAddress,   &QRadioButton::toggled, this, &DisassembleMemory::onThisAddress) ;

    connect (m_startBox, &QLineEdit::textEdited,      this, &DisassembleMemory::onStartEdited) ;
    connect (m_startBox, &QLineEdit::editingFinished, this, &DisassembleMemory::onStartFinished) ;
    connect (m_endBox,   &QLineEdit::textEdited,      this, &DisassembleMemory::onEndEdited) ;
    connect (m_endBox,   &QLineEdit::editingFinished, this, &DisassembleMemory::onEndFinished) ;

    m_disassembler = new Disassembler (MAC->m_rom, false) ;

}


void DisassembleMemory::get16bitConfigData (char* key, QLineEdit* box)
{
    QString tmpString ;

    CFG->Get ((char*)key, &tmpString) ;
    box->setText (tmpString) ;
}


void DisassembleMemory::cleanupAddress (QLineEdit* box)
{
    QString numString = box->text() ;
    int n = numString.length() ;
    if (n > 4) {
        numString = numString.right(4) ;
    } else if (n < 4) {
        for (int i=0; i<4-n; i++) {
            numString = QString ("0") + numString ;
        }
    }
    numString = numString.toUpper() ;
    box->setText (numString) ;
}


void DisassembleMemory::onStartFinished (void)
{
    cleanupAddress (m_startBox) ;
    QString str = m_startBox->text() ;
    CFG->Set ("disassemble_start_address", str) ;
    ulong longAddr;
    longAddr = str.toULong (NULL, 16) ;
    quint16 addr = longAddr ;
    MAC->setTraceStart (addr) ;
}


void DisassembleMemory::onEndFinished (void)
{
    cleanupAddress (m_endBox) ;
    QString str = m_endBox->text() ;
    CFG->Set ("disassemble_end", str) ;
    ulong longAddr;
    longAddr = str.toULong (NULL, 16) ;
    quint16 addr = longAddr ;
    MAC->setTraceEnd (addr) ;
}


void DisassembleMemory::onStartEdited (void)
{
    editAddress (m_startBox) ;
}


void DisassembleMemory::onEndEdited (void)
{
    editAddress (m_endBox) ;
}


void DisassembleMemory::editAddress (QLineEdit* box)
{
    QString text = box->text() ;
    int pos = box->cursorPosition() ;
    int len = text.length() ;
    if (pos > 4) {
        text = text.remove (0,1) ;
        box->setText (text) ;
    } else {
        if (len > 4) text = text.remove (len-1,1) ;
        box->setText (text) ;
        box->setCursorPosition (pos) ;
    }
}


void DisassembleMemory::onMainButton (void)
{
    CFG->Set ("disassemble_mem_type", uint(0)) ;
}


void DisassembleMemory::onAuxButton (void)
{
    CFG->Set ("disassemble_mem_type", uint(1)) ;
}


void DisassembleMemory::onRomButton (void)
{
    CFG->Set ("disassemble_mem_type", uint(2)) ;
}

void DisassembleMemory::onNInstructions (void)
{
    CFG->Set ("disassemble_end_criterion", uint(0)) ;
}


void DisassembleMemory::onNBytes (void)
{
    CFG->Set ("disassemble_end_criterion", uint(1)) ;
}


void DisassembleMemory::onThisAddress (void)
{
    CFG->Set ("disassemble_end_criterion", uint(2)) ;
}


void DisassembleMemory::onDoItButtonClicked (void)
{
//    QRect rect = m_doItButton->geometry() ;

    quint8* mem ;

    if      (m_mainButton->isChecked()) mem = MAC->m_ram ;
    else if (m_auxButton->isChecked())  mem = MAC->m_aux ;
    else                                mem = MAC->m_rom ;

    long longNumber ;
    QString string = m_startBox->text() ;
    longNumber = string.toLong (NULL, 16) ;
    quint16 start = longNumber ;

    string = m_endBox->text() ;
    longNumber = string.toLong (NULL, 16) ;
    quint16 end = longNumber ;
    if (end == 0) return ; 

    quint16 address = start ;
    int count = 0 ;
    char buffer[128] ;

    printf ("\n") ;
    while (true) {
        address = m_disassembler->sprint_6502_instruction (address, mem[address], mem[address+1], mem[address+2], buffer) ;
        printf ("%s\n", buffer) ;
        count++ ;

        if (m_nInstructions->isChecked()) {
            if (count >= end) break ;
        } else if (m_nBytes->isChecked()) {
            if ((address - start) >= end) break ;
        } else {
            if (address >= end) break ;
        }
    }
    printf ("\n") ;
    fflush (stdout) ;

}

