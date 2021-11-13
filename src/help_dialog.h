
#include <QWidget>
#include <QString>
#include <QTextBrowser>
#include <QPushButton>
//#include <>
//#include <>
//#include <>


class Help : public QDialog
{

public:

    Help (MainWindow *parent) ;

    static void showPage(const QString &page) ;

private:

    void onHome (void) ;
    void onBack (void) ;
    void onClose (void) ;

    void resizeEvent (QResizeEvent *e) ;
//    void updateWindowTitle (void) ;

    MainWindow*   m_parent ;
    QTextBrowser* m_browser ;
    QPushButton*  m_homeButton ;
    QPushButton*  m_backButton ;
    QPushButton*  m_closeButton ;

    QString       m_index ;
} ;
