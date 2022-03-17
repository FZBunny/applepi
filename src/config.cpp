/*****************************************************************************

    File: "config.cpp"

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


#include <iostream>

#include <QDebug>
#include <QDir>
#include <QErrorMessage>

#include "config.h"
#include "defs.h"
#include "mainwindow.h"

using namespace std ;

//Config::Config()

Config::Config() : QSettings (QSettings::NativeFormat, QSettings::UserScope, "applepi", "applepi")
{
    QString defaultDir = QDir::homePath() ;
    char sep = QDir::separator().cell() ;
    m_cfgName = defaultDir ;
    m_cfgName.append (sep) ;
    m_cfgName.append (".config") ;

    QFileInfo test (m_cfgName) ;
    if (test.exists()) {                 // does directory "~/.config" exist?
        m_cfgName.append (sep) ;
        m_cfgName.append ("applepi.conf") ;   // if so, config file is "~/.config/applepi.conf"
    } else {
        m_cfgName = defaultDir ;
        m_cfgName.append (sep) ;
        m_cfgName.append (".applepi.conf") ;  // else it is "~/.applepi.conf"
    }
    
    test.setFile (m_cfgName) ;
    if ( ! test.exists()) {
        QFile newFile (m_cfgName) ;
        newFile.open (QIODevice::WriteOnly) ;
        newFile.close() ;
    }

    m_settings = new QSettings (m_cfgName, QSettings::NativeFormat) ;

    QString checkValue ;
    checkValue = m_settings->value ("check-key", "oops").value<QString>() ;

    if (checkValue == "oops") {
        QString checkValue ("DON'T DELETE THIS KEY") ;
        QString yes("yes") ;
        QString zeros("0000") ;
        QString ffff ("FFFF") ;
        QString tmpStr ;

        Set ("check-key",  checkValue) ;
        Set ("directory_for_slot1_print", defaultDir) ;
        Set ("disassemble_start_address", zeros) ;
        Set ("disassemble_end", zeros) ;
//        Set ("disassemble_mem_type", uint(1)) ;
//        Set ("disassemble_end_criterion", uint(1)) ;
        Set ("floppy1_path", defaultDir) ;
        Set ("floppy2_path", defaultDir) ;
        tmpStr = "/dev/input/mouse0" ;
        Set ("game_controller_name", tmpStr) ;
        Set ("game_controller_id", (uint)0) ;
        Set ("hd1_volume_path", defaultDir) ;
        Set ("hd2_volume_path", defaultDir) ;
        Set (QString("help_size"), QSize(420,500)) ;
        Set (QString("help_position"), QPoint(10,10)) ;
        Set ("internal_rom_number", (uint)3) ;
        Set ("rom_path", defaultDir) ;
        Set ("speaker_volume", (uint)20) ;
        Set ("tape_path",    defaultDir) ;
        Set ("tape_write_protect",    (uint)0) ;
        Set ("text_echo_path", defaultDir) ;
        Set ("trace_start_address", zeros) ;
        Set ("trace_end_address",   ffff) ;
        tmpStr = "0" ;
        Set ("trap_history_lines", tmpStr) ;
        Set ("trap_history_dump", (uint)0) ;
        Set ("trap0_address", zeros) ;
        Set ("trap1_address", zeros) ;
        Set ("trap2_address", zeros) ;
        Set ("trap3_address", zeros) ;
        Set ("trap0_enable",  (uint)0) ;
        Set ("trap1_enable",  (uint)0) ;
        Set ("trap2_enable",  (uint)0) ;
        Set ("trap3_enable",  (uint)0) ;
        Set ("text_echo_path", defaultDir) ;
        Set ("use_internal_rom", yes) ;
        tmpStr = "10,10" ;
        Set ("window_position", tmpStr) ;
        Set ("window_scale", (uint)1) ;

        m_settings->sync() ;
    }

}


bool Config::keyExists (QString key) 
{
    bool ok = m_settings->contains(key) ;

    if ( ! ok) {
        QString errMsg ;
        QTextStream (&errMsg) << "The configuration file " << m_cfgName 
                              << " doesn't contain the key \"" << key << "\"." ;

        qStdOut() << endl << "*** Warning: " << errMsg << endl << endl ;

 //       QErrorMessage* box = new QErrorMessage (main_window) ;
 //       box->showMessage (errMsg) ;
    }
    return ok ;
}


void Config::Set (const QString& key, QSize s)
{
    QString value ;
    QTextStream(&value) << s.width() << "," <<  s.height() ;
// qDebug() << "set 1 key=" << key << "; value=" << value ;
    m_settings->setValue (key, value) ;
    m_settings->sync() ;
}


void Config::Set (const QString& key, QPoint pos)
{
    QString value ;
    QTextStream(&value) << pos.x() << "," <<  pos.y() ;
// qDebug() << "set 2 key=" << key << "; value=" << value ;
    m_settings->setValue (key, value) ;
    m_settings->sync() ;
}


void Config::Set (const QString& key, bool value)
{
    m_settings->setValue (key, value) ;
// qDebug() << "set 3 key=" << key << "; value=" << value ;
    m_settings->sync() ;
}


void Config::Set (const QString& key, uint value)
{ 
    m_settings->setValue (key, value) ;
// qDebug() << "set 4 key=" << key << "; value=" << value ;
    m_settings->sync() ;
}


void Config::Set (const QString& key, QString& value)
{
    m_settings->setValue (key, value) ;
//qDebug() << "set 5 key=" << key << "; value=" << value ;
    m_settings->sync() ;
}


void Config::Set (const QString& key, char* value)
{
    m_settings->setValue (key, value) ;
//qDebug() << "set 5 key=" << key << "; value=" << value ;
    m_settings->sync() ;
}


// ---------------------------------------------------


bool Config::Get (const QString& key, QString* value)
{
    if (keyExists(key) == false) return false ;
    static QString def ("No Such Key") ;
    *value = m_settings->value (key,def).value<QString>() ;
    return true ;
}


bool Config::Get (const QString& key, bool *value)
{
    if (keyExists(key) == false) return false ;
    *value = m_settings->value (key).value<bool>() ;
    return true ;
}


bool Config::Get (const QString& key, uint* value)
{
    if (keyExists(key) == false) return false ;
    *value = m_settings->value (key).value<uint>() ;
    return true ;
}


bool Config::Get (const QString& key, QPoint *pos)
{
    if (keyExists(key) == false) return false ;

    QString buffer ;
    buffer = m_settings->value (key).value<QString>() ;
    QStringList list = buffer.split (",") ;

    if (list.length() < 2) {
        QString tmpStr ("10,10") ;
        Set (key, tmpStr) ;
         pos->setX (10) ;
         pos->setY (10) ;
    } else {
        uint tmp ;
        tmp = list.at(0).toUInt() ; 
        pos->setX (tmp) ;
        tmp = list.at(1).toUInt() ;
        pos->setY (tmp) ;
    }

    return true ;
}


bool Config::Get (const QString& key, QSize *size)
{  
    if (keyExists(key) == false) return false ;

    QString  buffer ;
    buffer = m_settings->value (key).value<QString>() ;
    QStringList list = buffer.split (",") ;

    if (list.length() < 2) {
        QString tmpStr ("10,10") ;
        Set (key, tmpStr) ;
        size->setWidth (100) ;
        size->setHeight (100) ;
    } else {
        uint tmp ;
        tmp = list.at(0).toUInt() ;
        size->setWidth (tmp) ;
        tmp = list.at(1).toUInt() ;
        size->setHeight (tmp) ;
    }

    return true ;
}

