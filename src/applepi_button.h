

#include <QPushButton>

class ApplepiButton : public QPushButton
{  
public:
    ApplepiButton (const QIcon &icon, const QString &text, QWidget *parent) ;
    ApplepiButton (const QString &text, QWidget *parent) ;

private:
    void keyPressEvent(QKeyEvent *e) ;
} ;
