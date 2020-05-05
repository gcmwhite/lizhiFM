#ifndef NETWORKCOOKIEJAR_H
#define NETWORKCOOKIEJAR_H

#include <QNetworkCookieJar>
#include <QNetworkCookie>

class NetworkCookieJar : public QNetworkCookieJar
{
    Q_OBJECT
public:
    NetworkCookieJar();

    //获取cookie
    QList<QNetworkCookie> getCookies();
    //设置cookie
    void setCookies(const QList<QNetworkCookie>& cookies);
};

#endif // NETWORKCOOKIEJAR_H
