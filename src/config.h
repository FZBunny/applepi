/*****************************************************************************

    File: "config.h"

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



#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QRect>
#include <QSettings>
#include <QDir>
#include <QTextStream>
#include <QStandardPaths>


class Config : public QSettings
{
public:

    Config() ;
    ~Config() {} ;

    void Set (const QString& key, QString& value) ;
    void Set (const QString& key, uint value) ;
    void Set (const QString& key, bool value) ;
    void Set (const QString& key, QSize size) ;
    void Set (const QString& key, QPoint position) ;
    void Set (char* key,          QSize size) ;
    void Set (char* key,          QPoint position) ;
    void Set (const QString& key, char* value) ;

    bool Get (const QString& key, QString* value) ;
    bool Get (const QString& key, bool *value) ;
    bool Get (const QString& key, QSize *size) ;
    bool Get (const QString& key, QPoint *position) ;
    bool Get (const QString& key, uint *value) ;

private:

    QSettings* m_settings ;
    QString    m_cfgName ;

    bool keyExists (QString key) ;

} ;

#endif // CONFIG_H
