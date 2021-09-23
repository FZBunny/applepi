

#include <QtCore/QThread>

#include "mainwindow.h"
#include "config.h"

class MainWindow ;


class Paste : public QThread
{
public:
    Paste (MainWindow* parent, QString string) ;
    void run (void) ;

private:
    MainWindow* m_parent ;
    QString     m_string ;

} ;
