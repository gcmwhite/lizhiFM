#ifndef LIZHIFMHTTP_H
#define LIZHIFMHTTP_H

#include <QObject>

class LizhiFMHttp : public QObject
{
    Q_OBJECT
public:
    explicit LizhiFMHttp(QObject *parent = nullptr);
    QString fileExist(QString);

signals:
    void htmlSignal(QString,QString);
    void imgDownloadFinishedSiganl(QString);

public slots:
    void urlSlot(QString);
    void downloadSlot(QString);
};

#endif // LIZHIFMHTTP_H
