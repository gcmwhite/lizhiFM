#ifndef UPDATE_H
#define UPDATE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

class Update : public QObject
{
    Q_OBJECT
public:
    explicit Update(const QString &,QObject *parent = nullptr);

private:
    QNetworkAccessManager *manager;
    QNetworkRequest request;

signals:

public slots:
};

#endif // UPDATE_H
