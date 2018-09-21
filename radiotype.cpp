#include "radiotype.h"
#include <QPushButton>
#include <QDebug>

RadioType::RadioType(QWidget *parent) : QWidget(parent)
{
    mainLayout = new QGridLayout(this);
}

void RadioType::setRadioType(const QVector<QStringList> &vec_list)
{
    int size = vec_list.size();
    for (int i = 0;i < size;i++)
    {
        QStringList list = vec_list.at(i);
        if (list.isEmpty())
            continue;
        QPushButton *btn = new QPushButton(QString(list.at(1)));
        btn->setCursor(Qt::PointingHandCursor);
        connect(btn,&QPushButton::clicked,[=](){
            radio_type_btn_clicked_signal(list.at(0));
        });
        mainLayout->addWidget(btn,i/5,i%5);
    }
}
