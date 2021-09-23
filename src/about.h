

#ifndef ABOUT_H
#define ABOUT_H

#include <QLabel>
#include <QWidget>
#include <QWindow>
#include <QDialog>
#include <QPushButton>
#include <QTextBrowser>


class About : public QDialog
{
public:

    About (QWindow* parent) ;
    ~About() {} ;

private:

    void onCloseButtonClicked (void) ;
    void onUrlButtonClicked (void) ;
    int  centerText (QLabel* label) ;

    const int WIDTH = 420 ;
    const int HEIGHT = 600 ;
    
    QLabel* m_txt1 ;
    QLabel* m_txt2 ;
    QLabel* m_txt3 ;
    QLabel* m_txt4 ;

    const QString m_url = "www.gnu.org/licenses/gpl-3.0.html" ;

    QWidget*       m_panel1 ;
    QWindow*       m_panel2 ;
    QTextBrowser*  m_html ;
    QPushButton*   m_close ;
    QPushButton*   m_visit ;
} ;

#endif
