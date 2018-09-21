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
    QVector<QStringList> get_hot_grid();
    QVector<QStringList> get_optimization_grid();
    QVector<QStringList> get_music_list(const QString &);
    QVector<QStringList> get_tag_info_list(const QString &);

private:
    QVector<QStringList> get_radio_list(const QString &);
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
