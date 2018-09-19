#include "lizhiapi.h"
#include <QEventLoop>
#include <QDebug>
#include <QRegExp>

LiZhiAPI::LiZhiAPI(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
    request.setRawHeader("Host","www.lizhi.fm");
    request.setRawHeader("Connection","keep-alive");
    request.setRawHeader("Accept-Language","zh,zh-CN;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2");
    request.setHeader(QNetworkRequest::ContentTypeHeader,"text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    request.setHeader(QNetworkRequest::UserAgentHeader,"Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:62.0) Gecko/20100101 Firefox/62.0");
}

//获取节目类型
QVector<QStringList> LiZhiAPI::get_radio_type()
{
    const QString url = BaseUrl + "/sitemap.html";
    request.setUrl(QUrl(url));
    qDebug() << "url:" << url;
    reply = manager->get(request);
    QEventLoop loop;
    connect(manager,&QNetworkAccessManager::finished,&loop,&QEventLoop::quit);
    loop.exec();
    const QString data = reply->readAll();
    QRegExp rx("【节目类型】.*</ul>");
    rx.setMinimal(true);
    rx.indexIn(data);
    QString contentStr = rx.cap(0);
    rx.setPattern("<a href=\".*\">.*</a>");
    int pos = 0;
    QVector<QStringList> vec_list(50);
    while ((pos = rx.indexIn(contentStr,pos)) != -1)
    {
        QStringList list;
        QRegExp t_rx(">.*(?=<)");
        QString temp = rx.cap(0);
        list << temp.replace(t_rx,"");
        vec_list.append(list);
        pos += rx.matchedLength();
    }
    qDebug() << "vec_list:" << vec_list;

    return vec_list;
}
