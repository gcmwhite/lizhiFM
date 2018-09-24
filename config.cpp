#include "config.h"
#include <QFile>
#include <QDataStream>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

Config::Config(QObject *parent) : QObject(parent)
{

}

//新建配置
void Config::create_config(const QJsonDocument &json)
{
    QFile file("./config/config.dat");
    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Can`t open file for writing!";
        return ;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_DefaultCompiledVersion);
    out << json;
    file.flush();
    file.close();
}

//读取配置
QJsonDocument Config::read_config()
{
    QFile file("./config/config.dat");
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Can`t open file for reading!";
        return ;
    }
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_DefaultCompiledVersion);
    const QJsonDocument json;
    in >> json;
    file.close();
    return json;
}

//新建播放列表
void Config::create_list(const QJsonDocument &json)
{
    QSqlDatabase database;
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("fm_play_list.db");
    if (!database.open())
    {
        qDebug() << "Error: Failed to connect database." << database.lastError();
        return ;
    }
    QSqlQuery sql_query;
    bool success = sql_query.exec("create table play_list"
                                  "(id int primary key,"
                                  "music_id varchar,"
                                  "music_name varchar)");
    if (success)
        qDebug() << "创建数据库表成功！";
    else
        qDebug() << "创建数据库表失败！";
}

//读取播放列表
void Config::read_list()
{

}
