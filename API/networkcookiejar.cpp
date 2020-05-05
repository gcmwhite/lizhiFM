#include "networkcookiejar.h"

NetworkCookieJar::NetworkCookieJar()
{

}

//获取cookie
QList<QNetworkCookie> NetworkCookieJar::getCookies()
{
    return allCookies();
}

//设置cookie
void NetworkCookieJar::setCookies(const QList<QNetworkCookie> &cookies)
{
    setAllCookies(cookies);
}
