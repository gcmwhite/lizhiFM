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

    connect(this,&Aplayer::bufferStatusChanged,this,[=](int percentFilled){
        qDebug() << "buffer:" << percentFilled;
    });

    //自动播放下一曲
    connect(this,&Aplayer::stateChanged,this,[=](){
        if (vec_play_list.isEmpty())
            return ;
        if (this->mediaStatus() == QMediaPlayer::EndOfMedia)
        {
            a_next();
        }
    });

    //歌曲切换
    connect(this,&Aplayer::currentMediaChanged,this,[=](){
        if (vec_play_list.isEmpty())
            return ;
        emit current_media_index_changed(index);
        emit current_media_title_changed(vec_play_list.at(index).at(1));
    });

}

//添加播放列表
void Aplayer::add_play_list(const bool ok,const QVector<QStringList> &vec_list)
{
    vec_play_list.clear();
    vec_play_list.reserve(vec_list.size());
    vec_play_list = vec_list;
    if (ok)
    {
        index = 0;
        a_play(index);
    } else {
        if (this->state() != QMediaPlayer::PlayingState)
            a_play(index);
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
    this->play();
}

void Aplayer::a_play(const QString &data_id)
{
    qDebug() << "测试殿2";
    this->setMedia(QUrl(_get_music_url_(data_id)));
    qDebug() << "测试殿3";
    this->play();
}

//上一曲

void Aplayer::a_previous()
{
    if (index >= 1)
    {
        a_play(--index);
    }
}



//下一曲

void Aplayer::a_next()
{
    if (index < vec_play_list.size() - 1)
    {
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

//获取当前正在播放的曲目
int Aplayer::get_current_index()
{
    return index;
}

//更改当前播放的曲目
void Aplayer::set_current_index(int num)
{
    index = num;
}
