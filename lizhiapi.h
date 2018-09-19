#ifndef LIZHIAPI_H
#define LIZHIAPI_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QVector>

class LiZhiAPI : public QObject
{
    Q_OBJECT
public:
    explicit LiZhiAPI(QObject *parent = nullptr);
    QVector<QStringList> get_radio_type();

private:
    const QString BaseUrl = "http://www.lizhi.fm";
    QNetworkAccessManager *manager;
    QNetworkRequest request;
    QNetworkReply *reply;

signals:

public slots:
};

#endif // LIZHIAPI_H
