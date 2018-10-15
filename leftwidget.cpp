#include "leftwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QAbstractItemView>

LeftWidget::LeftWidget(QWidget *parent) : QWidget(parent)
{
    this->setFixedWidth(230);

    control_widget_ = new QWidget;
    previous_btn_ = new QPushButton;
    play_btn_ = new QPushButton;
    next_btn_ = new QPushButton;
    list_wigdet_ = new QListWidget;

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *controlLayout = new QHBoxLayout(control_widget_);

    mainLayout->setMargin(0);
//    mainLayout->setSpacing(0);

    mainLayout->addWidget(control_widget_);
    mainLayout->addWidget(list_wigdet_);

    controlLayout->setMargin(0);
    controlLayout->setSpacing(0);

    control_widget_->setObjectName("controlWidget");
    controlLayout->addWidget(previous_btn_);
    controlLayout->addWidget(play_btn_);
    controlLayout->addWidget(next_btn_);

    const QSize ICON_SIZE(30,30);
    const QSize BTN_SIZE(46,46);

    previous_btn_->setFlat(true);
    previous_btn_->setIcon(QIcon(":/imgs/pre.ico"));
    previous_btn_->setIconSize(ICON_SIZE);
    previous_btn_->setFixedSize(BTN_SIZE);

    play_btn_->setFlat(true);
    play_btn_->setIcon(QIcon(":/imgs/play.ico"));
    play_btn_->setIconSize(ICON_SIZE);
    play_btn_->setFixedSize(BTN_SIZE);

    next_btn_->setFlat(true);
    next_btn_->setIcon(QIcon(":/imgs/next.ico"));
    next_btn_->setIconSize(ICON_SIZE);
    next_btn_->setFixedSize(BTN_SIZE);

    list_wigdet_->addItem("当前列表为空！");
    list_wigdet_->setSelectionMode(QAbstractItemView::ExtendedSelection);           //多行选中

    menu_ = new QMenu(list_wigdet_);
    position_ = new QAction("定位");
    remove_ = new QAction("移除选中曲目");
    clear_ = new QAction("清空列表");

    menu_->addAction(position_);
    menu_->addAction(remove_);
    menu_->addAction(clear_);

}

void LeftWidget::contextMenuEvent(QContextMenuEvent *event)
{
    menu_->move(QCursor::pos());
    menu_->exec();
    event->accept();
}
