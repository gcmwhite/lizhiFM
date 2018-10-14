#include "mainwidget.h"
#include "QGridLayout"
#include "aboutwidget.h"
#include "donatewidget.h"
#include "skinwidget.h"
#include "config.h"
#include "update.h"
#include <QTimer>
#include <QJsonObject>
#include <QJsonArray>
#include <QPalette>
#include <QJsonDocument>
#include <QBrush>
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QApplication>
#include <QMessageBox>

MainWidget::MainWidget(QWidget *parent) : QWidget(parent)
{
    Config *config = new Config(this);
    QJsonObject configurationJson = config->read_config();
    qDebug() << "configr:" << configurationJson;

    if (!configurationJson.isEmpty())

    {
        QString skinTemp = configurationJson["skin"].toString();
        if (skinTemp.isEmpty())
            skin_ = ":/background/background_1.jpg";
        else
            skin_ = skinTemp;

        QString positionTemp = configurationJson["position"].toString();
        if (positionTemp.isEmpty())
            position_ = "0";
        else
            position_ = positionTemp;

        QString currentMusicTemp = configurationJson["currentMusic"].toString();
        if (currentMusicTemp.isEmpty())
            currentMusic = "0";
        else
            currentMusic = currentMusicTemp;

        playListJsonArray_ = configurationJson["playListJsonArray"].toArray();
    }

    init_ui_();
    init_network_();
    init_a_player_();

    QTimer *timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,[=](){
        new Update(version,this);
        timer->stop();
    });
    timer->start(30000);

}

MainWidget::~MainWidget()
{

    Config *config = new Config(this);
    QJsonObject configurationJson;          //配置信息
    configurationJson.insert("skin",skin_);
    if (aplayer->position() != 0)
        position_ = QString::number(aplayer->position()/1000);
    else
        position_ = "0";
    configurationJson.insert("position",position_);
    configurationJson.insert("currentMusic",QString::number(aplayer->get_current_index()));

    QJsonArray jsArray;
    for (int i = 0;i < vec_play_list.size();i++)
    {
        QStringList list = vec_play_list.at(i);
        QJsonObject tempJson;
        tempJson.insert("data-id",list.at(0));
        tempJson.insert("audioName",list.at(1));
        jsArray.append(tempJson);
    }
//    qDebug() << "vect:" << vec_play_list;

    configurationJson.insert("playListJsonArray",jsArray);

    qDebug() << "configw:" << configurationJson;
    config->create_config(configurationJson);

}

//主界面初始化
void MainWidget::init_ui_()
{
    this->resize(1100,590);
    this->setWindowIcon(QIcon(":/imgs/lizhi_favicon.ico"));
    this->setWindowTitle("荔枝FM 2.0 dev");

    leftWidget = new LeftWidget;
    footWidget = new FootWidget;
    tabWidget = new TabWidget;

    hot_grid_widget = new GridBtnWidget;
    optimization_grid_widget = new GridBtnWidget;
    radioType = new RadioType;

    tabWidget->addTab(hot_grid_widget,"热门电台");
    tabWidget->addTab(optimization_grid_widget,"优选电台");
    tabWidget->addTab(radioType,"节目类型");

    mainLayout = new QGridLayout(this);
    stackedLayout = new QStackedLayout;
    stackedLayout->setMargin(0);
    stackedLayout->addWidget(tabWidget);
    stackedLayout->setCurrentWidget(tabWidget);

    mainLayout->setMargin(10);
    mainLayout->setSpacing(5);

    mainLayout->addWidget(leftWidget,0,0);
    mainLayout->addLayout(stackedLayout,0,1);
    mainLayout->addWidget(footWidget,1,0,2,0);

    set_background_image(skin_);

    QDesktopWidget *desktop = QApplication::desktop();

    //绑定关于按钮
    connect(tabWidget,&TabWidget::aboutClicked,this,[=](){
        AboutWidget *aboutWidget = new AboutWidget;
        aboutWidget->setWindowIcon(QIcon(":/imgs/lizhi_favicon.ico"));

        int x = (desktop->width() - aboutWidget->width()) / 2;
        int y = (desktop->height() - aboutWidget->height()) / 2;

        aboutWidget->show();

        aboutWidget->move(x,y);
    });

    //绑定捐赠按钮
    connect(tabWidget,&TabWidget::donateClicked,this,[=](){
        DonateWidget *donateWidget = new DonateWidget;
        donateWidget->setWindowIcon(QIcon(":/imgs/lizhi_favicon.ico"));

        int x = (desktop->width() - donateWidget->width()) / 2;
        int y = (desktop->height() - donateWidget->height()) / 2;

        donateWidget->show();

        donateWidget->move(x,y);
    });

    //绑定Github按钮
    connect(tabWidget,&TabWidget::githubClicked,this,[=](){
        QDesktopServices::openUrl(QUrl("https://github.com/gcmwhite/lizhiFM"));
    });

    //绑定皮肤按钮
    SkinWidget *skinWidget = new SkinWidget;
    skinWidget->setWindowIcon(QIcon(":/imgs/lizhi_favicon.ico"));
    connect(skinWidget,&SkinWidget::backgroundSiganl,this,[=](const QString &skin_str){
        skin_ = skin_str;
        set_background_image(skin_);
    });
    connect(tabWidget,&TabWidget::skinClicked,this,[=](){
        int x = (desktop->width() - skinWidget->width()) / 2;
        int y = (desktop->height() - skinWidget->height()) / 2;

        skinWidget->show();

        skinWidget->move(x,y);

    });
}

//网络初始化
void MainWidget::init_network_()
{
    lizhiAPI = new LiZhiAPI(this);

    hot_grid_widget->set_grid_btn_widget(lizhiAPI->get_hot_grid());                                 //获取热门电台
    connect(tabWidget,&TabWidget::currentChanged,this,[=](int index){
        static bool ok[2] = {true,true};                    //热门电台、优选电台
        if (ok[0] && (index == 1))
        {
            optimization_grid_widget->set_grid_btn_widget(lizhiAPI->get_optimization_grid());           //获取优选电台
            ok[0] = false;
        }
        if (ok[1] && (index == 2))
        {
            radioType->setRadioType(lizhiAPI->get_radio_type());                                        //获取节目类别
            ok[1] = false;
        }
    });

    musicListWidget = new MusicListWidget;
    stackedLayout->addWidget(musicListWidget);

    tagWidget = new TagWidget;
    stackedLayout->addWidget(tagWidget);

    //绑定hot_grid_widget按钮
    connect(hot_grid_widget,&GridBtnWidget::grid_btn_signal,this,[=](const QString &path){
        musicListWidget->set_music_list_widget(lizhiAPI->get_music_list(path));
        stackedLayout->setCurrentWidget(musicListWidget);
    });

    //绑定optimization_grid_widget按钮
    connect(optimization_grid_widget,&GridBtnWidget::grid_btn_signal,this,[=](const QString &path){
        musicListWidget->set_music_list_widget(lizhiAPI->get_music_list(path));
        stackedLayout->setCurrentWidget(musicListWidget);
    });

    //绑定radioType按钮
    connect(radioType,&RadioType::radio_type_btn_clicked_signal,this,[=](const QString &path){
        tagWidget->set_tag_widget(lizhiAPI->get_tag_info_list(path));
        stackedLayout->setCurrentWidget(tagWidget);
    });

    //绑定tabWidget按钮
    connect(tagWidget->gridBtnWidget,&GridBtnWidget::grid_btn_signal,this,[=](const QString &path){
        musicListWidget->set_music_list_widget(lizhiAPI->get_music_list(path));
        stackedLayout->setCurrentWidget(musicListWidget);
    });

    //返回tabWidget
    connect(musicListWidget,&MusicListWidget::back_btn_clicked_signal,this,[=](){
        stackedLayout->setCurrentWidget(tabWidget);
    });

    //返回tabWidget
    connect(tagWidget,&TagWidget::back_btn_clicked_signal,this,[=](){
        stackedLayout->setCurrentWidget(tabWidget);
    });

    //绑定music list page changed
    connect(musicListWidget,&MusicListWidget::page_changed_signal,this,[=](const QString &path){
        musicListWidget->set_music_list_widget(lizhiAPI->get_music_list(path));
    });

    //绑定tag widget changed
    connect(tagWidget,&TagWidget::tag_page_changed,this,[=](const QString &path){
        tagWidget->set_tag_widget(lizhiAPI->get_tag_info_list(path));
//        stackedLayout->setCurrentWidget(tagWidget);
    });
}

//播放器初始化
void MainWidget::init_a_player_()
{
    aplayer = new Aplayer();

    //绑定右键菜单 添加到播放列表按钮
    connect(musicListWidget,&MusicListWidget::add_play_list_signal,this,[=](const bool ok,const QJsonObject &json){
        QJsonArray js_array = json["list"].toArray();
        int size = js_array.size();
        if (ok)
        {
            vec_play_list.clear();
            leftWidget->list_wigdet_->clear();
            vec_play_list.reserve(size);
        } else {
            if (leftWidget->list_wigdet_->item(0)->text() == "当前列表为空！")
                leftWidget->list_wigdet_->clear();
            vec_play_list.reserve(vec_play_list.size() + size);
        }
        for (int i = 0;i < size;i++)
        {
            QStringList list;
            const QJsonObject temp_json = js_array.at(i).toObject();
            const QString audioName = temp_json["audioName"].toString();
            list << temp_json["data-id"].toString();
            list << audioName;
            leftWidget->list_wigdet_->addItem(audioName);
            vec_play_list.append(list);
        }
        aplayer->add_play_list(ok,vec_play_list);

    });

    //播放当前音乐
    connect(musicListWidget,&MusicListWidget::play_current_music_signal,this,[=](const QStringList &list){
        qDebug() << "测试殿1";
        aplayer->a_play(list.at(0));
        this->setWindowTitle("荔枝FM 2.0 dev："+list.at(1));
    });

    connect(leftWidget->list_wigdet_,&QListWidget::doubleClicked,this,[=](){
        if (leftWidget->list_wigdet_->item(0)->text() == "当前列表为空！")
        {
            QMessageBox::warning(this,"注意","当前列表为空！",QMessageBox::Yes);
            return ;
        }
        int index = leftWidget->list_wigdet_->currentIndex().row();
        aplayer->set_current_index(index);
        aplayer->a_play(index);
    });

    //进度条控制
    static bool timeSliderPressFlag = false;
    connect(footWidget->time_slider_,&QSlider::sliderPressed,this,[=](){
        timeSliderPressFlag = true;
    });

    connect(footWidget->time_slider_,&QSlider::sliderReleased,this,[=](){
//        qDebug() << "timeSliderPressFlag:" <<  timeSliderPressFlag;
        if (timeSliderPressFlag)
        {
            aplayer->setPosition(qint64(footWidget->time_slider_->value()*1000));
            timeSliderPressFlag = false;
        }
    });

    //进度条显示
    connect(aplayer,&Aplayer::positionChanged,this,[=](qint64 position){
        int currentT = int(position/1000);
        int remainT = int((aplayer->duration()-position)/1000);
        if (position <= 3000)
        {
            footWidget->time_slider_->setRange(0,int(aplayer->duration()/1000));
        }
        if (!timeSliderPressFlag)
            footWidget->time_slider_->setValue(currentT);
        footWidget->current_time_label_->setText(QString("%1:%2").arg(currentT/60,2,10,QLatin1Char('0'))
                                    .arg(currentT%60,2,10,QLatin1Char('0')));
        footWidget->remaining_time_label_->setText(QString("%1:%2").arg(remainT/60,2,10,QLatin1Char('0'))
        .arg(remainT%60,2,10,QLatin1Char('0')));
    });

    //音量设置
    connect(footWidget->sound_slider_,&QSlider::valueChanged,this,[=](int sound){
        aplayer->setVolume(sound);
        if (sound <= 0)
        {
            footWidget->mute_btn_->setIcon(QIcon(":/imgs/mute.ico"));
        } else {
            footWidget->mute_btn_->setIcon(QIcon(":/imgs/sound.ico"));
        }
    });

    //静音
    connect(footWidget->mute_btn_,&QPushButton::clicked,this,[=](){
        static bool mute_flag = false;
        mute_flag = !mute_flag;
        aplayer->setMuted(mute_flag);
        if (mute_flag)
        {
            footWidget->mute_btn_->setIcon(QIcon(":/imgs/mute.ico"));
        } else {
            footWidget->mute_btn_->setIcon(QIcon(":/imgs/sound.ico"));
        }
    });

    //播放器控制
    connect(leftWidget->previous_btn_,&QPushButton::clicked,this,[=](){
        aplayer->a_previous();
    });

    connect(leftWidget->play_btn_,&QPushButton::clicked,this,[=](){
        aplayer->a_play();
    });

    connect(leftWidget->next_btn_,&QPushButton::clicked,this,[=](){
        aplayer->a_next();
    });

    //歌名显示
    static QString title;
    connect(aplayer,&Aplayer::current_media_title_changed,this,[=](const QString &t_title){
        title = t_title;
    });

    //播放按钮
    connect(aplayer,&Aplayer::stateChanged,this,[=](QMediaPlayer::State newState){
        qDebug() << "newstate:" << newState;
        switch (newState) {
        case QMediaPlayer::PlayingState:
            leftWidget->play_btn_->setIcon(QIcon(":/imgs/pause.ico"));
            this->setWindowTitle(QString("荔枝FM 2.0 dev：%1").arg(title));
            break;
        case QMediaPlayer::PausedState:
            leftWidget->play_btn_->setIcon(QIcon(":/imgs/play.ico"));
            this->setWindowTitle("荔枝FM 2.0 dev");
            break;
        case QMediaPlayer::StoppedState:
            if (aplayer->mediaStatus() == QMediaPlayer::EndOfMedia)
            {
                leftWidget->play_btn_->setIcon(QIcon(":/imgs/play.ico"));
                footWidget->time_slider_->setValue(0);
                footWidget->current_time_label_->setText("00:00");
                footWidget->remaining_time_label_->setText("00:00");
                this->setWindowTitle("荔枝FM 2.0 dev");
            }
            break;
        }
    });

    //当前播放位置
    connect(aplayer,&Aplayer::current_media_index_changed,this,[=](int index){
        leftWidget->list_wigdet_->setCurrentRow(index);
    });

    //leftwidget->listwidget
    //定位当前正在播放的曲目
    connect(leftWidget->position_,&QAction::triggered,this,[=](){
        leftWidget->list_wigdet_->setCurrentRow(aplayer->get_current_index());
    });

    //移除选中曲目
    connect(leftWidget->remove_,&QAction::triggered,this,[=](){
        if (leftWidget->list_wigdet_->item(0)->text() == "当前列表为空！")
            return ;
        QList<QListWidgetItem*> list = leftWidget->list_wigdet_->selectedItems();

        if (list.isEmpty())
        {
            leftWidget->list_wigdet_->addItem("当前列表为空！");
            return;
        }
        foreach (QListWidgetItem *item, list) {
//            qDebug() << "item:" << leftWidget->list_wigdet_->row(item);
            int index = leftWidget->list_wigdet_->row(item);
            aplayer->vec_play_list.removeAt(index);
            vec_play_list.removeAt(index);
            leftWidget->list_wigdet_->takeItem(index);
        }
    });

    //清空列表
    connect(leftWidget->clear_,&QAction::triggered,this,[=](){
        aplayer->vec_play_list.clear();
        vec_play_list.clear();
        leftWidget->list_wigdet_->clear();
        leftWidget->list_wigdet_->addItem("当前列表为空！");
    });

    //读取播放列表信息

    if (playListJsonArray_.isEmpty())
        return;

    vec_play_list.clear();
    leftWidget->list_wigdet_->clear();
    for (int i = 0;i < playListJsonArray_.size();i++)
    {
        QStringList list;
        const QJsonObject temp_json = playListJsonArray_.at(i).toObject();
        const QString audioName = temp_json["audioName"].toString();
        list << temp_json["data-id"].toString();
        list << audioName;
        leftWidget->list_wigdet_->addItem(audioName);
        vec_play_list.append(list);
    }

    aplayer->set_current_index(currentMusic.toInt());
    aplayer->add_play_list(false,vec_play_list);
//    aplayer->setPosition(qint64(position_.toInt()*1000));
    aplayer->pause();

}

//设置背景
void MainWidget::set_background_image(const QString &pix_url)
{
    QPixmap pix(pix_url);
    QPalette m_palette(this->palette());
    m_palette.setBrush(this->backgroundRole(),QBrush(pix.scaled(this->size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
    this->setPalette(m_palette);
}

void MainWidget::resizeEvent(QResizeEvent *event)
{
    set_background_image(skin_);
    QWidget::resizeEvent(event);
}


