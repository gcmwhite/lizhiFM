#include "radiotype.h"
#include <QPushButton>
#include <QDebug>
#include <QJsonArray>

RadioType::RadioType(QWidget *parent) : QWidget(parent)
{
    mainLayout = new QGridLayout(this);
}

void RadioType::setRadioType(const QJsonObject &json)
{
    QJsonArray js_array = json["list"].toArray();
    for (int i = 0;i < js_array.size();i++)
    {
        QJsonObject temp_json = js_array.at(i).toObject();
        QPushButton *btn = new QPushButton(temp_json["title"].toString());
        btn->setCursor(Qt::PointingHandCursor);
        connect(btn,&QPushButton::clicked,[=](){
            radio_type_btn_clicked_signal(temp_json["url"].toString());
        });
        mainLayout->addWidget(btn,i/5,i%5);
    }
}
