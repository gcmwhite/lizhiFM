#ifndef APLAYER_H
#define APLAYER_H

#include <QObject>
#include <QMediaPlayer>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QVector>

class Aplayer : public QMediaPlayer
{
    Q_OBJECT
public:
    explicit Aplayer(QObject *parent = nullptr);
    void add_play_list(const bool,const QVector<QStringList> &);
    void a_play();
    void a_play(int);
    void a_previous();
    void a_next();

private:
    QString _get_music_url_(const QString &);
    int index = 0;

private:
    QNetworkAccessManager *manager;
    QNetworkRequest request;

public:
    bool play_flag = false;                 //播放状态
    QVector<QStringList> vec_play_list;

signals:
    void current_media_index_changed(int);
    void current_media_title_changed(const QString &);


public slots:
};

#endif // APLAYER_H
