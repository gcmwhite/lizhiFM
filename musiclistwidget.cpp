#include "musiclistwidget.h"
#include <QHBoxLayout>
#include <QStandardItem>
#include <QAbstractItemView>
#include <QHeaderView>
#include <QDebug>
#include <QRegExp>
#include <QAction>
#include <QDialog>

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

    const QSize _M_SIZE_(25,25);

    back_btn = new QPushButton;
    title_label = new QLabel;

    QHBoxLayout *hboxLayout_1 = new QHBoxLayout;
    hboxLayout_1->setMargin(0);
    hboxLayout_1->addWidget(back_btn);
    hboxLayout_1->addWidget(title_label);

    back_btn->setFlat(true);
    back_btn->setCursor(Qt::PointingHandCursor);
    back_btn->setToolTip("返回");
    back_btn->setFixedSize(_M_SIZE_);
    back_btn->setIcon(QIcon(":/imgs/back_.ico"));
    back_btn->setIconSize(_M_SIZE_);

    title_label->setAlignment(Qt::AlignCenter);

    QHBoxLayout *hboxLayout_2 = new QHBoxLayout;
    previous_btn = new QPushButton;
    next_btn = new QPushButton;
    page_label = new QLabel;

    hboxLayout_2->setAlignment(Qt::AlignCenter);

    hboxLayout_2->addWidget(previous_btn);
    hboxLayout_2->addWidget(page_label);
    hboxLayout_2->addWidget(next_btn);

    page_label->setAlignment(Qt::AlignCenter);

    previous_btn->setFlat(true);
    previous_btn->setCursor(Qt::PointingHandCursor);
    previous_btn->setToolTip("上一页");
    previous_btn->setEnabled(false);
    previous_btn->setFixedSize(_M_SIZE_);
    previous_btn->setIcon(QIcon(":/imgs/previous_page.ico"));
    previous_btn->setIconSize(_M_SIZE_);

    next_btn->setFlat(true);
    next_btn->setCursor(Qt::PointingHandCursor);
    next_btn->setToolTip("下一页");
    next_btn->setFixedSize(_M_SIZE_);
    next_btn->setIcon(QIcon(":/imgs/next_page.ico"));
    next_btn->setIconSize(_M_SIZE_);

    mainLayout->addLayout(hboxLayout_1,1);
    mainLayout->addWidget(view,8);
    mainLayout->addLayout(hboxLayout_2,1);

    //返回
    connect(back_btn,&QPushButton::clicked,this,[=](){
        emit back_btn_clicked_signal();
    });

    //鼠标右键
    menu = new QMenu(this);
    add_play_list = new QMenu("添加到播放列表");
    play_current_music = new QAction("播放当前音乐");

    menu->addMenu(add_play_list);
    menu->addAction(play_current_music);

    add_play_list_not_clear = new QAction("全部添加");
    add_play_list_clear = new QAction("全部添加&&清除原列表");
    add_play_list_selection = new QAction("添加选中到播放列表");

    add_play_list->addAction(add_play_list_not_clear);
    add_play_list->addAction(add_play_list_clear);
    add_play_list->addAction(add_play_list_selection);


}

void MusicListWidget::set_music_list_widget(const QVector<QStringList> &vec_list)
{

    head->removeRows(0,head->rowCount());

    previous_btn->disconnect();
    next_btn->disconnect();

    title_label->setText(QString("<h2>%1</h2>").arg(vec_list.at(0).at(0)));

    const QString previous = vec_list.at(0).at(1);
    const QString next = vec_list.at(0).at(2);

    QRegExp rx("/\\d*(?=.html)");
    rx.setMinimal(true);
    rx.indexIn(next);
    const QString page = rx.cap(0).mid(1);
    const int index = page.toInt() - 1;
    page_label->setText(QString("第 %1 页").arg(index));

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
        next_btn->setEnabled(true);
        if (previous.isEmpty())
        {
            previous_btn->setEnabled(false);
        } else {

            previous_btn->setEnabled(true);
            emit page_changed_signal(previous);
        }
    });

    //下一页
    connect(next_btn,&QPushButton::clicked,this,[=](){
        previous_btn->setEnabled(true);
        if (next.isEmpty())
        {
            next_btn->setEnabled(false);
        } else {
            next_btn->setEnabled(true);
            emit page_changed_signal(next);
        }
    });

    add_play_list_not_clear->disconnect();
    add_play_list_clear->disconnect();
    add_play_list_selection->disconnect();

    //全部添加
    connect(add_play_list_not_clear,&QAction::triggered,this,[=](){
        emit add_play_list_signal(false,vec_list);
    });

    //添加并删除列表
    connect(add_play_list_clear,&QAction::triggered,this,[=](){
        emit add_play_list_signal(true,vec_list);
    });

    //添加选中列表
    connect(add_play_list_selection,&QAction::triggered,this,[=](){
        qDebug() << "添加选中列表";
    });

    //播放当前音乐
    connect(play_current_music,&QAction::triggered,this,[=](){
        qDebug() << "播放当前音乐";
    });

}

void MusicListWidget::contextMenuEvent(QContextMenuEvent *event)
{
    menu->move(QCursor::pos());
    menu->exec();
    event->accept();
}

