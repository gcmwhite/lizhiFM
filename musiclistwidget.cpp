#include "musiclistwidget.h"
#include <QHBoxLayout>
#include <QStandardItem>
#include <QAbstractItemView>
#include <QHeaderView>
#include <QDebug>

MusicListWidget::MusicListWidget(QWidget *parent) : QWidget(parent)
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(0);
    QStringList list;
    list << "名称" << "时间" << "时长";
    head = new QStandardItemModel;
    head->setHorizontalHeaderLabels(list);
    view = new QTableView;
    view->setModel(head);
    view->setAlternatingRowColors(true);
    view->setShowGrid(false);
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    const QSize _SIZE_(25,25);

    back_btn = new QPushButton;
    title_label = new QLabel;

    QHBoxLayout *hboxLayout_1 = new QHBoxLayout;
    hboxLayout_1->setMargin(0);
    hboxLayout_1->addWidget(back_btn);
    hboxLayout_1->addWidget(title_label);

    back_btn->setFlat(true);
    back_btn->setToolTip("返回");
    back_btn->setFixedSize(_SIZE_);
    back_btn->setIcon(QIcon(":/imgs/back_.ico"));
    back_btn->setIconSize(_SIZE_);

    title_label->setAlignment(Qt::AlignCenter);

    QHBoxLayout *hboxLayout_2 = new QHBoxLayout;
    previous_btn = new QPushButton;
    next_btn = new QPushButton;
    page_label = new QLabel;

    hboxLayout_2->setAlignment(Qt::AlignCenter);

    hboxLayout_2->addWidget(previous_btn);
    hboxLayout_2->addWidget(page_label);
    hboxLayout_2->addWidget(next_btn);

    page_label->setText("第 1 页");
    page_label->setAlignment(Qt::AlignCenter);

    previous_btn->setFlat(true);
    previous_btn->setToolTip("上一页");
    previous_btn->setEnabled(false);
    previous_btn->setFixedSize(_SIZE_);
    previous_btn->setIcon(QIcon(":/imgs/previous_page.ico"));
    previous_btn->setIconSize(_SIZE_);

    next_btn->setFlat(true);
    next_btn->setToolTip("下一页");
    next_btn->setFixedSize(_SIZE_);
    next_btn->setIcon(QIcon(":/imgs/next_page.ico"));
    next_btn->setIconSize(_SIZE_);

    mainLayout->addLayout(hboxLayout_1,1);
    mainLayout->addWidget(view,8);
    mainLayout->addLayout(hboxLayout_2,1);

    //返回
    connect(back_btn,&QPushButton::clicked,this,[=](){
        emit back_btn_clicked_signal();
    });
}

void MusicListWidget::set_music_list_widget(const QVector<QStringList> &vec_list)
{

    head->removeRows(0,head->rowCount());

    previous_btn->disconnect();
    next_btn->disconnect();

    title_label->setText(QString("<h2>%1</h2>").arg(vec_list.at(0).at(0)));

    QString next = vec_list.at(0).at(1);

    int size = vec_list.size();
    for (int i = 1;i < size;i++)
    {
        QStringList list = vec_list.at(i);
        if (list.isEmpty())
            continue;
        head->setItem(i-1,0,new QStandardItem(QString(list.at(1))));
        head->setItem(i-1,1,new QStandardItem(QString(list.at(2))));
        head->setItem(i-1,2,new QStandardItem(QString(list.at(3))));
    }

    //上一页
    connect(previous_btn,&QPushButton::clicked,this,[=](){
        if (index_page <= 2)
        {
            previous_btn->setEnabled(false);
        }
        else
        {
            page_label->setText(QString("第 %1 页").arg(--index_page));
        }
        if (next.contains(">下一页<"))
        {
            next_btn->setEnabled(true);
        }
        qDebug() << "上一页";
    });

    //下一页
    connect(next_btn,&QPushButton::clicked,this,[=](){
        if (next.contains(">下一页<"))
        {
            page_label->setText(QString("第 %1 页").arg(++index_page));
        } else
        {
            next_btn->setEnabled(false);
        }
        if (index_page > 1)
        {
            previous_btn->setEnabled(true);
        }
        qDebug() << "下一页";
    });

}
