#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <QDebug>
#include <QGridLayout>
#include <QRegExp>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QObjectList>
#include <QPixmap>
#include <QMessageBox>

#define lizhi "http://www.lizhi.fm"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    layoutInit();       //初始化
    widget = new QWidget;
    tagWidget = new QWidget;

    fmPlayer = new QMediaPlayer(this);
    connect(fmPlayer,&QMediaPlayer::currentMediaChanged,this,[=](){
        qDebug() << "当前音乐改变";
        ui->pauseBtn_2->setIcon(QIcon(":/ico/pause.ico"));
        ui->listWidget->setCurrentRow(listCurrentIndex);
        if (playTitle.isEmpty())
        {
            ui->timeLabel->setText("荔枝FM");
        }else {
            QString str = QString("正在播放：").append(playTitle.at(listCurrentIndex));
            if (str.length() > 18)
            {
                str = str.left(18);
                str.append("...");
            }
            ui->titleLabel->setText(str);
        }

    });

    connect(fmPlayer,&QMediaPlayer::stateChanged,this,[=](){        //播放列表
        qDebug() << "state:" << fmPlayer->state();
        qDebug() << fmPlayer->mediaStatus();
        if (fmPlayer->mediaStatus() == QMediaPlayer::EndOfMedia)
        {
            nextSlot();
        }
    });

    connect(fmPlayer,&QMediaPlayer::positionChanged,this,[=](qint64 time){      //更新进度条
        int positionTime = int(time/1000);
        int totalTime = int(fmPlayer->duration() / 1000);
        int remainingTime = totalTime - positionTime;
        ui->timeSlider->setRange(0,totalTime);
        ui->timeSlider->setValue(positionTime);
        ui->timeLabel->setText(QString("%1:%2").arg(positionTime/60,2,10,QLatin1Char('0'))
                               .arg(positionTime%60,2,10,QLatin1Char('0')));
        ui->remainingTimeLabel->setText(QString("%1:%2").arg(remainingTime/60,2,10,QLatin1Char('0'))
                                        .arg(remainingTime%60,2,10,QLatin1Char('0')));
    });
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::closeEvent(QCloseEvent *)
{
    fmPlayer->stop();
    delete fmPlayer;
}

void MainWidget::layoutInit()
{
    connect(ui->previousBtn_2,&QPushButton::clicked,this,&MainWidget::previousSlot);      //上一曲
    connect(ui->nextBtn_2,&QPushButton::clicked,this,&MainWidget::nextSlot);      //下一曲
    connect(ui->pauseBtn_2,&QPushButton::clicked,[=](){       //暂停
        if (fmPlayer->state() == QMediaPlayer::PausedState)
        {
            fmPlayer->play();
            ui->pauseBtn_2->setIcon(QIcon(":/ico/pause.ico"));
        }else if (fmPlayer->state() == QMediaPlayer::PlayingState){
            fmPlayer->pause();
            ui->pauseBtn_2->setIcon(QIcon(":/ico/play.ico"));
        }else {
            QMessageBox box(QMessageBox::Warning,"注意","播放列表为空！");
            box.setStandardButtons(QMessageBox::Ok);
            box.setButtonText(QMessageBox::Ok,"确定");
            box.exec();
        }
        /*
        static bool flag = true;
        flag = !flag;
        if (flag)
        {

            fmPlayer->play();
            ui->pauseBtn_2->setIcon(QIcon(":/ico/pause.ico"));
        }else{
            fmPlayer->pause();
            ui->pauseBtn_2->setIcon(QIcon(":/ico/play.ico"));
        }*/
    });


    liZhiFMAPI = new LiZhiFMAPI(this);
    connect(this,&MainWidget::imgUrlSiganl,liZhiFMAPI,&LiZhiFMAPI::downloadSlot);
    connect(this,&MainWidget::urlSignal,liZhiFMAPI,&LiZhiFMAPI::urlSlot);
    connect(liZhiFMAPI,&LiZhiFMAPI::htmlSignal,this,&MainWidget::htmlSlot);
    connect(liZhiFMAPI,&LiZhiFMAPI::imgDownloadFinishedSiganl,this,&MainWidget::imgLabelSlot);

    QString str = "/hot/";      //热门电台
    QString url = lizhi + str;
    emit urlSignal(url);

    str = "/promo/";            //优选网络
    url = lizhi + str;
    emit urlSignal(url);

    str = "/sitemap.html";      //节目类型
    url = lizhi + str;
    emit urlSignal(url);
}

void MainWidget::htmlSlot(QString url, QString html)
{
    QStringList list = readPage(url,html);
    if (url.contains("/hot/"))      //热门电台
    {
        addListtoListWidget(list,radioNameList(html),ui->widget_3);

    }else if (url.contains("/promo/"))      //优选电台
    {
        addListtoListWidget(list,radioNameList(html),ui->widget_4);

    }else if (url.endsWith("/sitemap.html"))        //节目列表
    {
        QStringList list = classList(html);
        list = hrefList(list);
        QGridLayout *gridLayout = new QGridLayout(ui->widget_5);
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
    }else if (url.contains("/tag/")){
        QObjectList tagChildrenList = tagWidget->children();
        if (!tagChildrenList.isEmpty())
        {
            for (int i = 0;i < tagChildrenList.size();i++)
            {
                delete tagChildrenList.at(i);
            }
        }
        QWidget *tempWidget = new QWidget;
        addListtoListWidget(list,radioNameList(html),tempWidget);
        QVBoxLayout *tempvBoxLayout = new QVBoxLayout(tagWidget);
        QPushButton *back = new QPushButton;
        back->setText("返回");
        tempvBoxLayout->addWidget(tempWidget,10);
        tempvBoxLayout->addWidget(back,1);

        ui->verticalLayout_5->replaceWidget(ui->widget_5,tagWidget);
        ui->widget_5->setParent(NULL);

        connect(back,&QPushButton::clicked,this,[=](){
            ui->verticalLayout_5->replaceWidget(tagWidget,ui->widget_5);
            tagWidget->setParent(NULL);
        });


    }else if (url.contains("/user/"))           //作者内容列表
    {
        qDebug() << "收到user数据";
        QObjectList widgetChildrenList = widget->children();
        if (!widgetChildrenList.isEmpty())
        {
            for (int i = 0; i < widgetChildrenList.size();i++)
            {
//                qDebug() << widgetChildrenList.at(i);
                delete widgetChildrenList.at(i);
            }
        }
        QPushButton *backBtn = new QPushButton;
        backBtn->setText("返回");
        QPushButton *playBtn = new QPushButton;
        playBtn->setText("播放列表");
        QHBoxLayout *hBoxLayout = new QHBoxLayout;
        hBoxLayout->addWidget(backBtn);
        hBoxLayout->addWidget(playBtn);
        QVBoxLayout *vBoxLayout = new QVBoxLayout(widget);
        vBoxLayout->addLayout(hBoxLayout);
        QListWidget *listWidget = new QListWidget;
        vBoxLayout->addWidget(listWidget);

        QPushButton *preBtn = new QPushButton;
        preBtn->setText("上一页");
        QLabel *label = new QLabel;
        label->setText(QString(" 第 %1 页 ").arg(list.at(2)));
        QPushButton *nextBtn = new QPushButton;
        nextBtn->setText("下一页");
        QHBoxLayout *hBoxLayout_2 = new QHBoxLayout;
        hBoxLayout_2->addStretch();
        hBoxLayout_2->addWidget(preBtn);
        hBoxLayout_2->addWidget(label);
        hBoxLayout_2->addWidget(nextBtn);
        hBoxLayout_2->addStretch();

        vBoxLayout->addLayout(hBoxLayout_2);

        connect(preBtn,&QPushButton::clicked,this,[=](){
            if (!QString(list.at(0)).isEmpty())
            {
                qDebug() << "上一页:" << list.at(0);
                emit urlSignal(list.at(0));
            }else {
                QMessageBox box(QMessageBox::Warning,"注意","已经是第一页!");
                box.setStandardButtons(QMessageBox::Ok);
                box.setButtonText(QMessageBox::Ok,"确定");
                box.exec();
            }
        });

        connect(nextBtn,&QPushButton::clicked,this,[=](){
            if (!QString(list.at(1)).isEmpty())
            {
                qDebug() << "下一页" << list.at(1);
                emit urlSignal(list.at(1));
            } else {
                QMessageBox box(QMessageBox::Warning,"注意","已经是最后一页!");
                box.setStandardButtons(QMessageBox::Ok);
                box.setButtonText(QMessageBox::Ok,"确定");
                box.exec();
            }
        });

        QStringList list = user_sList(html);
        QStringList urlList;
        QStringList titleList;
        QStringList tempList;
        for (int i = 0; i < list.size(); i++)
        {
            tempList = QString(list.at(i)).split(',');
            urlList << tempList.at(0);
            titleList << tempList.at(1);
        }
        listWidget->addItems(titleList);
        connect(playBtn,&QPushButton::clicked,[=](){
            if (!playList.isEmpty())
            {
                playList.clear();
                playTitle.clear();
                ui->listWidget->clear();
                listCurrentIndex = 0;
            }
            playList = urlList;
            playTitle = titleList;
            ui->listWidget->addItems(titleList);
            emit urlSignal(lizhi+QString("/media/url/")+playList.at(listCurrentIndex));
        });
        connect(listWidget,&QListWidget::doubleClicked,[=](){
            qDebug() << urlList.at(listWidget->currentRow());
        });

        QWidget *tempWidget;
        QVBoxLayout *tempLay;

        switch (ui->tabWidget->currentIndex())
        {
            case 0:tempWidget = ui->widget_3;tempLay = ui->verticalLayout_3;break;
            case 1:tempWidget = ui->widget_4;tempLay = ui->verticalLayout_4;break;
            case 2:tempWidget = tagWidget;tempLay = ui->verticalLayout_5;break;
        }
        tempLay->replaceWidget(tempWidget,widget);
        tempWidget->setParent(NULL);
        connect(backBtn,&QPushButton::clicked,this,[=](){
            tempLay->replaceWidget(widget,tempWidget);
            widget->setParent(NULL);
            qDebug() << "返回";
        });
    }else if (html.startsWith("{\"rcode"))
    {
        QRegExp rx("http.*(?=\")");
        rx.setMinimal(true);
        rx.indexIn(html);
        QString url = rx.cap(0);
        fmPlayer->setMedia(QUrl(url));
        fmPlayer->play();
        qDebug() << url;
    }
//    qDebug() << "收到数据" << url;
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
//    qDebug() << list;
    return list;
}

QStringList MainWidget::classList(QString html)
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

QStringList MainWidget::hrefList(QStringList list)
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
        emit urlSignal(lizhi+QString("/media/url/")+playList.at(--listCurrentIndex));
        fmPlayer->stop();
    }
    qDebug() << "上一曲";
}

void MainWidget::nextSlot()     //下一曲
{
    if (listCurrentIndex < playList.size() - 1)
    {
        emit urlSignal(lizhi+QString("/media/url/")+playList.at(++listCurrentIndex));
        qDebug() << "下一曲";
        fmPlayer->stop();
    }

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
    vboxLayout->setMargin(0);
    vboxLayout->setSpacing(0);
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
        imgLabel->setObjectName(QString(tempList.at(2)).split('/').last());
        QPixmap pix(":/ico/noImg.jpg");
        imgLabel->setPixmap(pix);
//        QMovie *movie = new QMovie("./img/loading.gif");
//        imgLabel->setMovie(movie);
//        movie->start();
//        qDebug() << "imglabel objectnaem" << imgLabel->objectName();
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
//        qDebug() << tempList.at(2);
        emit imgUrlSiganl(tempList.at(2));
    }
    QHBoxLayout *hboxLayout = new QHBoxLayout;
    vboxLayout->addLayout(hboxLayout);
    hboxLayout->setMargin(0);
    hboxLayout->setSpacing(0);
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

    connect(preBtn,&QPushButton::clicked,this,[=](){
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

    connect(nextBtn,&QPushButton::clicked,this,[=](){
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

void MainWidget::imgLabelSlot(QString str)
{

//     qDebug() << "img" << str;
     QLabel *label = ui->tabWidget->findChild<QLabel *>(str.split('/').last());
//     qDebug() << "img;abel" << label;
//     qDebug() << "label" << label << str << "," << str.split('/').last();
        if (label != NULL && (!str.isEmpty()))
        {
             QPixmap pix(str);
             label->setPixmap(pix);
        }
}

QStringList MainWidget::readPage(QString strUrl,QString html)
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

