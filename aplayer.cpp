#include "aplayer.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QDebug>
#include <QEventLoop>
#include <QNetworkReply>

Aplayer::Aplayer(QObject *parent)
    : QMediaPlayer(parent)
{
    manager = new QNetworkAccessManager(this);
    request.setRawHeader("Host","www.lizhi.fm");
    request.setHeader(QNetworkRequest::UserAgentHeader,"Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:62.0) Gecko/20100101 Firefox/62.0");
    request.setRawHeader("Accept-Language","zh,zh-CN;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2");
    request.setRawHeader("Referer","http://www.lizhi.fm/box");
    request.setRawHeader("X-Requested-With","XMLHttpRequest");
    request.setRawHeader("Connection","keep-alive");

    //自动播放下一曲
    connect(this,&Aplayer::stateChanged,this,[=](){
        if (this->mediaStatus() == QMediaPlayer::EndOfMedia)
        {
            a_next();
        }
    });

    //歌曲切换
    connect(this,&Aplayer::currentMediaChanged,this,[=](){
        emit current_media_index_changed(index);
        emit current_media_title_changed(vec_play_list.at(index).at(1));
    });
}

//添加播放列表
void Aplayer::add_play_list(const bool ok, QVector<QStringList> vec_list)
{
    vec_list.pop_front();
    if (ok)
    {
        index = 0;
        vec_play_list.clear();
        vec_play_list.reserve(vec_list.size());
        vec_play_list = vec_list;
        a_play(index);
    } else {
        vec_play_list.reserve(vec_list.size() + vec_play_list.size());
        vec_play_list += vec_list;
    }
}

//播放
void Aplayer::a_play()
{
    if (this->state() == QMediaPlayer::PlayingState)
    {
        this->pause();
    } else {
        this->play();
    }
}

void Aplayer::a_play(int index)
{
    this->setMedia(QUrl(_get_music_url_(vec_play_list.at(index).at(0))));
    a_play();
}

//上一曲
void Aplayer::a_previous()
{
    if (index >= 1)
    {
        this->stop();
        a_play(--index);
    }
}

//下一曲
void Aplayer::a_next()
{
    if (index < vec_play_list.size() - 1)
    {
        this->stop();
        a_play(++index);
    }
}

//获取音乐地址
QString Aplayer::_get_music_url_(const QString &musci_id)
{
    const QString url = "http://www.lizhi.fm/media/url/" + musci_id;
    qDebug() << "m_url:" << url;
    request.setUrl(QUrl(url));
    QNetworkReply *reply = manager->get(request);
    QEventLoop loop;
    connect(manager,&QNetworkAccessManager::finished,&loop,&QEventLoop::quit);
    loop.exec();
    const QString data = reply->readAll();
//    qDebug() << data;
    QJsonDocument document = QJsonDocument::fromJson(data.toUtf8());
    QJsonObject root = document.object();
    const QString msg = root["msg"].toString();
    if (msg == "ok")
    {
        QJsonObject data = root["data"].toObject();
        const QString music_url = data["url"].toString();
        qDebug() << "music_url:" << music_url;
        return music_url;
    }
    return "";
}
