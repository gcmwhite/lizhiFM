#ifndef APIWORK_H
#define APIWORK_H

#define BASE_URL QStringLiteral("https://www.lizhi.fm")        //基地址&更多分类
#define ALL_RADIO_TAG_URL BASE_URL                             //更多分类
#define PROMO_URL_KEY QStringLiteral("/promo/")                //优选电台
#define HOT_URL_KEY QStringLiteral("/hot/")                    //热榜
#define RADIO_URL_KEY QStringLiteral("/label/")                //分类下电台列表
#define AUDIO_URL_KEY QStringLiteral("/user/")                 //音频列表
#define MEDIA_URL_KEY QStringLiteral("/media/")                //媒体

#include "pagedata.h"
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class APIWork : public QObject
{
    Q_OBJECT
public:
    explicit APIWork();
    ~APIWork();

private:
    void initRequestHeader();                                    //初始化HTTP头部
    void allRadioTagParsing(const QByteArray& bytes);            //按所有分类格式化数据
    void radioListParsing(const QString& url,const QByteArray& bytes);             //按电台列表格式化数据
    void audioListParsing(const QString& url,const QByteArray& bytes);             //按音频列表格式化数据

signals:
    void signalParseFinished(const PageData& pageData);           //解析后的数据
    void signalPlayMedia(const QString& url);

public slots:
    void slotRun();                                    //线程运行
    void slotUrlGet(const QString& url);               //发起Get请求

private slots:
    void slotHttpReceived(QNetworkReply* reply);            //接收HTTP数据

private:
    QNetworkAccessManager* networkAccessManager_{nullptr};
    QNetworkRequest networkRequest_;
};

#endif // APIWORK_H
