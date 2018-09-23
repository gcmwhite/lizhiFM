#include "mainwidget.h"
#include "QGridLayout"
#include <QTimer>

MainWidget::MainWidget(QWidget *parent) : QWidget(parent)
{
    init_ui_();
    init_network_();
    init_a_player_();
}

//主界面初始化
void MainWidget::init_ui_()
{
    this->resize(990,590);
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

}

//网络初始化
void MainWidget::init_network_()
{
    lizhiAPI = new LiZhiAPI(this);

    QTimer *timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,[=](){
        static int timer_count = 0;
        if (timer_count == 0)
        {
            radioType->setRadioType(lizhiAPI->get_radio_type());                                        //获取节目类别
        }else if (timer_count == 1) {
            hot_grid_widget->set_grid_btn_widget(lizhiAPI->get_hot_grid());                             //获取热门电台
            timer->setInterval(500);
        }else if (timer_count == 2)
        {
            optimization_grid_widget->set_grid_btn_widget(lizhiAPI->get_optimization_grid());           //获取优选电台
            timer->stop();
            timer->disconnect();
            timer->setParent(nullptr);
            timer->deleteLater();
        }
        timer_count++;
    });
    timer->start(50);

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
    connect(musicListWidget,&MusicListWidget::add_play_list_signal,this,[=](const bool ok,const QVector<QStringList> &vec_list){
        aplayer->add_play_list(ok,vec_list);
        if (ok)
        {
            leftWidget->list_wigdet_->clear();
        } else {
            if (leftWidget->list_wigdet_->item(0)->text() == "当前列表为空！")
            {
                leftWidget->list_wigdet_->clear();
            }
        }
        for (int i = 1;i < vec_list.size();i++)
        {
            QStringList list = vec_list.at(i);
            leftWidget->list_wigdet_->addItem(list.at(1));
        }
    });

    //进度条控制
    static bool timeSliderPressFlag = false;
    connect(footWidget->time_slider_,&QSlider::sliderPressed,this,[=](){
        timeSliderPressFlag = true;
    });

    connect(footWidget->time_slider_,&QSlider::sliderReleased,this,[=](){
        timeSliderPressFlag = false;
    });

    //进度条显示
    connect(aplayer,&Aplayer::positionChanged,this,[=](qint64 position){
        int currentT = int(position/1000);
        int remainT = int((aplayer->duration()-position)/1000);
        if (position == 0)
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
        switch (newState) {
        case QMediaPlayer::PlayingState:
            leftWidget->play_btn_->setIcon(QIcon(":/imgs/pause.ico"));
            this->setWindowTitle(QString("荔枝FM 2.0 dev：%1").arg(title));
            break;
        case QMediaPlayer::PausedState:
            leftWidget->play_btn_->setIcon(QIcon(":/icons/player_play.ico"));
            this->setWindowTitle("荔枝FM 2.0 dev");
        case QMediaPlayer::StoppedState:
            leftWidget->play_btn_->setIcon(QIcon(":/imgs/play.ico"));
            footWidget->time_slider_->setValue(0);
            footWidget->current_time_label_->setText("00:00");
            footWidget->remaining_time_label_->setText("00:00");
            this->setWindowTitle("荔枝FM 2.0 dev");
            break;
        }
    });

    //当前播放位置
    connect(aplayer,&Aplayer::current_media_index_changed,this,[=](int index){
        qDebug() << "indexp:" << index;
        leftWidget->list_wigdet_->setCurrentRow(index);
    });

}

