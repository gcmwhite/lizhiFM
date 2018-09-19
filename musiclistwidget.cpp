#include "musiclistwidget.h"
#include <QLabel>
#include <QPushButton>
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
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

void MusicListWidget::set_music_list_widget(const QVector<QStringList> &vec_list)
{
    head->removeRows(0,head->rowCount());
    QLabel *title_label = new QLabel;
    title_label->setAlignment(Qt::AlignCenter);
    title_label->setText(QString("<h2>%1</h2>").arg(vec_list.at(0).at(0)));
    QString next = vec_list.at(0).at(1);
    mainLayout->addWidget(title_label,1);
    mainLayout->addWidget(view,8);
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
    QHBoxLayout *hboxLayout = new QHBoxLayout;
    QPushButton *previous_btn = new QPushButton;
    QPushButton *next_btn = new QPushButton;
    QLabel *page_label = new QLabel;

    hboxLayout->setAlignment(Qt::AlignCenter);

    hboxLayout->addWidget(previous_btn);
    hboxLayout->addWidget(page_label);
    hboxLayout->addWidget(next_btn);
    mainLayout->addLayout(hboxLayout,1);

    page_label->setText("第 1 页");
    page_label->setAlignment(Qt::AlignCenter);

    previous_btn->setFlat(true);
    previous_btn->setEnabled(false);
    previous_btn->setFixedSize(25,25);
    previous_btn->setIcon(QIcon(":/imgs/previous_page.ico"));
    previous_btn->setIconSize(QSize(25,25));

    next_btn->setFlat(true);
    next_btn->setFixedSize(25,25);
    next_btn->setIcon(QIcon(":/imgs/next_page.ico"));
    next_btn->setIconSize(QSize(25,25));

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
