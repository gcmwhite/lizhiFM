#ifndef LIZHIAPI_H
#define LIZHIAPI_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonObject>

class LiZhiAPI : public QObject
{
    Q_OBJECT
public:
    explicit LiZhiAPI(QObject *parent = nullptr);
    QJsonObject get_radio_type();
    QJsonObject get_hot_grid();
    QJsonObject get_optimization_grid();
    QJsonObject get_music_list(const QString &);
    QJsonObject get_tag_info_list(const QString &);

private:
    QJsonObject get_radio_list(const QString &);
    QString _get_(const QString &);

private:
    const QString BaseUrl = "http://www.lizhi.fm";
    QNetworkAccessManager *manager;
    QNetworkRequest request;
    QNetworkReply *reply;

signals:

public slots:
};

#endif // LIZHIAPI_H
