
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include <QWindow>
#include <QDialog>


#include "defs.h"

/*
void centerDialog (MainWindow* parent, QDialog* d, int width, int height)
{
    d->resize (width,height) ;
    QPoint pPos = parent->pos() ;
    QSize pSize = parent->size() ;
    int x = ((pPos.x()+pSize.width()/2) - width/2) ;
    int y = ((pPos.y()+pSize.height()/2) - height/2) ;
	d->move (x,y) ;
}
*/

// Trim trailing spaces from a character string

void trim (char* buffer, int len)
{
    int i ;
    for (i=len-1; i>=0; i--) {
        if (buffer[i] != 0x20) break ;
    }
    buffer[i+1] = 0 ;

}


// A Qt-friendly replacement for 'cout'

QTextStream& qStdOut()
{
    static QTextStream ts (stdout) ;
    return ts;
}

