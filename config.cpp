#include "config.h"
#include <QFile>
#include <QDataStream>
#include <QFileInfo>
#include <QJsonArray>
#include <QDebug>
#include <QJsonDocument>
#include <QStandardPaths>
#include <QDir>

Config::Config(QObject *parent) : QObject(parent)
{
    QString userPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QString userName = userPath.section("/", -1, -1);

#ifdef Q_OS_LINUX
    path = QString("/home/%1/.config/lizhiFM/").arg(userName);
    QDir dir;
    if (!dir.exists(path))
    {
        bool ok = dir.mkpath(path);
        qDebug() << "create path:" << path << ":" << ok;
    }

    path = path + "config.dat";

#endif

#ifdef Q_OS_WIN
    path = "./config.dat";
#endif
}

//新建配置
void Config::create_config(const QJsonObject &json)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Can`t open configuration file for writing!";
        file.close();
        return ;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_DefaultCompiledVersion);
    const QByteArray json_byte_array = QJsonDocument(json).toJson();
    out << json_byte_array;
    file.flush();
    file.close();
}

//读取配置
QJsonObject Config::read_config()
{
    QFile file(path);
    QJsonObject json;
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Can`t open configuration file for reading!";
        return json;
    }
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_DefaultCompiledVersion);
    QByteArray json_byte_array;
    in >> json_byte_array;
    file.close();
    json = QJsonDocument::fromJson(json_byte_array).object();
//    qDebug() << "read json:" << json;
    return json;
}

