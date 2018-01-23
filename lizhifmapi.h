#ifndef LIZHIFMAPI_H
#define LIZHIFMAPI_H

#include <QObject>

class LiZhiFMAPI : public QObject
{
    Q_OBJECT
public:
    explicit LiZhiFMAPI(QObject *parent = nullptr);

signals:
    void htmlSignal(QString,QString);
    void imgDownloadFinishedSiganl(QString);

public slots:
    void urlSlot(QString);
    void downloadSlot(QString);
};

#endif // LIZHIFMAPI_H
