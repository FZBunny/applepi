/*****************************************************************************

    File: "traptrace.cpp"

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

//using namespace std ;
//#include  <iomanip>

#include "traptrace.h"
#include "machine.h"


TrapTrace::TrapTrace (MainWindow* parent) :  QDialog (parent)
{
    this->setWindowTitle ("Trap and Trace") ;
    
    MAC->enableTrace (false) ;
    
    m_parent = parent ;
    int width = 650 ;
    int height = 235 ;
    setMinimumSize (width,height) ;
    setMaximumSize (width,height) ;

    int x, y ;
    CENTER_DIALOG
    
    this->setAttribute (Qt::WA_DeleteOnClose) ;

    MAC->setTrapTraceDialogPointer (this) ;  // (machine has to know how to tell us a trap has been hit)

    m_hexValidator = new QRegExpValidator (QRegExp ("[0123456789ABCDEFabcdef]{1,5}"), this) ;
    m_decimalValidator = new QRegExpValidator (QRegExp ("[0123456789]{1,6}"), this) ;

    const QSize buttonSize (90, 28) ;
    const QSize textSize (52,26) ;

#ifdef Q_PROCESSOR_ARM               // We assume an arm processor means we're running PiOS;
    QFont font1 = this->font() ;     // PiOS seems to use larger fonts than most other distros
    int ptSize = font1.pointSize() ;
    font1.setPointSize (ptSize-1) ;
    this->setFont (font1) ;
#endif


//---------  T R A P  -----------


    int trap_X = 20 ;
    int trap_Y = 0 ;

    int trapCheckbox_X = trap_X + 55 ;
    int trapCheckbox_Y = trap_Y + 21 ;

    int trapButtonX = trap_X + 170 ;
    int trapButtonY = 20 ;

    QSize addressBoxSize (50,24) ;

    QFrame* box1 = new QFrame (this, Qt::Widget) ;
    box1->move (10,25) ;
    box1->resize (300, 188) ;
    box1->setFrameStyle (QFrame::Panel | QFrame::Raised) ;
    box1->setLineWidth(3) ;
    box1->show() ;

    QFrame* box2 = new QFrame (this, Qt::Widget) ;
    box2->move (335,25) ;
    box2->resize (300, 188) ;
    box2->setFrameStyle (QFrame::Panel | QFrame::Raised) ;
    box2->setLineWidth(3) ;
    box2->show() ;

    m_singleStepButton = new ApplepiButton ("Single Step", box1) ;
    m_runFreeButton    = new ApplepiButton ("Continue", box1) ;
    m_haltButton       = new ApplepiButton ("- HALT -", box1) ;

    m_singleStepButton->move (trapButtonX, trapButtonY) ;
    m_singleStepButton->resize (buttonSize) ;
    m_runFreeButton->move (trapButtonX, trapButtonY+40) ;
    m_runFreeButton->resize (buttonSize) ;
    m_haltButton->move (trapButtonX, trapButtonY+80) ;
    m_haltButton->resize (buttonSize) ;

    m_trap0_AddressBox = new QLineEdit ("0000", box1) ;
    m_trap0_AddressBox->resize (addressBoxSize) ;
    m_trap0_AddressBox->move (trap_X,trap_Y+20) ;
    m_trap0_AddressBox->setAlignment (Qt::AlignCenter) ;
    m_trap0_AddressBox->setValidator (m_hexValidator) ;

    m_trap1_AddressBox = new QLineEdit ("0000", box1) ;
    m_trap1_AddressBox->resize (addressBoxSize) ;
    m_trap1_AddressBox->move (trap_X,trap_Y+50) ;
    m_trap1_AddressBox->setAlignment (Qt::AlignCenter) ;
    m_trap1_AddressBox->setValidator (m_hexValidator) ;

    m_trap2_AddressBox = new QLineEdit ("0000", box1) ;
    m_trap2_AddressBox->resize (addressBoxSize) ;
    m_trap2_AddressBox->move (trap_X,trap_Y+80) ;
    m_trap2_AddressBox->setAlignment (Qt::AlignCenter) ;
    m_trap2_AddressBox->setValidator (m_hexValidator) ;

    m_trap3_AddressBox = new QLineEdit ("0000", box1) ;
    m_trap3_AddressBox->resize (addressBoxSize) ;
    m_trap3_AddressBox->move (trap_X,trap_Y+110) ;
    m_trap3_AddressBox->setAlignment (Qt::AlignCenter) ;
    m_trap3_AddressBox->setValidator (m_hexValidator) ;

    m_trap0_checkBox = new QCheckBox ("1 Enable", box1) ;
    m_trap0_checkBox->move (trapCheckbox_X,trapCheckbox_Y) ;

    m_trap1_checkBox = new QCheckBox ("2 Enable", box1) ;
    m_trap1_checkBox->move (trapCheckbox_X,trapCheckbox_Y+30) ;
    
    m_trap2_checkBox = new QCheckBox ("3 Enable", box1) ;
    m_trap2_checkBox->move (trapCheckbox_X,trapCheckbox_Y+60) ;

    m_trap3_checkBox = new QCheckBox ("4 Enable", box1) ;
    m_trap3_checkBox->move (trapCheckbox_X,trapCheckbox_Y+90) ;

    m_history_checkBox = new QCheckBox ("Dump", box1) ;
    m_history_checkBox->move (trap_X+20,trap_Y+150) ;

    m_requestedDumpLinesBox = new QLineEdit ("0", box1) ;
    m_requestedDumpLinesBox->resize (70,24) ;
    m_requestedDumpLinesBox->move (trap_X+85,trap_Y+150) ;
    m_requestedDumpLinesBox->setAlignment (Qt::AlignRight) ;
    m_requestedDumpLinesBox->setValidator (m_decimalValidator) ;

    QLabel* lab1 = new QLabel ("Lines (decimal)", this) ;
    lab1->move (trap_X+170,trap_Y+165) ;
    lab1->resize (100,40) ;

    getBoolConfigData ((char*)"trap0_enable", m_trap0_checkBox) ;
    getBoolConfigData ((char*)"trap1_enable", m_trap1_checkBox) ;
    getBoolConfigData ((char*)"trap2_enable", m_trap2_checkBox) ;
    getBoolConfigData ((char*)"trap3_enable", m_trap3_checkBox) ;
    getBoolConfigData ((char*)"history_dump", m_history_checkBox) ;

    get16bitConfigData ((char*)"trap0_address", m_trap0_AddressBox) ;
    get16bitConfigData ((char*)"trap1_address", m_trap1_AddressBox) ;
    get16bitConfigData ((char*)"trap2_address", m_trap2_AddressBox) ;
    get16bitConfigData ((char*)"trap3_address", m_trap3_AddressBox) ;

    connect (m_singleStepButton, &ApplepiButton::clicked, this, &TrapTrace::onSingleStepClicked) ;
    connect (m_runFreeButton,    &ApplepiButton::clicked, this, &TrapTrace::onRunFreeClicked) ;
    connect (m_haltButton,       &ApplepiButton::clicked, this, &TrapTrace::onHaltClicked) ;

    connect (m_trap0_checkBox,   &QCheckBox::stateChanged, this, &TrapTrace::onTrap0CheckBox) ;
    connect (m_trap1_checkBox,   &QCheckBox::stateChanged, this, &TrapTrace::onTrap1CheckBox) ;
    connect (m_trap2_checkBox,   &QCheckBox::stateChanged, this, &TrapTrace::onTrap2CheckBox) ;
    connect (m_trap3_checkBox,   &QCheckBox::stateChanged, this, &TrapTrace::onTrap3CheckBox) ;

    connect (m_history_checkBox, &QCheckBox::stateChanged, this, &TrapTrace::onHistoryCheckBox) ;

    connect (m_trap0_AddressBox, &QLineEdit::textEdited, this,  &TrapTrace::onTrap0Edited) ;
    connect (m_trap1_AddressBox, &QLineEdit::textEdited, this,  &TrapTrace::onTrap1Edited) ;
    connect (m_trap2_AddressBox, &QLineEdit::textEdited, this,  &TrapTrace::onTrap2Edited) ;
    connect (m_trap3_AddressBox, &QLineEdit::textEdited, this,  &TrapTrace::onTrap3Edited) ;

    connect (m_trap0_AddressBox, &QLineEdit::editingFinished, this,  &TrapTrace::onTrap0Finshed) ;
    connect (m_trap1_AddressBox, &QLineEdit::editingFinished, this,  &TrapTrace::onTrap1Finshed) ;
    connect (m_trap2_AddressBox, &QLineEdit::editingFinished, this,  &TrapTrace::onTrap2Finshed) ;
    connect (m_trap3_AddressBox, &QLineEdit::editingFinished, this,  &TrapTrace::onTrap3Finshed) ;

    connect (m_requestedDumpLinesBox, &QLineEdit::editingFinished, this,  &TrapTrace::onLinesToDumpEntered) ;
    
    QString addr ;
    addr = m_trap0_AddressBox->text() ;
    MAC->setTrap (0, addr, m_trap0_checkBox->isChecked()) ;
    addr = m_trap1_AddressBox->text() ;
    MAC->setTrap (1, addr, m_trap1_checkBox->isChecked()) ;
    addr = m_trap2_AddressBox->text() ;
    MAC->setTrap (2, addr, m_trap2_checkBox->isChecked()) ;
    addr = m_trap3_AddressBox->text() ;
    MAC->setTrap (3, addr, m_trap3_checkBox->isChecked()) ;

    QString  tmpString ;
    CFG->Get ("lines2dump", &tmpString) ;

    m_requestedDumpLinesBox->selectAll() ;
    m_requestedDumpLinesBox->insert (tmpString) ;
    MAC->setLines2Dump (tmpString) ;

    MAC->enableHistory (m_history_checkBox->isChecked()) ;

    setHalted (false) ;

//---------  T R A C E  -----------

 //   int traceRef_X = 335 ;
 //   int traceRef_Y = 10 ;

    int traceButton_X   = 175 ;
    int traceStartBox_Y = 50 ;
    int ssTraceFetch_X  = 20 ;
    int ssTraceFetch_Y  = 70 ;

    m_traceStartBox = new QLineEdit ("0000", box2) ;
    m_traceStartBox->move (traceButton_X+20,traceStartBox_Y) ;
    m_traceStartBox->resize (addressBoxSize) ;
    m_traceStartBox->setAlignment (Qt::AlignCenter) ;
    m_traceStartBox->setValidator (m_hexValidator) ;
    get16bitConfigData ((char*)"trace_start_address", m_traceStartBox) ;

    m_traceEndBox = new QLineEdit ("0000", box2) ;
    m_traceEndBox->move (traceButton_X+20,traceStartBox_Y+30) ;
    m_traceEndBox->resize (addressBoxSize) ;
    m_traceEndBox->setAlignment (Qt::AlignCenter) ;
    m_traceEndBox->setValidator (m_hexValidator) ;
    get16bitConfigData ((char*)"trace_end_address", m_traceEndBox) ;

    m_ss_fetch_checkBox  = new QCheckBox ("Trace SS fetches", box2) ;
    m_ss_store_checkBox  = new QCheckBox ("Trace SS stores",  box2) ;
    m_ss_fetch_checkBox->move (ssTraceFetch_X, ssTraceFetch_Y) ;
    m_ss_store_checkBox->move (ssTraceFetch_X, ssTraceFetch_Y+25) ;

    m_traceButton = new ApplepiButton  ("Start Trace", box2) ;
    m_traceButton->move (traceButton_X,traceStartBox_Y+75) ;
    m_traceButton->resize (buttonSize) ;

    connect (m_ss_fetch_checkBox, &QCheckBox::stateChanged, this, &TrapTrace::onSs_fetch_checkBox) ;
    connect (m_ss_store_checkBox, &QCheckBox::stateChanged, this, &TrapTrace::onSs_store_checkBox) ;

    connect (m_traceStartBox, &QLineEdit::textEdited,      this,  &TrapTrace::onTraceStartEdited) ;
    connect (m_traceStartBox, &QLineEdit::editingFinished, this,  &TrapTrace::onTraceStartFinshed) ;

    connect (m_traceEndBox, &QLineEdit::textEdited,      this,  &TrapTrace::onTraceEndEdited) ;
    connect (m_traceEndBox, &QLineEdit::editingFinished, this,  &TrapTrace::onTraceEndFinshed) ;

    connect (m_traceButton,   &ApplepiButton::clicked, this, &TrapTrace::onTraceButtonClicked) ;

}


void TrapTrace::get16bitConfigData (char* key, QLineEdit* box)
{
    QString tmpString ;
    CFG->Get ((char*)key, &tmpString) ;
    box->setText (tmpString) ;
}


void TrapTrace::getBoolConfigData (char* key, QCheckBox* box)
{
    bool  value ;
    CFG->Get (key, &value) ;
    box->setChecked (value) ;
}


// Clear all machine traps and disable instruction tracing when this dialog is dismissed.

void TrapTrace::reject (void)
{
//printf ("void TrapTrace::reject\n") ;
    m_parent->trapTraceIsStopping() ;

    for (int i=0; i<3; i++) {
        MAC->setTrap (i, QString("0"), 0) ;
    }
    MAC->enableTrace (false) ;
    MAC->runFree() ;

    QDialog::reject() ;
}


// This function is overridden to ignore keystrokes which  
// would otherwise be seen by the buttons as 'click' events.
// That would be bad.

void TrapTrace::keyPressEvent (QKeyEvent *)
{
//printf ("keyPressEvent\n") ;
}


void TrapTrace::editAddress (QLineEdit* box)
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


QString* TrapTrace::finishAddress (QLineEdit* box)
{
    QString* text = new QString (box->text()) ;
    quint16 value = text->toUShort (NULL, 16) ;
    char buffer[8] ;
    sprintf (buffer, "%4.4X", value) ;
    *text = buffer ;
    box->selectAll() ;
    box->insert (*text) ;

    return text ;
}



//-------------------  T R A P  ---------------------



void TrapTrace::onTrap0Finshed (void)
{
    QString* text = finishAddress (m_trap0_AddressBox) ;
    CFG->Set ("trap0_address", *text) ;
    if (m_trap0_checkBox->isChecked()) MAC->setTrap (0, *text, true) ;
}


void TrapTrace::onTrap1Finshed (void)
{
    QString* text = finishAddress (m_trap1_AddressBox) ;
    CFG->Set ("trap1_address", *text) ;
    if (m_trap0_checkBox->isChecked()) MAC->setTrap (1, *text, true) ;
}


void TrapTrace::onTrap2Finshed (void)
{
    QString* text = finishAddress (m_trap2_AddressBox) ;
    CFG->Set ("trap2_address", *text) ;
    if (m_trap0_checkBox->isChecked()) MAC->setTrap (2, *text, true) ;
}


void TrapTrace::onTrap3Finshed (void)
{
    QString* text = finishAddress (m_trap3_AddressBox) ;
    CFG->Set ("trap3_address", *text) ;
    if (m_trap0_checkBox->isChecked()) MAC->setTrap (3, *text, true) ;
}


void TrapTrace::onTrap0Edited (void)
{
    editAddress (m_trap0_AddressBox) ;
}


void TrapTrace::onTrap1Edited (void)
{
    editAddress (m_trap1_AddressBox) ;
}


void TrapTrace::onTrap2Edited (void)
{
    editAddress (m_trap2_AddressBox) ;
}


void TrapTrace::onTrap3Edited (void)
{
    editAddress (m_trap3_AddressBox) ;
}


void TrapTrace::onLinesToDumpEntered (void)
{
    QString str = m_requestedDumpLinesBox->text() ;
    ulong nLines ;
    nLines = str.toUInt() ;
    if (nLines > HISTORY_LEN) {
        QTextStream (&str) << HISTORY_LEN ;
        m_requestedDumpLinesBox->selectAll() ;
        m_requestedDumpLinesBox->insert (str) ;
    }
    MAC->setLines2Dump (str) ;
    CFG->Set ("lines2dump", str) ;
}


void TrapTrace::onSingleStepClicked (void)
{
    MAC->singleStep() ;
    fflush (stdout) ;
}


void TrapTrace::onRunFreeClicked (void)
{
    MAC->runFree() ;
    setHalted (false) ;
}


void TrapTrace::onHaltClicked ()
{
    MAC->halt() ;
    setHalted (true) ;
}


void TrapTrace::setHalted (bool halted)
{
    m_singleStepButton->setEnabled (halted) ;
    m_runFreeButton->setEnabled (halted) ;
    m_haltButton->setEnabled ( ! halted) ;
}



void TrapTrace::onTrap0CheckBox (void)
{
    bool enabled = m_trap0_checkBox->isChecked() ;
    CFG->Set ("trap0_enable", enabled) ;
    MAC->setTrap (0, m_trap0_AddressBox->text(), enabled) ;
}


void TrapTrace::onTrap1CheckBox (void)
{
    bool enabled = m_trap1_checkBox->isChecked() ;
    CFG->Set ("trap1_enable", enabled) ;
    MAC->setTrap (1, m_trap1_AddressBox->text(), enabled) ;
}


void TrapTrace::onTrap2CheckBox (void)
{
    bool enabled = m_trap2_checkBox->isChecked() ;
    CFG->Set ("trap2_enable", enabled) ;
    MAC->setTrap (2, m_trap2_AddressBox->text(), enabled) ;
}


void TrapTrace::onTrap3CheckBox (void)
{
    bool enabled = m_trap3_checkBox->isChecked() ;
    CFG->Set ("trap3_enable", enabled) ;
    MAC->setTrap (3, m_trap3_AddressBox->text(), enabled) ;
}


void TrapTrace::onHistoryCheckBox (void)
{
    bool enabled = m_history_checkBox->isChecked() ;
//printf ("m_history_checkBox = %i\n", enabled) ;
    CFG->Set ("history_dump", enabled) ;
    MAC->enableHistory (enabled) ;
}



//-------------------  T R A C E  ---------------------






void TrapTrace::onTraceStartEdited (void)
{
printf ("onTraceStartEdited\n") ;
    editAddress (m_traceStartBox) ;
}


void TrapTrace::onTraceStartFinshed (void)
{
    QString* text = finishAddress (m_traceStartBox) ;
    CFG->Set ("trace_start_address", *text) ;
    quint16 addr = text->toUShort (nullptr, 16) ;
    MAC->setTraceStart (addr) ;
}


void TrapTrace::onTraceEndEdited (void)
{
    editAddress (m_traceEndBox) ;
}


void TrapTrace::onTraceEndFinshed (void)
{
    QString* text = finishAddress (m_traceEndBox) ;
    CFG->Set ("trace_end_address", *text) ;
    quint16 addr = text->toUShort (nullptr, 16) ;
    MAC->setTraceEnd (addr) ;
}


void TrapTrace::onTraceButtonClicked (void)
{
printf ("onTraceButtonClicked: traceIsOn=%i\n", MAC->traceIsOn()) ;
    if (MAC->traceIsOn()) {
        MAC->enableTrace (false) ;
        m_traceButton->setText ("Start Trace") ;
    } else {
        QString loString = m_traceStartBox->text() ;
        QString hiString = m_traceEndBox->text() ;
        quint16 addr = hiString.toUShort (nullptr, 16) ;
        MAC->setTraceEnd (addr) ;
        addr = loString.toUShort (nullptr, 16) ;
        MAC->setTraceStart (addr) ;
        MAC->enableTrace (true) ;
        m_traceButton->setText("STOP Trace") ;
    }
}


void TrapTrace::onSs_fetch_checkBox (void)
{
    bool flag = m_ss_fetch_checkBox->isChecked() ;
    MAC->ss_setFetchSnoop (flag) ;
}


void TrapTrace::onSs_store_checkBox (void)
{
    bool flag = m_ss_store_checkBox->isChecked() ;
    MAC->ss_setStoreSnoop (flag) ;
}

