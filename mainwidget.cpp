#include "mainwidget.h"
#include "aboutwidget.h"
#include "donatewidget.h"
#include "skinwidget.h"
#include <QRegExp>
#include <QDebug>
#include <QMessageBox>
#include <QPixmap>
#include <QPalette>
#include <QPainter>
#include <QBrush>
#include <QDesktopServices>
#include <QUrl>
#include <QFile>
#include <QDataStream>

#define lizhi "http://www.lizhi.fm"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    mainWidgetInit();           //初始化
    readData();     //读取配置信息
}

MainWidget::~MainWidget()
{
    fmPlayer->pause();
    writeData();
    fmPlayer->stop();
}

void MainWidget::mainWidgetInit()       //Initialization
{
    this->resize(990,590);
    this->setWindowTitle("荔枝FM");
    this->setWindowIcon(QIcon(":/imgs/lizhi_favicon.ico"));

    pixmap = QPixmap(":/background/background_1.jpg");
    setBackgroundImage(pixmap);

    listCurrentIndex = 0;       //列表当前位置
    readFlag = false;           //配置读取状态

    tabWidget_1 = new QWidget;
    tabWidget_2 = new QWidget;
    tabWidget_3 = new QWidget;
    tabWidget_3_layout = new QVBoxLayout(tabWidget_3);
    tabWidget_3_1 = new QWidget;
    tabWidget_3_2 = new QWidget;
    tabWidget = new MTabWidget;
    tabWidget_3_layout->addWidget(tabWidget_3_1);
    tabWidget->addTab(tabWidget_1,"热门电台");
    tabWidget->addTab(tabWidget_2,"优选电台");
    tabWidget->addTab(tabWidget_3,"节目类型");
    leftWidget = new LeftWidget;
    footWidget = new FootWidget;
    listWidget = new ListWidget;
    mainLayout = new QVBoxLayout(this);
    centerLayout = new QHBoxLayout;

    centerLayout->addWidget(leftWidget,1);
    centerLayout->addWidget(tabWidget,3);

    mainLayout->addLayout(centerLayout,10);
    mainLayout->addWidget(footWidget);

    lizhiFMHttp = new LizhiFMHttp(this);
    fmPlayer = new QMediaPlayer(this);
    fmPlayer->setVolume(100);

    connect(this,&MainWidget::urlSignal,lizhiFMHttp,&LizhiFMHttp::urlSlot);
    connect(lizhiFMHttp,&LizhiFMHttp::htmlSignal,this,&MainWidget::htmlSlot);
    connect(this,&MainWidget::imgUrlSiganl,lizhiFMHttp,&LizhiFMHttp::downloadSlot);
    connect(lizhiFMHttp,&LizhiFMHttp::imgDownloadFinishedSiganl,this,&MainWidget::imgLabelSlot);

    QString str = "/hot/";      //热门电台
    QString url = lizhi + str;
    emit urlSignal(url);

    str = "/promo/";            //优选网络
    url = lizhi + str;
    emit urlSignal(url);

    str = "/sitemap.html";      //节目类型
    url = lizhi + str;
    emit urlSignal(url);

    connect(listWidget->preBtn,&QPushButton::clicked,this,[=](){
        if (!prePageUrl.isEmpty())
        {
            qDebug() << "上一页:" << prePageUrl;
            emit urlSignal(prePageUrl);
        }else {
            QMessageBox box(QMessageBox::Warning,"注意","已经是第一页!");
            box.setStandardButtons(QMessageBox::Ok);
            box.setButtonText(QMessageBox::Ok,"确定");
            box.exec();
        }
    });

    connect(listWidget->nextBtn,&QPushButton::clicked,[=](){
//        qDebug() << "xwyiy" << nextPageUrl;
        if (!nextPageUrl.isEmpty())
        {
            qDebug() << "下一页" << nextPageUrl;
            emit urlSignal(nextPageUrl);
        } else {
            QMessageBox box(QMessageBox::Warning,"注意","已经是最后一页!");
            box.setStandardButtons(QMessageBox::Ok);
            box.setButtonText(QMessageBox::Ok,"确定");
            box.exec();
        }
    });

    connect(listWidget->backBtn,&QPushButton::clicked,[=](){
        centerLayout->replaceWidget(listWidget,tabWidget);
        listWidget->setParent(NULL);
    });

    connect(listWidget->playListBtn,&QPushButton::clicked,[=](){
        readFlag = false;
        listCurrentIndex = 0;
        playList.clear();
        playTitle.clear();
        leftWidget->listWidget->clear();
        playTitle = titleList;
        leftWidget->listWidget->addItems(playTitle);
        playList = urlList;
        emit urlSignal(lizhi+QString("/media/url/")+playList.at(listCurrentIndex));
    });

    connect(leftWidget->preBtn,&QPushButton::clicked,this,&MainWidget::previousSlot);
    connect(leftWidget->nextBtn,&QPushButton::clicked,this,&MainWidget::nextSlot);
    connect(leftWidget->pauseBtn,&QPushButton::clicked,this,[=](){
        if (fmPlayer->state() == QMediaPlayer::PlayingState){
            fmPlayer->pause();
            leftWidget->pauseBtn->setIcon(QIcon(":/imgs/play.ico"));
        } else {
            fmPlayer->play();
            leftWidget->pauseBtn->setIcon(QIcon(":/imgs/pause.ico"));
        }
        if (playTitle.isEmpty())
        {
            QMessageBox box(QMessageBox::Warning,"注意","播放列表为空！");
            box.setStandardButtons(QMessageBox::Ok);
            box.setButtonText(QMessageBox::Ok,"确定");
            box.exec();
        }
    });

    connect(fmPlayer,&QMediaPlayer::currentMediaChanged,this,[=](){
        leftWidget->listWidget->setCurrentRow(listCurrentIndex);
            if (playTitle.isEmpty())
            {
                leftWidget->titleLabel->setText("荔枝FM");
            }else {
                QString str = QString("正在播放：").append(playTitle.at(listCurrentIndex));
                if (str.length() > 18)
                {
                    str = str.left(18);
                    str.append("...");
                }
                leftWidget->titleLabel->setText(str);
            }

        });

        connect(fmPlayer,&QMediaPlayer::stateChanged,this,[=](){        //播放列表
            qDebug() << "state:" << fmPlayer->state();
            qDebug() << fmPlayer->mediaStatus();
            if (fmPlayer->mediaStatus() == QMediaPlayer::EndOfMedia)
            {
                nextSlot();
            }
            if (fmPlayer->state() == QMediaPlayer::PlayingState)
            {
                leftWidget->pauseBtn->setIcon(QIcon(":/imgs/pause.ico"));
            } else {
                leftWidget->pauseBtn->setIcon(QIcon(":/imgs/play.ico"));
            }
        });

        connect(fmPlayer,&QMediaPlayer::positionChanged,this,[=](qint64 time){     //更新进度条
            int positionTime = int(time/1000);
            int totalTime = int(fmPlayer->duration() / 1000);
            int remainingTime = totalTime - positionTime;
            footWidget->timeSlider->setRange(0,totalTime);
            if (!footWidget->timeSlider->isSliderDown())
            {
                footWidget->timeSlider->setValue(positionTime);
            }
            footWidget->timeLabel->setText(QString("%1:%2").arg(positionTime/60,2,10,QLatin1Char('0'))
                                   .arg(positionTime%60,2,10,QLatin1Char('0')));
            footWidget->remainingTimeLabel->setText(QString("%1:%2").arg(remainingTime/60,2,10,QLatin1Char('0'))
                                            .arg(remainingTime%60,2,10,QLatin1Char('0')));
        });

        connect(footWidget->timeSlider,&QSlider::sliderReleased,[=](){      //实现进度条的拖动
            fmPlayer->setPosition(qint64(footWidget->timeSlider->value()*1000));
        });

        connect(fmPlayer,&QMediaPlayer::bufferStatusChanged,this,[=](int buffer){
            qDebug() << "buffer：" <<buffer;
        });

        connect(tabWidget,&MTabWidget::aboutClicked,this,[=](){     //tab关于按钮
            AboutWidget *aboutWidget = new AboutWidget;
            aboutWidget->show();
        });

        connect(tabWidget,&MTabWidget::donateClicked,this,[=](){        //tab捐赠按钮
            DonateWidget *donateWidget = new DonateWidget;
            donateWidget->show();
        });

        connect(tabWidget,&MTabWidget::settingClicked,this,[=](){           //tab设置按钮
            QMessageBox box(QMessageBox::Warning,"提示","没有设置内容");
            box.exec();
        });

        connect(tabWidget,&MTabWidget::githubClicked,this,[=](){            //tab github按钮
            QDesktopServices::openUrl(QUrl("https://github.com/gcmwhite/lizhiFM"));
        });

        connect(tabWidget,&MTabWidget::skinClicked,this,[=](){          //tab皮肤按钮
            SkinWidget *skinWidget = new SkinWidget;
            skinWidget->show();
            connect(skinWidget,&SkinWidget::backgroundSiganl,[=](QString str){
                background = str;
                pixmap = QPixmap(background);
                setBackgroundImage(pixmap);
            });
        });

}

void MainWidget::htmlSlot(QString url, QString html)        //html process
{
    qDebug() << "htmlslot" << url;
    QStringList list = readPage(url,html);
    if (html.startsWith("{\"rcode"))
    {
        QRegExp rx("http.*(?=\")");
        rx.setMinimal(true);
        rx.indexIn(html);
        QString url = rx.cap(0);
        fmPlayer->setMedia(QUrl(url));
        if (!readFlag)
        {
            fmPlayer->play();
        } else {
            qDebug() << "position" << position;
            fmPlayer->setPosition(position);
            fmPlayer->play();
            fmPlayer->pause();
        }
        qDebug() << url;
    }else if (url.contains("/hot/")){
        addListtoListWidget(list,radioNameList(html),tabWidget_1);
    }else if (url.contains("/promo/"))
    {
        addListtoListWidget(list,radioNameList(html),tabWidget_2);
    }else if (url.contains("sitemap.html"))
    {
        QStringList list = classList(html);
        list = hrefList(list);
        QGridLayout *gridLayout = new QGridLayout(tabWidget_3_1);
        QStringList tempList;
        for (int i = 0 ;i < list.size(); i++)
        {
            tempList = QString(list.at(i)).split(',');
            QPushButton *button = new QPushButton;
            connect(button,&QPushButton::clicked,this,[=](){
                qDebug() << lizhi + tempList.at(0);
                emit urlSignal(lizhi + tempList.at(0));
            });
            button->setText(tempList.at(1));
            gridLayout->addWidget(button,i/5,i%5);
        }
    }else if (url.contains("/user/"))
    {
        centerLayout->replaceWidget(tabWidget,listWidget);
        tabWidget->setParent(NULL);
        listWidget->pageLabel->setText(QString(" 第 %1 页 ").arg(list.at(2)));
        prePageUrl = list.at(0);
        nextPageUrl = list.at(1);

        list = user_sList(html);
        urlList.clear();
        titleList.clear();
        listWidget->radioListWidget->clear();
        for (int i = 0; i < list.size(); i++)
        {
            QStringList tempList = QString(list.at(i)).split(',');
            urlList << tempList.at(0);
            titleList << tempList.at(1);
        }
        listWidget->radioListWidget->addItems(titleList);
    }else if (url.contains("/tag/"))
    {
        QObjectList childrenList = tabWidget_3_2->children();
        if (!childrenList.isEmpty())
        {
            for (int i = 0;i < childrenList.size();i++)
            {
                delete childrenList.at(i);
            }
        }
        QWidget *tempWidget = new QWidget;
        addListtoListWidget(list,radioNameList(html),tempWidget);
        QVBoxLayout *tempvBoxLayout = new QVBoxLayout(tabWidget_3_2);
        QPushButton *back = new QPushButton;
        back->setFlat(true);
        back->setIcon(QIcon(":/imgs/back.ico"));
        back->setIconSize(QSize(30,30));
        back->setToolTip("返回");
        back->setCursor(Qt::PointingHandCursor);
//        back->setText("返回");
        tempvBoxLayout->addWidget(back,1,Qt::AlignLeft);
        tempvBoxLayout->addWidget(tempWidget,10);

        tabWidget_3_layout->replaceWidget(tabWidget_3_1,tabWidget_3_2);
        tabWidget_3_1->setParent(NULL);

        connect(back,&QPushButton::clicked,this,[=](){
            tabWidget_3_layout->replaceWidget(tabWidget_3_2,tabWidget_3_1);
            tabWidget_3_2->setParent(NULL);
        });
    }
}

QStringList MainWidget::radioNameList(QString str)      //获取radioNameList
{
    QStringList list;
    QRegExp reg;
    reg.setMinimal(true);
    reg.setPattern("radio-last-audio.*</p>");
    QRegExp rx;
    rx.setMinimal(true);
    int pos = 0;
    while ((pos = reg.indexIn(str,pos)) != -1)
    {
        pos += reg.matchedLength();
        QString temp1 = reg.cap(0);

        rx.setPattern("data-user-name=\".*(?=\")");
        rx.indexIn(temp1);
        QString name = rx.cap(0).remove(0,16);

        rx.setPattern("data-uid=\".*(?=\")");
        rx.indexIn(temp1);
        QString listUrl = lizhi + rx.cap(0).replace("data-uid=\"","/user/");

        rx.setPattern("data-cover=\".*(?=\")");
        rx.indexIn(temp1);
        QString imgUrl = rx.cap(0).remove(0,12);

        list << name + "," + listUrl + "," +imgUrl;
    }
    return list;
}

void MainWidget::addListtoListWidget(QStringList page,QStringList list,QWidget *widget)
{
    QObjectList widgetChildren = widget->children();
    if (!widgetChildren.isEmpty())
    {
        for (int i = 0;i < widgetChildren.size(); i++)
        {
            delete widgetChildren.at(i);
        }
    }

    QVBoxLayout *vboxLayout = new QVBoxLayout(widget);
//    vboxLayout->setMargin(0);
//    vboxLayout->setSpacing(0);
    QGridLayout *gridLayout = new QGridLayout;
    vboxLayout->addLayout(gridLayout);
    gridLayout->setMargin(0);
    gridLayout->setSpacing(0);
    QStringList tempList;
    for (int i = 0 ;i < list.size(); i++)
    {
        tempList = QString(list.at(i)).split(',');
        QWidget *tempWidget = new QWidget;
        QVBoxLayout *layout = new QVBoxLayout(tempWidget);
        layout->setMargin(0);
        layout->setSpacing(0);
        QLabel *imgLabel = new QLabel;
        imgLabel->setMaximumSize(70,70);
        imgLabel->setScaledContents(true);
//        imgLabel->setObjectName(QString(tempList.at(2)).split('/').last());
//        qDebug() << "objectName:  " << imgLabel->objectName();
//        QPixmap pix(":/imgs/noImg.jpg");
//        imgLabel->setPixmap(pix);
        QPushButton *button = new QPushButton;
        button->setText(tempList.at(0));
        button->setFlat(true);
        button->setCursor(Qt::PointingHandCursor);
        layout->addWidget(imgLabel);
        layout->setAlignment(imgLabel,Qt::AlignHCenter);
        layout->addWidget(button);
        connect(button,&QPushButton::clicked,this,[=](){
            emit urlSignal(tempList.at(1));     //网页地址
        });
        gridLayout->addWidget(tempWidget,i/5,i%5);


        QString filePath = lizhiFMHttp->fileExist(tempList.at(2));
        QPixmap pix;
        if (filePath.isEmpty())         //if empty from url to get image
        {
            imgLabel->setObjectName(QString(tempList.at(2)).split('/').last());
            pix.load(":/imgs/noImg.jpg");
            imgLabel->setPixmap(pix);
            emit imgUrlSiganl(tempList.at(2));
        } else {                        //else form local to get image
            pix.load(filePath);
            imgLabel->setPixmap(pix);
        }


    }
    QHBoxLayout *hboxLayout = new QHBoxLayout;
    vboxLayout->addLayout(hboxLayout);
//    hboxLayout->setMargin(0);
//    hboxLayout->setSpacing(0);
    QPushButton *preBtn = new QPushButton;
    preBtn->setText("上一页");
    QPushButton *nextBtn = new QPushButton;
    nextBtn->setText("下一页");
    QLabel *pageLabel = new QLabel;
    pageLabel->setText(QString(" 第 %1 页 ").arg(page.at(2)));
    hboxLayout->addStretch();
    hboxLayout->addWidget(preBtn);
    hboxLayout->addWidget(pageLabel);
    hboxLayout->addWidget(nextBtn);
    hboxLayout->addStretch();

    connect(preBtn,&QPushButton::clicked,this,[=](){            //上一页槽函数
        if (!QString(page.at(0)).isEmpty())
        {
            qDebug() << "上一页:" << page.at(0);
            emit urlSignal(page.at(0));
        }else {
            QMessageBox box(QMessageBox::Warning,"注意","已经是第一页!");
            box.setStandardButtons(QMessageBox::Ok);
            box.setButtonText(QMessageBox::Ok,"确定");
            box.exec();
        }
    });

    connect(nextBtn,&QPushButton::clicked,this,[=](){       //下一页槽函数
        if (!QString(page.at(1)).isEmpty())
        {
            qDebug() << "下一页:" << page.at(1);
            emit urlSignal(page.at(1));
        }else {
            QMessageBox box(QMessageBox::Warning,"注意","已经是最后一页!");
            box.setStandardButtons(QMessageBox::Ok);
            box.setButtonText(QMessageBox::Ok,"确定");
            box.exec();
        }
    });

}
QStringList MainWidget::readPage(QString strUrl,QString html)       //读取上一页、下一页信息
{
    qDebug() << "strurl" <<strUrl;
    QStringList pageUrl;
    if (strUrl.contains("/hot") | strUrl.contains("/promo/")
            | strUrl.contains("/user/") | strUrl.contains("/tag/"))
    {

        int pageIndex = 1;
        QRegExp reg(".*html$");
        reg.setMinimal(true);
        if (reg.indexIn(strUrl) != -1)
        {
            pageIndex = QString(QString(strUrl.split('/').last()).split('.').at(0)).toInt();
        }
        QString url = strUrl.contains("/user/") ? strUrl.remove(QRegExp("/p/\\d+\\.html$"))
                                                : strUrl.remove(QRegExp("\\d+\\.html$"));
        qDebug() << "url" << url;
        if (pageIndex == 1)
        {
            pageUrl << "";
        } else if (pageIndex > 1)
        {
            int num = pageIndex;
            if (strUrl.contains("/user/"))
            {
                pageUrl << QString("%1/p/%2.html").arg(url).arg(--num);
            }else {
                pageUrl << QString("%1%2.html").arg(url).arg(--num);
            }
        }
        QRegExp rx(">下一页<");
        if (rx.indexIn(html) != -1)
        {
            int num = pageIndex;
            if (strUrl.contains("/user/"))
            {
                pageUrl << QString("%1/p/%2.html").arg(url).arg(++num);
            }else {
                pageUrl << QString("%1%2.html").arg(url).arg(++num);
            }
        }else {
            pageUrl << "";
        }
        pageUrl << QString("%1").arg(pageIndex);
        qDebug() << pageUrl;

    }
    return pageUrl;

}

void MainWidget::imgLabelSlot(QString str)          //set label image
{
    QLabel *label = tabWidget->findChild<QLabel *>(str.split('/').last());
    if (label != NULL && (!str.isEmpty()))
    {
     QPixmap pix(str);
     label->setPixmap(pix);
    }
}

QStringList MainWidget::hrefList(QStringList list)          //链接列表
{
    QStringList listTemp;
    for (int i = 0;i < list.size();i++)
    {
        QRegExp rx(">.*(?=<)");
        rx.setMinimal(true);
        rx.indexIn(list.at(i));
        QString value = rx.cap(0).replace(">","");
        rx.setPattern("/.*(?=\")");
        rx.indexIn(list.at(i));
        QString key = rx.cap(0);
        listTemp << key + "," +value;
    }
    return listTemp;
}

QStringList MainWidget::classList(QString html)         //节目类型列表
{
    QStringList list;

    QRegExp rx("【节目类型】.*</ul>");
     rx.setMinimal(true);
     rx.indexIn(html);
     QString contentStr = rx.cap(0);
     rx.setPattern("<a href=\".*\">.*</a>");
     int pos = 0;
     while ((pos = rx.indexIn(contentStr,pos)) != -1)
     {
         list << rx.cap(0);
         pos += rx.matchedLength();
     }

    return list;
}

QStringList MainWidget::user_sList(QString html)        //作者节目列表
{
    QStringList listTemp;
    QRegExp rx("audioList fontYaHei js-audio-list.*</u");
    rx.setMinimal(true);
    rx.indexIn(html);
    html = rx.cap(0);
    int pos = 0;
    rx.setPattern("<li>.*</li>");
    QRegExp rx_2;
    rx_2.setMinimal(true);
    while ((pos = rx.indexIn(html,pos)) != -1)
    {
        QString str = rx.cap(0);
        rx_2.setPattern("<a href=.*class");
        rx_2.indexIn(str);
        str = rx_2.cap(0);
        rx_2.setPattern("href=\".*(?=\")");
        rx_2.indexIn(str);
        QString url = rx_2.cap(0).remove(0,7).split('/').at(1);
        rx_2.setPattern("title=\".*(?=\")");
        rx_2.indexIn(str);
        QString title = rx_2.cap(0).remove(0,7);
        listTemp << url + ',' + title;
        pos += rx.matchedLength();
    }
    return listTemp;
}

void MainWidget::previousSlot()     //上一曲
{
    if (listCurrentIndex > 0)
    {
        fmPlayer->stop();
        readFlag = false;
        emit urlSignal(lizhi+QString("/media/url/")+playList.at(--listCurrentIndex));
    }
    qDebug() << "上一曲";
}

void MainWidget::nextSlot()     //下一曲
{
    if (listCurrentIndex < playList.size() - 1)
    {
        fmPlayer->stop();
        readFlag = false;
        emit urlSignal(lizhi+QString("/media/url/")+playList.at(++listCurrentIndex));
        qDebug() << "下一曲";
    }

}

void MainWidget::setBackgroundImage(QPixmap &pixmap)        //设置背景
{
    QPalette palette(this->palette());
    palette.setBrush(this->backgroundRole(),QBrush(pixmap.scaled(this->size(),
                                                   Qt::IgnoreAspectRatio,
                                                   Qt::SmoothTransformation)));
    this->setPalette(palette);
}

void MainWidget::resizeEvent(QResizeEvent *event)       //在窗口改变大小时重绘背景
{
    setBackgroundImage(pixmap);
    QWidget::resizeEvent(event);
}

void MainWidget::readData()     //读取配置
{
    QFile file("./.setting");
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Can`t open file or reading";
        return ;
    }
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_10);
    in >> playTitle >> playList >> listCurrentIndex
       >> position >> background;
    file.close();
    qDebug() << "in:" << playTitle << listCurrentIndex << playList;
    if ((!playTitle.isEmpty()) && (!playList.isEmpty()))
    {
        leftWidget->listWidget->clear();
        leftWidget->listWidget->addItems(playTitle);
        leftWidget->listWidget->setCurrentRow(listCurrentIndex);
        readFlag = true;
        emit urlSignal(lizhi+QString("/media/url/")+playList.at(listCurrentIndex));
    }
    if (!background.isEmpty())
    {
        pixmap.load(background);
        setBackgroundImage(pixmap);
    }
}

void MainWidget::writeData()        //保存配置
{
    QFile file("./.setting");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        qDebug() << "Can`t open file for writing";
        return;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_10);
    /*
     * 列表标题、列表url、列表当前播放位置、进度条位置、播放状态
    */
    qDebug() << "fmposition:" << fmPlayer->position() << "," << listCurrentIndex;
    out << playTitle << playList << listCurrentIndex
        << fmPlayer->position() << background;
    file.close();
}
