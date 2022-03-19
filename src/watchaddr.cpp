/*****************************************************************************

    File: "watchaddr.cpp"

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

#include "watchaddr.h"
#include "machine.h"


WatchAddr::WatchAddr (MainWindow* parent) :  QDialog (parent)
{
    this->setWindowTitle ("Watch Memory") ;
    
    MAC->enableTrace (false) ;
    
    m_parent = parent ;
    int width = 650 ;
    int height = 235 ;
    setMinimumSize (width,height) ;
    setMaximumSize (width,height) ;

    int x, y ;
    CENTER_DIALOG
    
    this->setAttribute (Qt::WA_DeleteOnClose) ;

    MAC->setWatchAddrDialogPointer (this) ;  // (machine has to know how to tell us a watch has been hit)

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

    QLabel* lab1 = new QLabel ("W A T C H   A D R E S S", this) ;
    lab1->move (100,5) ;
    lab1->resize (70,20) ;

    int watch_X = 20 ;
    int watch_Y = 0 ;

    int watchCheckbox_X = watch_X + 55 ;
    int watchCheckbox_Y = watch_Y + 21 ;

    int watchButtonX = watch_X + 170 ;
    int watchButtonY = 20 ;

    QSize addressBoxSize (50,24) ;

    QFrame* box1 = new QFrame (this, Qt::Widget) ;
    box1->move (10,30) ;
    box1->resize (300, 188) ;
    box1->setFrameStyle (QFrame::Panel | QFrame::Raised) ;
    box1->setLineWidth(3) ;
    box1->show() ;

    QFrame* box2 = new QFrame (this, Qt::Widget) ;
    box2->move (335,30) ;
    box2->resize (300, 188) ;
    box2->setFrameStyle (QFrame::Panel | QFrame::Raised) ;
    box2->setLineWidth(3) ;
    box2->show() ;

    m_singleStepButton = new ApplepiButton ("Single Step", box1) ;
    m_runFreeButton    = new ApplepiButton ("Continue", box1) ;
    m_haltButton       = new ApplepiButton ("- HALT -", box1) ;

    m_singleStepButton->move (watchButtonX, watchButtonY) ;
    m_singleStepButton->resize (buttonSize) ;
    m_runFreeButton->move (watchButtonX, watchButtonY+40) ;
    m_runFreeButton->resize (buttonSize) ;
    m_haltButton->move (watchButtonX, watchButtonY+80) ;
    m_haltButton->resize (buttonSize) ;

    m_watch0_AddressBox = new QLineEdit ("0000", box1) ;
    m_watch0_AddressBox->resize (addressBoxSize) ;
    m_watch0_AddressBox->move (watch_X,watch_Y+20) ;
    m_watch0_AddressBox->setAlignment (Qt::AlignCenter) ;
    m_watch0_AddressBox->setValidator (m_hexValidator) ;

    m_watch1_AddressBox = new QLineEdit ("0000", box1) ;
    m_watch1_AddressBox->resize (addressBoxSize) ;
    m_watch1_AddressBox->move (watch_X,watch_Y+50) ;
    m_watch1_AddressBox->setAlignment (Qt::AlignCenter) ;
    m_watch1_AddressBox->setValidator (m_hexValidator) ;

    m_watch2_AddressBox = new QLineEdit ("0000", box1) ;
    m_watch2_AddressBox->resize (addressBoxSize) ;
    m_watch2_AddressBox->move (watch_X,watch_Y+80) ;
    m_watch2_AddressBox->setAlignment (Qt::AlignCenter) ;
    m_watch2_AddressBox->setValidator (m_hexValidator) ;

    m_watch3_AddressBox = new QLineEdit ("0000", box1) ;
    m_watch3_AddressBox->resize (addressBoxSize) ;
    m_watch3_AddressBox->move (watch_X,watch_Y+110) ;
    m_watch3_AddressBox->setAlignment (Qt::AlignCenter) ;
    m_watch3_AddressBox->setValidator (m_hexValidator) ;

    m_watch0_checkBox = new QCheckBox ("1 Enable", box1) ;
    m_watch0_checkBox->move (watchCheckbox_X,watchCheckbox_Y) ;

    m_watch1_checkBox = new QCheckBox ("2 Enable", box1) ;
    m_watch1_checkBox->move (watchCheckbox_X,watchCheckbox_Y+30) ;
    
    m_watch2_checkBox = new QCheckBox ("3 Enable", box1) ;
    m_watch2_checkBox->move (watchCheckbox_X,watchCheckbox_Y+60) ;

    m_watch3_checkBox = new QCheckBox ("4 Enable", box1) ;
    m_watch3_checkBox->move (watchCheckbox_X,watchCheckbox_Y+90) ;

    m_history_checkBox = new QCheckBox ("Dump", box1) ;
    m_history_checkBox->move (watch_X+20,watch_Y+150) ;

    m_requestedDumpLinesBox = new QLineEdit ("0", box1) ;
    m_requestedDumpLinesBox->resize (70,24) ;
    m_requestedDumpLinesBox->move (watch_X+85,watch_Y+150) ;
    m_requestedDumpLinesBox->setAlignment (Qt::AlignRight) ;
    m_requestedDumpLinesBox->setValidator (m_decimalValidator) ;

    QLabel* lab3 = new QLabel ("Lines (decimal)", this) ;
    lab3->move (watch_X+170,watch_Y+170) ;
    lab3->resize (100,40) ;

    getBoolConfigData ((char*)"watch0_enable", m_watch0_checkBox) ;
    getBoolConfigData ((char*)"watch1_enable", m_watch1_checkBox) ;
    getBoolConfigData ((char*)"watch2_enable", m_watch2_checkBox) ;
    getBoolConfigData ((char*)"watch3_enable", m_watch3_checkBox) ;
    getBoolConfigData ((char*)"watch_history_dump", m_history_checkBox) ;

    get16bitConfigData ((char*)"watch0_address", m_watch0_AddressBox) ;
    get16bitConfigData ((char*)"watch1_address", m_watch1_AddressBox) ;
    get16bitConfigData ((char*)"watch2_address", m_watch2_AddressBox) ;
    get16bitConfigData ((char*)"watch3_address", m_watch3_AddressBox) ;

    connect (m_singleStepButton, &ApplepiButton::clicked, this, &WatchAddr::onSingleStepClicked) ;
    connect (m_runFreeButton,    &ApplepiButton::clicked, this, &WatchAddr::onRunFreeClicked) ;
    connect (m_haltButton,       &ApplepiButton::clicked, this, &WatchAddr::onHaltClicked) ;

    connect (m_watch0_checkBox,   &QCheckBox::stateChanged, this, &WatchAddr::onWatch0CheckBox) ;
    connect (m_watch1_checkBox,   &QCheckBox::stateChanged, this, &WatchAddr::onWatch1CheckBox) ;
    connect (m_watch2_checkBox,   &QCheckBox::stateChanged, this, &WatchAddr::onWatch2CheckBox) ;
    connect (m_watch3_checkBox,   &QCheckBox::stateChanged, this, &WatchAddr::onWatch3CheckBox) ;

    connect (m_history_checkBox, &QCheckBox::stateChanged, this, &WatchAddr::onHistoryCheckBox) ;

    connect (m_watch0_AddressBox, &QLineEdit::textEdited, this,  &WatchAddr::onWatch0Edited) ;
    connect (m_watch1_AddressBox, &QLineEdit::textEdited, this,  &WatchAddr::onWatch1Edited) ;
    connect (m_watch2_AddressBox, &QLineEdit::textEdited, this,  &WatchAddr::onWatch2Edited) ;
    connect (m_watch3_AddressBox, &QLineEdit::textEdited, this,  &WatchAddr::onWatch3Edited) ;

    connect (m_watch0_AddressBox, &QLineEdit::editingFinished, this,  &WatchAddr::onWatch0Finshed) ;
    connect (m_watch1_AddressBox, &QLineEdit::editingFinished, this,  &WatchAddr::onWatch1Finshed) ;
    connect (m_watch2_AddressBox, &QLineEdit::editingFinished, this,  &WatchAddr::onWatch2Finshed) ;
    connect (m_watch3_AddressBox, &QLineEdit::editingFinished, this,  &WatchAddr::onWatch3Finshed) ;

    connect (m_requestedDumpLinesBox, &QLineEdit::editingFinished, this,  &WatchAddr::onLinesToDumpEntered) ;
    
    QString addr ;
    addr = m_watch0_AddressBox->text() ;
    MAC->setWatch (0, addr, m_watch0_checkBox->isChecked()) ;
    addr = m_watch1_AddressBox->text() ;
    MAC->setWatch (1, addr, m_watch1_checkBox->isChecked()) ;
    addr = m_watch2_AddressBox->text() ;
    MAC->setWatch (2, addr, m_watch2_checkBox->isChecked()) ;
    addr = m_watch3_AddressBox->text() ;
    MAC->setWatch (3, addr, m_watch3_checkBox->isChecked()) ;

    QString  tmpString ;
    CFG->Get ("watch_history_lines", &tmpString) ;

    m_requestedDumpLinesBox->selectAll() ;
    m_requestedDumpLinesBox->insert (tmpString) ;
    MAC->setLines2Dump (tmpString) ;

    MAC->enableHistory (m_history_checkBox->isChecked()) ;

    setHalted (false) ;

}


void WatchAddr::get16bitConfigData (char* key, QLineEdit* box)
{
    QString tmpString ;
    CFG->Get ((char*)key, &tmpString) ;
    box->setText (tmpString) ;
}


void WatchAddr::getBoolConfigData (char* key, QCheckBox* box)
{
    bool  value ;
    CFG->Get (key, &value) ;
    box->setChecked (value) ;
}


// Clear all machine watchs and disable instruction tracing when this dialog is dismissed.

void WatchAddr::reject (void)
{
//printf ("void WatchAddr::reject\n") ;
    m_parent->watchAddrIsStopping() ;

    for (int i=0; i<3; i++) {
        MAC->setWatch (i, QString("0"), 0) ;
    }
    MAC->enableTrace (false) ;
    MAC->runFree() ;

    QDialog::reject() ;
}


// This function is overridden to ignore keystrokes which   XXXXXXXXXXXXXXXXXXXXX   Remove ? XXXXXXXXXXXXXXXXX
// would otherwise be seen by the buttons as 'click' events.
// That would be bad.

void WatchAddr::keyPressEvent (QKeyEvent *)
{
//printf ("keyPressEvent\n") ;
}


void WatchAddr::editAddress (QLineEdit* box)
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


QString* WatchAddr::finishAddress (QLineEdit* box)
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


//---------------------------------------------------


void WatchAddr::onWatch0Finshed (void)
{
    QString* text = finishAddress (m_watch0_AddressBox) ;
    CFG->Set ("watch0_address", *text) ;
    if (m_watch0_checkBox->isChecked()) MAC->setWatch (0, *text, true) ;
}


void WatchAddr::onWatch1Finshed (void)
{
    QString* text = finishAddress (m_watch1_AddressBox) ;
    CFG->Set ("watch1_address", *text) ;
    if (m_watch0_checkBox->isChecked()) MAC->setWatch (1, *text, true) ;
}


void WatchAddr::onWatch2Finshed (void)
{
    QString* text = finishAddress (m_watch2_AddressBox) ;
    CFG->Set ("watch2_address", *text) ;
    if (m_watch0_checkBox->isChecked()) MAC->setWatch (2, *text, true) ;
}


void WatchAddr::onWatch3Finshed (void)
{
    QString* text = finishAddress (m_watch3_AddressBox) ;
    CFG->Set ("watch3_address", *text) ;
    if (m_watch0_checkBox->isChecked()) MAC->setWatch (3, *text, true) ;
}


void WatchAddr::onWatch0Edited (void)
{
    editAddress (m_watch0_AddressBox) ;
}


void WatchAddr::onWatch1Edited (void)
{
    editAddress (m_watch1_AddressBox) ;
}


void WatchAddr::onWatch2Edited (void)
{
    editAddress (m_watch2_AddressBox) ;
}


void WatchAddr::onWatch3Edited (void)
{
    editAddress (m_watch3_AddressBox) ;
}


void WatchAddr::onLinesToDumpEntered (void)
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
    CFG->Set ("watch_history_lines", str) ;
}


void WatchAddr::onSingleStepClicked (void)
{
    MAC->singleStep() ;
    fflush (stdout) ;
}


void WatchAddr::onRunFreeClicked (void)
{
    MAC->runFree() ;
    setHalted (false) ;
}


void WatchAddr::onHaltClicked ()
{
    MAC->halt() ;
    setHalted (true) ;
}


void WatchAddr::setHalted (bool halted)
{
    m_singleStepButton->setEnabled (halted) ;
    m_runFreeButton->setEnabled (halted) ;
    m_haltButton->setEnabled ( ! halted) ;
}



void WatchAddr::onWatch0CheckBox (void)
{
    bool enabled = m_watch0_checkBox->isChecked() ;
    CFG->Set ("watch0_enable", enabled) ;
    MAC->setWatch (0, m_watch0_AddressBox->text(), enabled) ;
}


void WatchAddr::onWatch1CheckBox (void)
{
    bool enabled = m_watch1_checkBox->isChecked() ;
    CFG->Set ("watch1_enable", enabled) ;
    MAC->setWatch (1, m_watch1_AddressBox->text(), enabled) ;
}


void WatchAddr::onWatch2CheckBox (void)
{
    bool enabled = m_watch2_checkBox->isChecked() ;
    CFG->Set ("watch2_enable", enabled) ;
    MAC->setWatch (2, m_watch2_AddressBox->text(), enabled) ;
}


void WatchAddr::onWatch3CheckBox (void)
{
    bool enabled = m_watch3_checkBox->isChecked() ;
    CFG->Set ("watch3_enable", enabled) ;
    MAC->setWatch (3, m_watch3_AddressBox->text(), enabled) ;
}


void WatchAddr::onHistoryCheckBox (void)
{
    bool enabled = m_history_checkBox->isChecked() ;
//printf ("m_history_checkBox = %i\n", enabled) ;
    CFG->Set ("watch_history_dump", enabled) ;
    MAC->enableHistory (enabled) ;
}

