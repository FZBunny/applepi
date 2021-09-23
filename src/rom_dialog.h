
#ifndef ROM_DIALOG_H
#define ROM_DIALOG_H

#include <QDialog>
#include <QFrame>
#include <QRadioButton>
#include <QLabel>
#include <QString>

#include "mainwindow.h"


class RomDialog : public QDialog
{

public:

    RomDialog (MainWindow* parent) ;
    ~RomDialog(){} ;

private:

    void onApple2Original  (void) ;
    void onApple2e         (void) ;
    void onApple2Plus      (void) ;
    void onApple2eEnhanced (void) ;
    void onUseExternalROM  (void) ;
    void onSelectRomFile   (void) ;
    void onCancel          (void) ;
    void onDone            (void) ;

    void centerFileName (void) ;

    MainWindow*    m_parent ;
    QFrame*        m_frame ;
    QLabel*        m_text ;
    QLabel*        m_romFileName ;
    QRadioButton*  m_apple2 ;
    QRadioButton*  m_apple2Plus ;
    QRadioButton*  m_apple2e ;
    QRadioButton*  m_apple2eEnhanced ;
    QRadioButton*  m_externalFile ;

    QPushButton*   m_fileButton ;
    QPushButton*   m_cancelButton ;
    QPushButton*   m_doneButton ;

    QString  m_romPath ;
    QString  m_useInternalRom ;
    uint     m_internalRomNumber ;

    int      m_buttonX, m_buttonY ;
} ;

#endif
