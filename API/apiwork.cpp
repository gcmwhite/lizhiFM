#include "apiwork.h"
#include "Common.h"
#include "Document.h"
#include "Node.h"
#include <QThread>
#include <QFile>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonArray>
#include <QFileInfo>
#include <QDir>

APIWork::APIWork()
{

}

APIWork::~APIWork()
{
    aLog() << "destructor...";
}

//发起Get请求
void APIWork::slotUrlGet(const QString &url)
{
    aLog() << "请求地址：" << url;
    networkRequest_.setUrl(url);
    networkAccessManager_->get(networkRequest_);
}

//初始化HTTP头部
void APIWork::initRequestHeader()
{
    auto func = [](QByteArray& bytes){
        QFile file(QStringLiteral(":/Resources/json/request-header.json"));
        file.open(QIODevice::ReadOnly);
        bytes = file.readAll();
        file.close();
    };

    QDir dir(CONFIG_PATH);
    QFileInfo info(dir,REQUEST_HEADEAR_FILE);
    QFile file(info.absoluteFilePath());
    QByteArray bytes;
    if (file.exists()) {
        if (file.open(QIODevice::ReadOnly)) {
            bytes = file.readAll();
        }else {
            aWarning() << QString("%1打开失败，将读取内置文件").arg(info.fileName());
            func(bytes);
        }
        file.close();
    }else {
        aWarning() << QString("%1文件不存在，将读取内置文件。程序将自动创建%1").arg(info.fileName());
        func(bytes);
        if (!dir.exists()) {
            dir.makeAbsolute();
            dir.mkpath(dir.path());
        }
        bool ok = QFile::copy(QStringLiteral(":/Resources/json/request-header.json"),info.absoluteFilePath());
        if (ok) {
            aSucess() << info.fileName() << QStringLiteral("创建成功！");
        }else {
            aWarning() << info.fileName() << QStringLiteral("创建失败！请检查是否有写入权限！");
        }
    }
    QJsonParseError jsonError;
    QJsonDocument doc = QJsonDocument::fromJson(bytes,&jsonError);
    if (QJsonParseError::NoError != jsonError.error) {
        aError() << QString("%1解析失败，将无法设置http header，可能无法正确获取网页内容。您可以尝试删除%1文件来解决此问题。").arg(info.fileName());
    }

    QJsonObject json = doc.object();
    for (auto it = json.constBegin();it != json.constEnd();++it) {
        networkRequest_.setRawHeader(it.key().toUtf8(),it.value().toString().toUtf8());
    }
}

//按所有分类格式化数据
void APIWork::allRadioTagParsing(const QByteArray &bytes)
{
    PageData::PageStatus pageStatus;
    pageStatus.parseWay = PageData::PARSE_WAY::ALL_RADIO_TAG;
    pageStatus.requestUrl = ALL_RADIO_TAG_URL;

    PageData::Adata aData;
    PageData pageData(this);

    CDocument doc;
    doc.parse(bytes.toStdString());
    CSelection allRadioTag = doc.find("#allRadioTag");
    CSelection allRadioTagDiv = doc.find("#allRadioTag>div");
    for (size_t i = 0;i < allRadioTagDiv.nodeNum();++i) {
        CSelection c = allRadioTagDiv.nodeAt(i).find("ul>li>a");
        QVector<std::tuple<QString,QString>> vec;
        if (c.nodeNum() > 0) {
            for (size_t i = 0;i < c.nodeNum();++i) {
                vec.append(std::make_tuple(QString::fromStdString(c.nodeAt(i).text())
                                           ,QStringLiteral("https:")+QString::fromStdString(c.nodeAt(i).attribute("href"))));
            }
            c = allRadioTagDiv.nodeAt(i).find(".tagName");
            if (c.nodeAt(0).valid()) {
                aData.dataTitle = QString::fromStdString(c.nodeAt(0).text());
            }
            aData.radioTag = vec;
            pageData.appendData(aData);
        }
    }
    CSelection c = allRadioTag.nodeAt(0).find(".left h2");
    if (c.nodeAt(0).valid()) {
        pageStatus.title = QString::fromStdString(c.nodeAt(0).text());
    }
    pageData.setPageStatus(pageStatus);

    emit signalParseFinished(pageData);
    /*
    for (const auto data : pageData.getData()) {
        qDebug() << data.dataTitle << "===================================";
        for (const auto m : data.radioTag) {
            qDebug() << QString("%1==>%2").arg(std::get<0>(m),std::get<1>(m));
        }
    }
    qDebug() << pageData.getPageStatus().title;
    */
}

//按电台列表格式化数据
void APIWork::radioListParsing(const QString &url, const QByteArray &bytes)
{
    PageData::PageStatus pageStatus;
    pageStatus.parseWay = PageData::PARSE_WAY::RADIO_LIST;
    pageStatus.requestUrl = url;

    PageData::Adata aData;
    PageData pageData(this);

    CDocument doc;
    doc.parse(bytes.toStdString());
    CSelection mainBox = doc.find(".mainBox");
    CSelection c = mainBox.find(".allRadioList>.radio_list");
    for (size_t i = 0;i < c.nodeNum();++i) {
        CNode cNode = c.nodeAt(i).find("p").nodeAt(1);
        if (aData.parse = cNode.valid(),!aData.parse) {
            continue ;
        }

        aData.dataBand = QString::fromStdString(cNode.attribute("data-band"));
        aData.dataUserName = QString::fromStdString(cNode.attribute("data-user-name"));
        aData.dataCover = QString::fromStdString(cNode.attribute("data-cover"));
        aData.url = QString("%1/user/%2")
                .arg(BASE_URL)
                .arg(QString::fromStdString(cNode.attribute("data-uid")));
        pageData.appendData(aData);
    }
    CNode node = mainBox.find(".page").nodeAt(0);
    if (node.valid()) {
        pageStatus.prev = node.find(".prev").nodeAt(0).valid();
        pageStatus.next = node.find(".next").nodeAt(0).valid();
//        CNode c = node.find(".current").nodeAt(0);
        if (url.endsWith(QStringLiteral(".html"))) {
            pageStatus.current = url.split('/').last().chopped(5).toInt();
        }/*else {
            pageStatus.current = c.valid() ? QString::fromStdString(c.text()).toInt() : pageStatus.current;
        }*/
    }
    node = mainBox.find(".box-title>.left").nodeAt(0);
    pageStatus.title = node.valid() ? QString::fromStdString(node.ownText()) : "";
    pageData.setPageStatus(pageStatus);

    emit signalParseFinished(pageData);

    /*
    for (const auto& aData : pageData.getData()) {
        qDebug() << "data-band:" << aData.dataBand;
        qDebug() << "data-cover:" << aData.dataCover;
        qDebug() << "data-user-name:" << aData.dataUserName;
        qDebug() << "data-url:" << aData.url;
        qDebug() << "++++++++++++++++++++++++++++++";
    }
    qDebug() << QString("titile:%0 current:%1 prev:%2 next:%3 request-url:%4 size:%5")
                .arg(pageStatus.title)
                .arg(pageStatus.current)
                .arg(pageStatus.prev)
                .arg(pageStatus.next)
                .arg(pageStatus.requestUrl)
                .arg(pageData.getData().size());
    */

}

//按音频列表格式化数据
void APIWork::audioListParsing(const QString &url, const QByteArray &bytes)
{
    PageData::PageStatus pageStatus;
    pageStatus.parseWay = PageData::PARSE_WAY::AUDIO_LIST;
    pageStatus.requestUrl = url;

    PageData::Adata aData;
    PageData pageData(this);

    CDocument doc;
    doc.parse(bytes.toStdString());
    CNode frame = doc.find(".frame").nodeAt(0);
    CNode node = frame.find("ul[class=\"audioList fontYaHei js-audio-list\"]").nodeAt(0);
    if (node.valid()) {
        CSelection c = node.find("li>a");
        for (size_t i = 0;i < c.nodeNum();++i) {
            node = c.nodeAt(i);
            CNode audioTimeNode = node.find(".aduioTime").nodeAt(0);
            aData.dataId = QString::fromStdString(node.attribute("data-id"));
            aData.dataTitle = QString::fromStdString(node.attribute("data-title"));
            aData.dataDuration = QString::fromStdString(node.attribute("data-duration")).toLongLong();
            aData.audioTime = QString::fromStdString(audioTimeNode.valid() ? audioTimeNode.text() : "");
            aData.url = QStringLiteral("https://www.lizhi.fm/media/url/")+aData.dataId;
            node = node.find(".radio-list-item-index").nodeAt(0);
            aData.itemIndex = node.valid() ? QString::fromStdString(node.text()).toInt() : aData.itemIndex;
            pageData.appendData(aData);
        }
    }
    node = frame.find(".page").nodeAt(0);
    if (node.valid()) {
        pageStatus.prev = node.find(".prev").nodeAt(0).valid();
        pageStatus.next = node.find(".next").nodeAt(0).valid();
        CNode c = node.find(".current").nodeAt(0);
        pageStatus.current = c.valid() ? QString::fromStdString(c.text()).toInt() : pageStatus.current;
    }
    node = frame.find(".user-info-name").nodeAt(0);
    pageStatus.title = node.valid() ? QString::fromStdString(node.ownText()) : "";
    pageData.setPageStatus(pageStatus);

    emit signalParseFinished(pageData);


#if 0
    for (const auto& aData : pageData.getData()) {
        qDebug() << "data-id:" << aData.dataId;
        qDebug() << "data-title:" << aData.dataTitle;
        qDebug() << "data-duration:" << aData.dataDuration;
        qDebug() << "autio-time:" << aData.audioTime;
        qDebug() << "url:" << aData.url;
        qDebug() << "+++++++++++++++++++++++++++++++";
    }
    qDebug() << QString("titile:%0 current:%1 prev:%2 next:%3 request-url:%4 size:%5")
                .arg(pageStatus.title)
                .arg(pageStatus.current)
                .arg(pageStatus.prev)
                .arg(pageStatus.next)
                .arg(pageStatus.requestUrl)
                .arg(pageData.getData().size());
#endif

}

//线程运行:只能从槽函数运行
void APIWork::slotRun()
{
    initRequestHeader();
    networkAccessManager_ = new QNetworkAccessManager(this);
    connect(networkAccessManager_,&QNetworkAccessManager::finished,this,&APIWork::slotHttpReceived);
}

//接收HTTP数据
void APIWork::slotHttpReceived(QNetworkReply *reply)
{
    reply->deleteLater();
    QByteArray bytes = reply->readAll();
    QString url = reply->url().url();
    aLog() << "接收HTTP数据：" << url;
    if (ALL_RADIO_TAG_URL == url) {                 //更多分类
        allRadioTagParsing(bytes);
    }else if (url.contains(PROMO_URL_KEY) ||            //优选电台
              url.contains(HOT_URL_KEY) ||              //热榜电台
              url.contains(RADIO_URL_KEY)) {            //某一分类电台列表
        radioListParsing(url,bytes);
    }else if (url.contains(AUDIO_URL_KEY)) {            //音频列表
        audioListParsing(url,bytes);
    }else if (url.contains(MEDIA_URL_KEY)) {            //媒体
        QJsonDocument doc = QJsonDocument::fromJson(bytes);
        QString url = doc.object().value("data").toObject().value("url").toString();
        aLog() << "正在播放:" << url;
        emit signalPlayMedia(url);
    }
}
