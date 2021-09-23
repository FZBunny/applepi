

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

    void Get (const QString& key, QString* value) ;
    void Get (const QString& key, bool *value) ;
    void Get (const QString& key, QSize *size) ;
    void Get (const QString& key, QPoint *position) ;
    void Get (const QString& key, uint *value) ;

private:

    QSettings* m_settings ;
    QString    m_cfgName ;

    bool keyExists (QString key) ;

} ;

#endif // CONFIG_H
