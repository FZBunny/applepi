
/******************************************************************

    This code has been temporarily abandoned due to missing 
    'QWebEngineView' support in Pi OS.

******************************************************************/


//#include <QWidget>
#include <QDialog>
#include <QString>
#include <QWebEngineView>
//#include <QPushButton>


class Help : public QDialog
{

public:

    Help (MainWindow *parent) ;

private:

    void resizeEvent (QResizeEvent *e) ;
//    void updateWindowTitle (void) ;

    MainWindow*     m_parent ;
    QWebEngineView* m_browser ;
    QString         m_index ;

} ;
