
#ifndef MISC_DEFS_H
#define MISC_DEFS_H

#include <QtGlobal>
#include <QTextStream>
#include <QWindow>
#include <QDialog>

#include "mainwindow.h"


QTextStream& qStdOut() ;        // A Qt-friendly replacement for 'cout'


#define CFG main_window->m_cfg
#define MAC main_window->m_mac


extern "C" void trim (char* buffer, int len) ;
//extern "C" void centerDialog (MainWindow* parent, QDialog* dialog, int width, int height) ;

#define SHARE_DIR "/usr/share/applepi/"

#define  MIN_MAINWINDOW_WIDTH  577
#define  MIN_MAINWINDOW_HEIGHT 580
//#define  MIN_MAINWINDOW_HEIGHT 600

#define  MAX_MAINWINDOW_WIDTH  1138
#define  MAX_MAINWINDOW_HEIGHT 968
//#define  MAX_MAINWINDOW_HEIGHT 989

#define  SCREEN_PIXELS_WIDTH  564
#define  SCREEN_PIXELS_HEIGHT 388

#define SECTORSIZE 256
#define BLOCKSIZE  512

#define DOS3 0
#define PRODOS 1

#define CENTER_DIALOG \
  { \
    resize (width,height) ; \
    QPoint pPos = parent->pos() ; \
    QSize pSize = parent->size() ; \
    x = ((pPos.x()+pSize.width()/2) - width/2) ; \
    y = ((pPos.y()+pSize.height()/2) - height/2) ; \
    move (x, y) ; \
  }

enum romNumber {
    APPLE2,
    APPLE2PLUS,
    APPLE2E,
    APPLE2E_ENHANCED,
    APPLE_UNKNOWN
} ;


#endif // MISC_DEFS_H
