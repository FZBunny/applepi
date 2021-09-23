

#include <QPushButton>
#include <QKeyEvent>


#include "applepi_button.h"

ApplepiButton::ApplepiButton (const QIcon &icon, const QString &text, QWidget *parent)
             : QPushButton   (icon, text, parent)
{}


ApplepiButton::ApplepiButton (const QString &text, QWidget *parent)
             : QPushButton   (text, parent)
{}


void ApplepiButton::keyPressEvent(QKeyEvent *e)
{
    e->ignore() ;
}

