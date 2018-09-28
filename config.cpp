#include "config.h"
#include <QFile>
#include <QDataStream>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QFileInfo>
#include <QJsonArray>
#include <QDebug>
#include <QJsonDocument>

Config::Config(QObject *parent) : QObject(parent)
{

}

//新建配置
void Config::create_config(const QJsonObject &json)
{
    QFile file("config.dat");
    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Can`t open file for writing!";
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
    QJsonObject json;
    QFile file("config.dat");
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Can`t open file for reading!";
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

//新建播放列表
void Config::create_list(const QVector<QStringList> &vec_list)
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
    sql_query.exec("select name from sqlite_master where name='play_list'");
    sql_query.first();
    const bool is_play_list = sql_query.value("name").toString() == "play_list";
//    qDebug() << "is_play_list:" << is_play_list;
    if (is_play_list)
    {
        sql_query.exec("drop table play_list");
    }
    sql_query.exec("create table play_list(id int primary key,music_id varchar,music_name varchar)");
    sql_query.prepare("insert into play_list values(?,?,?)");
    for (int i = 0;i < vec_list.size();i++)
    {
        QStringList list = vec_list.at(i);
        const QString music_id = list.at(0);
        const QString music_name = list.at(1);
        sql_query.bindValue(0,i);
        sql_query.bindValue(1,music_id);
        sql_query.bindValue(2,music_name);
        sql_query.exec();
//        bool ok = sql_query.exec();
//        if (ok)
//            qDebug() << "插入成功!" << i;
//        else
//            qDebug() << "插入失败！" << i;
    }
    database.close();
}

//读取播放列表
QVector<QStringList> Config::read_list()
{
    QVector<QStringList> vec_list;

    QSqlDatabase database;
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("fm_play_list.db");
    if (!database.open())
    {
        qDebug() << "Error: Failed to connect database." << database.lastError();
        return vec_list;
    }
    QSqlQuery sql_query;
    sql_query.exec("select *from play_list");
    while (sql_query.next())
    {
//        int id = sql_query.value(0).toInt();
        const QString music_id = sql_query.value(1).toString();
        const QString music_name = sql_query.value(2).toString();
        QStringList list;
        list << music_id << music_name;
        vec_list.append(list);
    }
    database.close();
//    qDebug() << "vec_list co:" << vec_list;
    return vec_list;
}
