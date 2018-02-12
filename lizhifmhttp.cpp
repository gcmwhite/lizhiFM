#include "lizhifmhttp.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QFileInfo>

LizhiFMHttp::LizhiFMHttp(QObject *parent) : QObject(parent)
{

}

void LizhiFMHttp::urlSlot(QString url)       //获取网页内容
{
    qDebug() << "urlSlot" << url;
    QNetworkRequest req;
    req.setUrl(QUrl(url));
    req.setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/63.0.3239.132 Safari/537.36");
    req.setRawHeader("Referer","http://www.lizhi.fm/");
    req.setRawHeader("Host","www.lizhi.fm");
    req.setRawHeader("Connection","keep-alive");
    req.setRawHeader("Accept-Language","zh-CN,zh");
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager,&QNetworkAccessManager::finished,this,[=](QNetworkReply *reply){
        QByteArray read(reply->readAll());
        QString html = QString::fromUtf8(read);
        emit htmlSignal(url,html);
        reply->deleteLater();
    });
    manager->get(req);
}

void LizhiFMHttp::downloadSlot(QString url)      //下载文件
{
    qDebug() << "downslot" << url;
    QDir fileDir;
    if (!fileDir.exists("imges"))
    {
        fileDir.mkdir("imges");
    }
    QFileInfo info(QUrl(url).path());
    QString fileName(info.fileName());
    QString filePath(fileDir.path()+QString("/imges/")+fileName);

    /*
    info.setFile(filePath);
    if (info.isFile())
    {
        qDebug() << "文件存在";
        emit imgDownloadFinishedSiganl(filePath);
        return;
    }
    */

    QFile *file = new QFile(filePath);
    if (!file->open(QIODevice::WriteOnly))
    {
        qDebug() << "file open error";
        delete file;
        file = 0;
        return;
    }
    QNetworkRequest req;
    req.setUrl(QUrl(url));
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkReply *reply = manager->get(req);
    connect(reply,&QNetworkReply::readyRead,this,[=](){
        if (file) file->write(reply->readAll());
    });
    connect(reply,&QNetworkReply::finished,this,[=](){
        file->flush();
        file->close();
        reply->deleteLater();
        delete file;
        emit imgDownloadFinishedSiganl(filePath);
//        qDebug() << "下载完成：" << fileName;
    });
}

QString LizhiFMHttp::fileExist(QString url)
{
    QDir fileDir;
    QFileInfo info(QUrl(url).path());
    QString fileName(info.fileName());
    QString filePath(fileDir.path()+QString("/imges/")+fileName);
    info.setFile(filePath);
    if (info.isFile())
    {
        return filePath;
    }
    return "";
}
