#include "footwidget.h"
#include <QHBoxLayout>

FootWidget::FootWidget(QWidget *parent) : QWidget(parent)
{
    current_time_label_ = new QLabel;
    remaining_time_label_ = new QLabel;
    time_slider_ = new QSlider(Qt::Horizontal);
    sound_slider_ = new QSlider(Qt::Horizontal);
    mute_btn_ = new QPushButton;

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(6);

    mainLayout->addWidget(current_time_label_);
    mainLayout->addWidget(time_slider_);
    mainLayout->addWidget(remaining_time_label_);
    mainLayout->addWidget(mute_btn_);
    mainLayout->addWidget(sound_slider_);

    current_time_label_->setText("00:00");
    remaining_time_label_->setText("00:00");

    mute_btn_->setFlat(true);
    mute_btn_->setFixedSize(30,30);
    mute_btn_->setIcon(QIcon(":/imgs/sound.ico"));
    mute_btn_->setIconSize(QSize(25,25));

    sound_slider_->setRange(0,100);
    sound_slider_->setValue(100);
    sound_slider_->setFixedWidth(60);
}
