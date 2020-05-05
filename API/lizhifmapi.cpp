#include "lizhifmapi.h"
#include "Common.h"

LiZhiFmAPI::LiZhiFmAPI(QObject *parent) : QObject(parent)
{
    init();
}

LiZhiFmAPI::~LiZhiFmAPI()
{
    aLog() << "destructor...";
    if (nullptr != thread_) {
        apiWork_->deleteLater();
        thread_->quit();
        thread_->wait();
    }
}

//发起Get请求
void LiZhiFmAPI::UrlGet(const QString &url)
{
    emit signalUrlGet(url);
}

//获取所有分类;解析主页即可
void LiZhiFmAPI::getAllRadioTag()
{
    emit signalUrlGet(ALL_RADIO_TAG_URL);
}

//获取优选列表【首页】
void LiZhiFmAPI::getPromo()
{
    emit signalUrlGet(BASE_URL + PROMO_URL_KEY);
}

//获取热榜列表【首页】
void LiZhiFmAPI::getHot()
{
    emit signalUrlGet(BASE_URL + HOT_URL_KEY);
}

//获取媒体地址
void LiZhiFmAPI::getMediaUrl(const QString &url)
{
    emit signalUrlGet(url);
}

void LiZhiFmAPI::init()
{
    //线程初始化
    apiWork_ = new APIWork;
    thread_ = new QThread(this);
    apiWork_->moveToThread(thread_);
    connect(thread_,&QThread::started,apiWork_,&APIWork::slotRun);
    thread_->start();

    //url get请求
    connect(this,&LiZhiFmAPI::signalUrlGet,apiWork_,&APIWork::slotUrlGet);

    //解析后的数据
    connect(apiWork_,&APIWork::signalParseFinished,this,&LiZhiFmAPI::signalParseFinished);

    //媒体地址
    connect(apiWork_,&APIWork::signalPlayMedia,this,&LiZhiFmAPI::signalPlayMedia);
}
