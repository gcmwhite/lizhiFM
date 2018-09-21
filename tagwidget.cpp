#include "tagwidget.h"
#include <QHBoxLayout>

TagWidget::TagWidget(QWidget *parent) : QWidget(parent)
{
    previous_page_btn = new QPushButton;
    next_page_btn = new QPushButton;
    back_btn = new QPushButton;

    index_label = new QLabel;
    title_label = new QLabel;

    gridBtnWidget = new GridBtnWidget;

    mainLayout = new QVBoxLayout(this);

    index_label->setAlignment(Qt::AlignCenter);
    index_label->setText("地 1 页");
    title_label->setAlignment(Qt::AlignCenter);

    const QSize _M_SIZE(25,25);

    previous_page_btn->setFlat(true);
    previous_page_btn->setEnabled(false);
    previous_page_btn->setToolTip("上一页");
    previous_page_btn->setCursor(Qt::PointingHandCursor);
    previous_page_btn->setFixedSize(_M_SIZE);
    previous_page_btn->setIcon(QIcon(":/imgs/previous_page.ico"));
    previous_page_btn->setIconSize(_M_SIZE);

    next_page_btn->setFlat(true);
    next_page_btn->setToolTip("下一页");
    next_page_btn->setCursor(Qt::PointingHandCursor);
    next_page_btn->setFixedSize(_M_SIZE);
    next_page_btn->setIcon(QIcon(":/imgs/next_page.ico"));
    next_page_btn->setIconSize(_M_SIZE);

    back_btn->setFlat(true);
    back_btn->setToolTip("返回");
    back_btn->setCursor(Qt::PointingHandCursor);
    back_btn->setFixedSize(_M_SIZE);
    back_btn->setIcon(QIcon(":/imgs/back_.ico"));
    back_btn->setIconSize(_M_SIZE);

    QHBoxLayout *hBoxLayout_1 = new QHBoxLayout;
    QHBoxLayout *hBoxLayout_2 = new QHBoxLayout;

    hBoxLayout_1->setMargin(0);
    hBoxLayout_2->setMargin(0);
    hBoxLayout_2->setAlignment(Qt::AlignCenter);

    hBoxLayout_1->addWidget(back_btn);
    hBoxLayout_1->addWidget(title_label);

    hBoxLayout_2->addWidget(previous_page_btn);
    hBoxLayout_2->addWidget(index_label);
    hBoxLayout_2->addWidget(next_page_btn);

    mainLayout->setMargin(0);

    mainLayout->addLayout(hBoxLayout_1,1);
    mainLayout->addWidget(gridBtnWidget,8);
    mainLayout->addLayout(hBoxLayout_2,1);

    connect(back_btn,&QPushButton::clicked,this,[=](){
        emit back_btn_clicked_signal();
    });
}

void TagWidget::set_tag_widget(const QVector<QStringList> &vec_list)
{
    QVector<QStringList> temp_vec_list = vec_list;

    QStringList list = temp_vec_list.at(0);
    title_label->setText(QString("<h2>%1</h2>").arg(list.at(0)));

    //删除控件
    gridBtnWidget->clear_children();

    temp_vec_list.pop_front();
    gridBtnWidget->set_grid_btn_widget(temp_vec_list);

}
