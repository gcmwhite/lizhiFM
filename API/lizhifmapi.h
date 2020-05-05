#ifndef LIZHIFMAPI_H
#define LIZHIFMAPI_H

#include "apiwork.h"
#include <QObject>
#include <QThread>
#include <QMutex>
#include <functional>

class LiZhiFmAPI : public QObject
{
    Q_OBJECT
public:
    explicit LiZhiFmAPI(QObject *parent = nullptr);
    ~LiZhiFmAPI();
    void UrlGet(const QString& url);                   //发起Get请求
    void getAllRadioTag();                             //获取所有分类;解析主页即可
    void getPromo();                                   //获取优选列表【首页】
    void getHot();                                     //获取热榜列表【首页】
    void getMediaUrl(const QString& url);              //获取媒体地址

private:
    void init();

signals:
    void signalUrlGet(const QString& url);                       //发起Get请求
    void signalParseFinished(const PageData& pageData);          //解析后的数据
    void signalPlayMedia(const QString& url);                    //媒体地址

private:
    APIWork* apiWork_{nullptr};             //工作线程
    QThread* thread_{nullptr};
    QMutex mutex_;

};

#endif // LIZHIFMAPI_H
