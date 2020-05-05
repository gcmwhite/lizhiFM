#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "framelesstool.h"
#include "cornerwidget.h"
#include "radiolistwidget.h"
#include "radiopushbutton.h"
#include "audiolistwidget.h"
#include "audiolistitem.h"
#include "version.h"
#include "Common.h"
#include <QFile>
#include <QFontDatabase>
#include <QGridLayout>
#include <QScrollArea>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

QFont LiZhiFM::iconFont;

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    init();
}

MainWidget::~MainWidget()
{
    delete ui;
    player_->stop();
    playerThread_->quit();
    playerThread_->wait();
}

 //根据解析数据创建窗体
void MainWidget::slotParseFinished(const PageData &pageData)
{
    PageData::PageStatus pageStatus = pageData.getPageStatus();
    if (!pageStatus.status) {
        return ;
    }
    switch (pageStatus.parseWay) {
    case PageData::PARSE_WAY::AUDIO_LIST: {
        createAudioListWidget(pageData);
        break ;
    }
    case PageData::PARSE_WAY::RADIO_LIST: {
        createRadioListWidget(pageData);
        break ;
    }
    case PageData::PARSE_WAY::ALL_RADIO_TAG: {
        createRadioTagWidget(pageData);
        break ;
    }
    }

}

//当前选中Tab改变
void MainWidget::slotcurrentTabChanged(int index)
{
//    qDebug() << "tab changed:" << index;
    static bool firstIn[3] { true,true,true };
    switch (index) {
    case 0: {
        if (firstIn[0]) {
            lizhiFmApi_->getHot();
        }
        break ;
    }
    case 1: {
        if (firstIn[1]) {
            lizhiFmApi_->getPromo();
        }
        break ;
    }
    case 2: {
        if (firstIn[2]) {
            lizhiFmApi_->getAllRadioTag();
        }
        break ;
    }
    }
    firstIn[index] = false;
}

//播放当前列表
void MainWidget::slotPlayItems(const QVector<QPair<QString, QString> > &items)
{
    mediaListModel_->updateModel(items);
    QModelIndex index = mediaListModel_->index(0);
    lizhiFmApi_->getMediaUrl(mediaListModel_->getUrl(index));
    ui->listView->setCurrentIndex(index);
}

//播放当前项
void MainWidget::slotPlayCurrentItem(const QModelIndex &index)
{
    autoNext_ = false;
    mediaListModel_->setCurrentPlay(index.row());
    ui->listView->setCurrentIndex(index);
    QPair<QString,QString> item = mediaListModel_->data(index,Qt::UserRole).value<QPair<QString,QString>>();
    lizhiFmApi_->getMediaUrl(item.second);
    ui->titleLabel->setText(QString("%1 v%2 正在播放：%3").arg(APPNAME).arg(VERSION).arg(item.first));
}

//播放
void MainWidget::slotPlay(const QString &mediaUrl)
{
    player_->setMedia(QUrl(mediaUrl));
    player_->play();
}

//播放时长发生改变
void MainWidget::slotDurationChanged(const qint64 duration)
{
    ui->timeSlider->setMaximum(duration);
}

//播放位置发生改变
void MainWidget::slotPositionChanged(const qint64 position)
{
    if (timeSilder_) {
        ui->timeSlider->setValue(position);
    }
    qint64 totalTime = player_->duration();
    qint64 time = position;
    qint64 remainingTime = totalTime - time;
    ui->remainingTimeLabel->setText(QString("%1:%2")
                                    .arg(remainingTime / 60000,2,10,QLatin1Char('0'))
                                    .arg(remainingTime / 1000 % 60,2,10,QLatin1Char('0')));
    ui->timeLabel->setText(QString("%1:%2")
                           .arg(time / 60000,2,10,QLatin1Char('0'))
                           .arg(time / 1000 % 60,2,10,QLatin1Char('0')));
}

//播放状态发生改变
void MainWidget::slotStateChanged(QMediaPlayer::State state)
{
    aWarning() << "state:" << state;
    switch (state) {
    case QMediaPlayer::StoppedState: {
        ui->playBtn->setText("\ue658");
        if (autoNext_) {
            on_nextBtn_clicked();
        }
        autoNext_ = true;
        break ;
    }
    case QMediaPlayer::PausedState: {
        ui->playBtn->setText("\ue658");
        break ;
    }
    case QMediaPlayer::PlayingState: {
        ui->playBtn->setText("\ue603");
        break ;
    }
    }
}

//清空子窗体
void MainWidget::clearChildren(QWidget *parent)
{
    QList<QWidget *> list = parent->findChildren<QWidget *>(QString(),Qt::FindDirectChildrenOnly);
    for (auto w : list) {
        aLog() << "delete widget:" << w;
        w->deleteLater();
    }
}

//初始化样式
void MainWidget::initStyle()
{
    QFile file(QStringLiteral(":/Resources/styles/default.qss"));
    file.open(QIODevice::ReadOnly);
    QByteArray bytes = file.readAll();
    file.close();
    qApp->setStyleSheet(bytes);
    int fontId = QFontDatabase::addApplicationFont(QStringLiteral(":/Resources/fonts/iconfont.ttf"));
    LiZhiFM::iconFont.setFamily(QFontDatabase::applicationFontFamilies(fontId).first());
    ui->minBtn->setFont(LiZhiFM::iconFont);
    ui->closeBtn->setFont(LiZhiFM::iconFont);
    ui->soundBtn->setFont(LiZhiFM::iconFont);
    ui->preBtn->setFont(LiZhiFM::iconFont);
    ui->playBtn->setFont(LiZhiFM::iconFont);
    ui->nextBtn->setFont(LiZhiFM::iconFont);
    QFont font = qApp->font();
    font.setPixelSize(11);
    qApp->setFont(font);
}

//播放器初始化
void MainWidget::initPlayer()
{

    player_ = std::make_shared<QMediaPlayer>();
    playerThread_ = new QThread(this);
    player_->moveToThread(playerThread_);
    playerThread_->start();
    ui->soundSlider->setValue(80);

    connect(ui->listView,&QListView::doubleClicked,this,&MainWidget::slotPlayCurrentItem);
    connect(lizhiFmApi_,&LiZhiFmAPI::signalPlayMedia,this,&MainWidget::slotPlay);
    connect(player_.get(),&QMediaPlayer::durationChanged,this,&MainWidget::slotDurationChanged);
    connect(player_.get(),&QMediaPlayer::positionChanged,this,&MainWidget::slotPositionChanged);
    connect(player_.get(),&QMediaPlayer::stateChanged,this,&MainWidget::slotStateChanged);
}

void MainWidget::init()
{
    //注册信号类型
    qRegisterMetaType<PageData>("PageData");
    qRegisterMetaType<QList<QPair<QByteArray,QByteArray>>>("QList<QPair<QByteArray,QByteArray>>");

    //界面相关
    ui->timeSlider->setMinimum(0);
    ui->hotTab->setLayout(new QVBoxLayout(ui->hotTab));
    ui->promoTab->setLayout(new QVBoxLayout(ui->promoTab));
    ui->moreTab->setLayout(new QVBoxLayout(ui->moreTab));

    initStyle();

    ui->titleLabel->setText(QString("%1 v%2").arg(APPNAME).arg(VERSION));
    ui->tabWidget->setCornerWidget(new CornerWidget(this));

    FrameLessTool* frameLess = new FrameLessTool(this);
    frameLess->setTitleWidget(ui->titleWidget);

    mediaListModel_ = new MediaListModel(this);
    ui->listView->setModel(mediaListModel_);
    ui->listView->setAlternatingRowColors(true);

    //Tab切换
    connect(ui->tabWidget,&QTabWidget::currentChanged,this,&MainWidget::slotcurrentTabChanged);

    //网络相关
    lizhiFmApi_ = new LiZhiFmAPI(this);
    connect(lizhiFmApi_,&LiZhiFmAPI::signalParseFinished,this,&MainWidget::slotParseFinished);

    //加载热门
    emit ui->tabWidget->currentChanged(0);


    //测试
//    AudioListWidget* lw = new AudioListWidget;
//    lw->show();
//    lizhiFmApi_->UrlGet("https://www.lizhi.fm/user/12474765");

    //播放器初始化
    initPlayer();
}

//创建电台列表窗体
void MainWidget::createRadioListWidget(const PageData &pageData)
{
    PageData::PageStatus pageStatus = pageData.getPageStatus();
    RadioListWidget* radioListWidget{nullptr};
    QWidget* tabWidget{nullptr};
    aLog() << "解析地址:" << pageStatus.requestUrl;
    if (pageStatus.requestUrl.contains(HOT_URL_KEY)) {
        tabWidget = ui->hotTab;
        clearChildren(tabWidget);
        radioListWidget = new RadioListWidget(tabWidget);
    }else if (pageStatus.requestUrl.contains(PROMO_URL_KEY)) {
        tabWidget = ui->promoTab;
        clearChildren(tabWidget);
        radioListWidget = new RadioListWidget(tabWidget);
    }else if (pageStatus.requestUrl.contains(RADIO_URL_KEY)) {
        tabWidget = ui->moreTab;
        clearChildren(tabWidget);
        radioListWidget = new RadioListWidget(tabWidget);
        radioListWidget->setHeadWidgetVisible(true);
        radioListWidget->setTitleLabel(pageStatus.title);
        connect(radioListWidget,&RadioListWidget::signalBackBtnClicked,
                radioListWidget,[this](){
            lizhiFmApi_->getAllRadioTag();
        });
    }
    radioListWidget->setPrevBtnEnable(pageStatus.prev);
    radioListWidget->setNextBtnEnable(pageStatus.next);
    radioListWidget->setCurrentPage(pageStatus.current);
    QString baseUrl = pageStatus.requestUrl;
    if (baseUrl.endsWith(".html")) {
        QStringList list = baseUrl.split('/');
        list.removeLast();
        baseUrl = list.join('/') + "/";
    }
    if (2 == pageStatus.current) {
        radioListWidget->setPrecvBtnPropertyVar("url",baseUrl);
    }else {
        radioListWidget->setPrecvBtnPropertyVar("url",QString("%1%2.html").arg(baseUrl).arg(pageStatus.current-1));
    }
    radioListWidget->setNextBtnPropertyVar("url",QString("%1%2.html").arg(baseUrl).arg(pageStatus.current+1));
    tabWidget->layout()->addWidget(radioListWidget);

    QWidget* centralWidget = radioListWidget->getCentralWidget();
    QGridLayout* gridLayout = new QGridLayout(centralWidget);
    QVector<PageData::Adata> dataVec = pageData.getData();
    int count { 0 };
    RadioPushButton* btn{nullptr};
    for (const auto& data : dataVec) {
        if (data.parse) {
            btn = new RadioPushButton(centralWidget);
            btn->setStr(data.dataUserName);
            btn->setPix(data.dataCover);
            btn->setProperty("title",data.dataUserName);
            btn->setProperty("url",data.url);
            connect(btn,&RadioPushButton::clicked,btn,[btn,this](){
                lizhiFmApi_->UrlGet(btn->property("url").toString());
            });
        }else {
            continue ;
        }
        gridLayout->addWidget(btn,count/5,count%5);
        ++count;
    }

    //加载上一页
    connect(radioListWidget,&RadioListWidget::signalPrecvBtnClicked,radioListWidget,[radioListWidget,this](){
        lizhiFmApi_->UrlGet(radioListWidget->getPrecvBtnPropertyVar("url").toString());
    });

    //加载下一页
    connect(radioListWidget,&RadioListWidget::signalNextBtnClicked,radioListWidget,[radioListWidget,this](){
        lizhiFmApi_->UrlGet(radioListWidget->getNextBtnPropertyVar("url").toString());
    });
}

//创建电台分类窗体
void MainWidget::createRadioTagWidget(const PageData &pageData)
{
    clearChildren(ui->moreTab);
    PageData::PageStatus pageStatus = pageData.getPageStatus();
    QScrollArea* scrollArea = new QScrollArea(ui->moreTab);
    ui->moreTab->layout()->addWidget(scrollArea);
    QWidget* contentW = new QWidget(scrollArea);
    scrollArea->setWidget(contentW);
    QSize size = ui->tabWidget->size();
    contentW->setMinimumSize(size.width() - 35,size.height());
    QVector<PageData::Adata> dataVec = pageData.getData();
    QVBoxLayout* mainLayout = new QVBoxLayout(contentW);
    for (const auto& data : dataVec) {
        QLabel* label = new QLabel(data.dataTitle,contentW);
        mainLayout->addWidget(label);
        QGridLayout* gridLayout = new QGridLayout;
        gridLayout->setAlignment(Qt::AlignLeft);
        int count { 0 };
        for (const auto &tuple : data.radioTag) {
            QPushButton* btn = new QPushButton(std::get<0>(tuple),contentW);
            btn->setProperty("url",std::get<1>(tuple));
            gridLayout->addWidget(btn,count/8,count%8);
            ++count;
            connect(btn,&QPushButton::clicked,contentW,[btn,this](){
                QString url = btn->property("url").toString();
                if (url.contains(RADIO_URL_KEY)) {
                    LabelHistoryUrl_ = url;
                }
                lizhiFmApi_->UrlGet(url);
            });
        }
        mainLayout->addLayout(gridLayout);
    }
}

//创建声音列表窗体
void MainWidget::createAudioListWidget(const PageData &pageData)
{
    PageData::PageStatus pageStatus = pageData.getPageStatus();
    QWidget* currentW = ui->tabWidget->currentWidget();
    clearChildren(currentW);
    AudioListWidget* audioListWidget = new AudioListWidget(currentW);
    currentW->layout()->addWidget(audioListWidget);
    audioListWidget->setTitleLabel(pageStatus.title);
    audioListWidget->setCurrentPage(pageStatus.current);
    audioListWidget->setPrevBtnEnable(pageStatus.prev);
    audioListWidget->setNextBtnEnable(pageStatus.next);
    if (ui->hotTab == currentW) {
        audioListWidget->setBackBtnPropertyVar("url",BASE_URL+HOT_URL_KEY);
    }else if(ui->promoTab == currentW) {
        audioListWidget->setBackBtnPropertyVar("url",BASE_URL+PROMO_URL_KEY);
    }else if (ui->moreTab == currentW) {
        audioListWidget->setBackBtnPropertyVar("url",LabelHistoryUrl_);
    }
    QString baseUrl = pageStatus.requestUrl;
    if (baseUrl.endsWith(".html")) {
        QStringList list = baseUrl.split('/');
        list.removeLast();
        list.removeLast();
        baseUrl = list.join('/');
    }
    if (2 == pageStatus.current) {
        audioListWidget->setPrecvBtnPropertyVar("url",baseUrl);
    }else {
        audioListWidget->setPrecvBtnPropertyVar("url",QString("%1/p/%2.html").arg(baseUrl).arg(pageStatus.current-1));
    }
    audioListWidget->setNextBtnPropertyVar("url",QString("%1/p/%2.html").arg(baseUrl).arg(pageStatus.current+1));
    QVector<PageData::Adata> dataVec = pageData.getData();
    QVector<AudioListItem *> itemVec;
    for (const auto& data : dataVec) {
        AudioListItem* item = new AudioListItem(audioListWidget);
        item->setIndex(data.itemIndex);
        item->setTitle(data.dataTitle);
        item->setTime(data.dataDuration);
        item->setDate(data.audioTime);
        item->setUrl(data.url);
        itemVec.append(item);
    }
    audioListWidget->addItems(itemVec);

    connect(audioListWidget,&AudioListWidget::signalBackBtnClicked,audioListWidget,[audioListWidget,this](){
        lizhiFmApi_->UrlGet(audioListWidget->getBackPropertyVar("url").toString());
    });
    connect(audioListWidget,&AudioListWidget::signalPrecvBtnClicked,audioListWidget,[audioListWidget,this](){
        lizhiFmApi_->UrlGet(audioListWidget->getPrevPropertyVar("url").toString());
    });
    connect(audioListWidget,&AudioListWidget::signalNextBtnClicked,audioListWidget,[audioListWidget,this](){
        lizhiFmApi_->UrlGet(audioListWidget->getNextPropertyVar("url").toString());
    });
    connect(audioListWidget,&AudioListWidget::signalAddItems,mediaListModel_,&MediaListModel::addItems);
    connect(audioListWidget,&AudioListWidget::signalPlayItems,this,&MainWidget::slotPlayItems);
    connect(audioListWidget,&AudioListWidget::signalPlay,this,[this](const QPair<QString,QString>& item){
        autoNext_ = false;
        ui->titleLabel->setText(QString("%1 v%2 正在播放：%3").arg(APPNAME).arg(VERSION).arg(item.first));
        lizhiFmApi_->getMediaUrl(item.second);
    });
    connect(audioListWidget,&AudioListWidget::signalAddItem,mediaListModel_,&MediaListModel::addItem);
}

void MainWidget::on_soundBtn_clicked()
{
    static int originSound{30};
    if (0 == ui->soundSlider->value()) {
        ui->soundSlider->setValue(originSound);
    }else {
        originSound = ui->soundSlider->value();
        ui->soundSlider->setValue(0);
    }
}

void MainWidget::on_soundSlider_valueChanged(int value)
{
    player_->setVolume(value);
    if (0 == value) {
        ui->soundBtn->setText("\ue606");
    }else if (50 >= value) {
        ui->soundBtn->setText("\ue605");
    }else if (80 >= value) {
        ui->soundBtn->setText("\ue604");
    }
}

void MainWidget::on_playBtn_clicked()
{
    if (QMediaPlayer::PlayingState == player_->state()) {
        player_->pause();
    }else {
        if (player_->isAudioAvailable()) {
            player_->play();
        }else if (0 != mediaListModel_->rowCount()) {
            slotPlayCurrentItem(mediaListModel_->index(mediaListModel_->getCurrentPlay()));
        }
    }
}

void MainWidget::on_preBtn_clicked()
{
    autoNext_ = false;
    int index = mediaListModel_->getCurrentPlay() - 1;
    if (index >= 0) {
        QPair<QString,QString> item = mediaListModel_->data(index,Qt::UserRole).value<QPair<QString,QString>>();
        ui->titleLabel->setText(QString("%1 v%2 正在播放：%3").arg(APPNAME).arg(VERSION).arg(item.first));
        lizhiFmApi_->getMediaUrl(item.second);
        mediaListModel_->setCurrentPlay(index);
        ui->listView->setCurrentIndex(mediaListModel_->index(index));
    }
}

void MainWidget::on_nextBtn_clicked()
{
    autoNext_ = false;
    int index = mediaListModel_->getCurrentPlay() + 1;
    if (index < mediaListModel_->rowCount()) {
        QPair<QString,QString> item = mediaListModel_->data(index,Qt::UserRole).value<QPair<QString,QString>>();
        ui->titleLabel->setText(QString("%1 v%2 正在播放：%3").arg(APPNAME).arg(VERSION).arg(item.first));
        lizhiFmApi_->getMediaUrl(item.second);
        mediaListModel_->setCurrentPlay(index);
        ui->listView->setCurrentIndex(mediaListModel_->index(index));
    }
}

void MainWidget::on_timeSlider_sliderReleased()
{
    player_->setPosition(ui->timeSlider->value());
    timeSilder_ = true;
}

void MainWidget::on_timeSlider_sliderPressed()
{
    timeSilder_ = false;
}
