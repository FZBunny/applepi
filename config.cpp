
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

        Set ("check-key",  checkValue) ;
        QString tmpStr ("10,10") ;
        Set ("window_position", tmpStr) ;
//        Set (QString("window_size"),     QSize(MIN_MAINWINDOW_WIDTH,MIN_MAINWINDOW_HEIGHT)) ;
        Set (QString("window_scale"), (uint)1) ;
        Set ("use_internal_rom", yes) ;
        Set ("internal_rom_number", (uint)2) ;
        Set ("rom_path", defaultDir) ;
        Set ("floppy1_path", defaultDir) ;
        Set ("floppy2_path", defaultDir) ;
        Set ("hd1_volume_path", defaultDir) ;
        Set ("hd2_volume_path", defaultDir) ;
        Set ("tape_path",    defaultDir) ;
        Set ("text_echo_path", defaultDir) ;
        Set ("tape_write_protect",    (uint)0) ;
        Set ("trace_start_address", zeros) ;
        Set ("trace_end_address",   ffff) ;
        Set ("trap0_address", zeros) ;
        Set ("trap1_address", zeros) ;
        Set ("trap2_address", zeros) ;
        Set ("trap3_address", zeros) ;
        Set ("trap0_enable",  (uint)0) ;
        Set ("trap1_enable",  (uint)0) ;
        Set ("trap2_enable",  (uint)0) ;
        Set ("trap3_enable",  (uint)0) ;
        tmpStr = "0" ;
        Set ("lines2dump", tmpStr) ;
        Set ("text_echo_path", defaultDir) ;
        Set ("history_dump", (uint)0) ;
        Set ("disassemble_start_address", zeros) ;
        Set ("disassemble_end", zeros) ;
        Set ("speaker_volume", (uint)20) ;

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


void Config::Get (const QString& key, QString* value)
{
    if (keyExists(key) == false) return ;
    static QString def ("No Such Key") ;
    *value = m_settings->value (key,def).value<QString>() ;
}


void Config::Get (const QString& key, bool *value)
{
    if (keyExists(key) == false) return ;
    *value = m_settings->value (key).value<bool>() ;
}


void Config::Get (const QString& key, uint* value)
{
    if (keyExists(key) == false) return ;
    *value = m_settings->value (key).value<uint>() ;
}


void Config::Get (const QString& key, QPoint *pos)
{
    if (keyExists(key) == false) return ;

    QString buffer ;
    
    buffer = m_settings->value (key).value<QString>() ;
    QStringList list = buffer.split (",") ;

    uint tmp ;
    tmp = list.at(0).toUInt() ; 
    pos->setX (tmp) ;
    tmp = list.at(1).toUInt() ;
    pos->setY (tmp) ;
}


void Config::Get (const QString& key, QSize *size)
{  
    if (keyExists(key) == false) return ;

    QString  buffer ;

    buffer = m_settings->value (key).value<QString>() ;
    QStringList list = buffer.split (",") ;

    uint tmp ;
    tmp = list.at(0).toUInt() ; 
    size->setWidth (tmp) ;
    tmp = list.at(1).toUInt() ;
    size->setHeight (tmp) ;
}

