#include "lizhiapi.h"
#include <QEventLoop>
#include <QDebug>
#include <QRegExp>
#include <QJsonArray>
#include <QJsonValue>

LiZhiAPI::LiZhiAPI(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
    request.setRawHeader("Host","www.lizhi.fm");
    request.setRawHeader("Connection","keep-alive");
    request.setRawHeader("Accept-Language","zh,zh-CN;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2");
    request.setHeader(QNetworkRequest::ContentTypeHeader,"text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    request.setHeader(QNetworkRequest::UserAgentHeader,"Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:62.0) Gecko/20100101 Firefox/62.0");
}

QString LiZhiAPI::_get_(const QString &url)
{
    request.setUrl(QUrl(url));
    qDebug() << "url:" << url;
    reply = manager->get(request);
    QEventLoop loop;
    connect(manager,&QNetworkAccessManager::finished,&loop,&QEventLoop::quit);
    loop.exec();
    const QString data = reply->readAll();
    return data;
}

//获取节目类型
QJsonObject LiZhiAPI::get_radio_type()
{
    const QString url = BaseUrl + "/sitemap.html";
    const QString data = _get_(url);

    QRegExp rx("【节目类型】.*</ul>");
    rx.setMinimal(true);
    rx.indexIn(data);
    QString contentStr = rx.cap(0);
    rx.setPattern("<a href=\".*\">.*</a>");
    int pos = 0;
    QJsonObject json;
    QJsonArray js_array;
    int count = 0;
    while ((pos = rx.indexIn(contentStr,pos)) != -1)
    {
        const QString temp = rx.cap(0);
        QJsonObject temp_json;
        QRegExp t_rx("/tag/[0-9]*/");
        t_rx.setMinimal(true);
        t_rx.indexIn(temp);
        temp_json.insert("url",t_rx.cap(0));
        t_rx.setPattern(">.*(?=<)");
        t_rx.indexIn(temp);
        temp_json.insert("title",t_rx.cap(0).mid(1));
        pos += rx.matchedLength();
        js_array.insert(count++,temp_json);
    }

    json.insert("list",js_array);

    return json;
}

//获取热门节目
QJsonObject LiZhiAPI::get_hot_grid()
{
    const QString url = BaseUrl + "/hot/";
    return get_radio_list(url);

}

//获取优选电台
QJsonObject LiZhiAPI::get_optimization_grid()
{
    const QString url = BaseUrl + "/promo/";
    return get_radio_list(url);
}

//获取电台列表
QJsonObject LiZhiAPI::get_radio_list(const QString &url)
{
    const QString data = _get_(url);

    QJsonObject json;
    QJsonArray js_array;
    QRegExp rx("<p class=\"radio-last-audio.*>");
    rx.setMinimal(true);
    int pos = 0;
    int count = 0;
    while ((pos = rx.indexIn(data,pos)) != -1)
    {
        QJsonObject temp_json;
        const QString temp = rx.cap(0);
        QStringList list;
        QRegExp t_rx("data-uid=\".*(?=\")");
        t_rx.setMinimal(true);
        t_rx.indexIn(temp);
        temp_json.insert("data-uid",t_rx.cap(0).mid(10));

        t_rx.setPattern("data-user-name=\".*(?=\")");
        t_rx.indexIn(temp);
        temp_json.insert("data-user-name",t_rx.cap(0).mid(16));

        t_rx.setPattern("data-radio-name=\".*(?=\")");
        t_rx.indexIn(temp);
        temp_json.insert("data-radio-name",t_rx.cap(0).mid(17));

        t_rx.setPattern("data-title=\".*(?=\")");
        t_rx.indexIn(temp);
        temp_json.insert("data-title",t_rx.cap(0).mid(12));

        t_rx.setPattern("data-cover=\".*(?=\")");
        t_rx.indexIn(temp);
        temp_json.insert("data-cover",t_rx.cap(0).mid(12));

        js_array.insert(count++,temp_json);
        pos += rx.matchedLength();
    }
    json.insert("list",js_array);

    return json;
}

//获取电台音乐列表
QJsonObject LiZhiAPI::get_music_list(const QString &path)
{
    const QString url = BaseUrl + "/user/" + path;
    const QString data = _get_(url);

    QJsonObject json;
    QJsonArray js_array;
    QRegExp rx("<h1 class=\"user-info-name\">.*(?=(<i class))");
    rx.setMinimal(true);
    rx.indexIn(data);
    json.insert("user-info-name",rx.cap(0).mid(27));

    rx.setPattern("<a href=\"/user/\\d*/(|p/\\d*.html)(?=(\" class=\"prev\">))");
    rx.indexIn(data);
    json.insert("prev",rx.cap(0).mid(15));

    rx.setPattern("<a href=\"/user/\\d*/p/\\d*.html(?=(\" class=\"next\">))");
    rx.indexIn(data);
    json.insert("next",rx.cap(0).mid(15));

    rx.setPattern("<ul class=\"audioList fontYaHei js.*</ul>");
    rx.indexIn(data);
    const QString temp = rx.cap(0);
    rx.setPattern("<a href=.*</a>");
    int pos = 0;
    int count = 0;
    while((pos = rx.indexIn(temp,pos)) != -1)
    {
        QJsonObject temp_json;

        const QString temp_2 = rx.cap(0);
        QRegExp t_rx("data-id=\".*(?=\")");
        t_rx.setMinimal(true);
        t_rx.indexIn(temp_2);
        temp_json.insert("data-id",t_rx.cap(0).mid(9));

        t_rx.setPattern("<p class=\"audioName\">.*(?=(</p>))");
        t_rx.indexIn(temp_2);
        temp_json.insert("audioName",t_rx.cap(0).mid(21));

        t_rx.setPattern("<p class=\"aduioTime\">.*(?=(</p>))");
        t_rx.indexIn(temp_2);
        temp_json.insert("aduioTime",t_rx.cap(0).mid(21).replace("&nbsp;"," "));

        t_rx.setPattern("<div class=\"right duration\">.*(?=(</div>))");
        t_rx.indexIn(temp_2);
        temp_json.insert("right-duration",t_rx.cap(0).mid(28).replace("&#x27;","′"));

        js_array.insert(count++,temp_json);
        pos += rx.matchedLength();
    }
    json.insert("list",js_array);

    return json;
}

//获取tag内容
QJsonObject LiZhiAPI::get_tag_info_list(const QString &path)
{
    const QString url = BaseUrl + path;
    const QString data = _get_(url);

    QJsonObject json;
    QJsonArray js_array;

    QRegExp rx("<a href=\"/\">发现</a>.*(?=(</div>))");
    rx.setMinimal(true);
    rx.indexIn(data);
    json.insert("tag-name",rx.cap(0).mid(18));

    rx.setPattern("<a href=\"(\\./|\\d*.html)(?=(\" class=\"prev\">))");
    rx.indexIn(data);
    json.insert("prev",rx.cap(0).mid(9));

    rx.setPattern("<a href=\"\\d*.html(?=(\" class=\"next\">))");
    rx.indexIn(data);
    json.insert("next",rx.cap(0).mid(9));

    rx.setPattern("/tag/\\d*/");
    rx.indexIn(path);
    json.insert("path",rx.cap(0));

    rx.setPattern("<p class=\"radio-last-audio.*>");
    int pos = 0;
    int count = 0;
    while ((pos = rx.indexIn(data,pos)) != -1)
    {
        const QString temp = rx.cap(0);
        QJsonObject temp_json;
        QRegExp t_rx("data-uid=\".*(?=\")");
        t_rx.setMinimal(true);
        t_rx.indexIn(temp);
        temp_json.insert("data-uid",t_rx.cap(0).mid(10));

        t_rx.setPattern("data-user-name=\".*(?=\")");
        t_rx.indexIn(temp);
        temp_json.insert("data-user-name",t_rx.cap(0).mid(16));

        t_rx.setPattern("data-radio-name=\".*(?=\")");
        t_rx.indexIn(temp);
        temp_json.insert("data-radio-name",t_rx.cap(0).mid(17));

        t_rx.setPattern("data-title=\".*(?=\")");
        t_rx.indexIn(temp);
        temp_json.insert("data-title",t_rx.cap(0).mid(12));

        t_rx.setPattern("data-cover=\".*(?=\")");
        t_rx.indexIn(temp);
        temp_json.insert("data-cover",t_rx.cap(0).mid(12));

        js_array.insert(count++,temp_json);
        pos += rx.matchedLength();
    }
    json.insert("list",js_array);

    return json;
}
