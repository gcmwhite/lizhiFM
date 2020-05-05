#include "framelesstool.h"
#include <QMouseEvent>
#include <QRect>

FrameLessTool::FrameLessTool(QObject *parent) : QObject(parent)
{
    if (nullptr != parent) {
        mainWidget_ = static_cast<QWidget *>(parent);
        mainWidget_->setWindowFlag(Qt::FramelessWindowHint);
        mainWidget_->installEventFilter(this);
        mainWidget_->setMouseTracking(true);
        mainWidget_->setMinimumSize(100,100);
    }
}

//设置标题栏窗口
void FrameLessTool::setTitleWidget(QWidget *titleWidget)
{
    if (nullptr != titleWidget) {
        titleWidget_ = titleWidget;
        titleWidget_->installEventFilter(this);
    }
}

//设置最小大小
void FrameLessTool::setMinSize(const QSize &size)
{
    mainWidget_->setMinimumSize(size);
}

//设置最小大小
void FrameLessTool::setMinSize(int w, int h)
{
    mainWidget_->setMinimumWidth(w);
    mainWidget_->setMinimumHeight(h);
}

bool FrameLessTool::eventFilter(QObject *watched, QEvent *event)
{
    if (titleWidget_ == watched) {             //窗口移动
        return move(event);
    }else if (mainWidget_ == watched) {        //窗口缩放
        return resize(event);
    }
    return QObject::eventFilter(watched,event);
}

//窗口移动
bool FrameLessTool::move(QEvent *event)
{
    static QMouseEvent* e{nullptr};
    if (QEvent::MouseButtonPress == event->type()) {
        e = static_cast<QMouseEvent *>(event);
        if (Qt::MouseButton::LeftButton == e->button()) {
            isPressedTitle_ = isPressedMain_ = true;
            pressedPoint_ = e->pos();
            return true;
        }
    }else if (QEvent::MouseMove == event->type()) {
        e = static_cast<QMouseEvent *>(event);
        if (isPressedTitle_ && CORNER::CENTERAL == corner_) {
            mainWidget_->move(e->globalPos() - pressedPoint_);
        }
    }else if (QEvent::MouseButtonRelease == event->type()) {
        e = static_cast<QMouseEvent *>(event);
        if (Qt::MouseButton::LeftButton == e->button()) {
            isPressedTitle_ = isPressedMain_ = false;
            return true;
        }
    }
    return false;
}

//窗口缩放
bool FrameLessTool::resize(QEvent *event)
{
    static QMouseEvent* e{nullptr};
    if (QEvent::MouseButtonPress == event->type()) {
        e = static_cast<QMouseEvent *>(event);
        if (Qt::MouseButton::LeftButton == e->button()) {
            isPressedMain_ = true;
        }

    }else if (QEvent::MouseMove == event->type()) {

        e = static_cast<QMouseEvent *>(event);
        cornerChanged(e);
        if (isPressedMain_) {
            sizeChanged(e);
        }
    }else if (QEvent::MouseButtonRelease == event->type()) {
        e = static_cast<QMouseEvent *>(event);
        if (Qt::MouseButton::LeftButton == e->button()) {
            isPressedMain_ = false;
        }
    }
    return false;
}

//鼠标相对于窗口位置改变
void FrameLessTool::cornerChanged(QMouseEvent *event)
{
    static QPoint pos;
    static int width,height;
    pos = event->pos();
    width = mainWidget_->width();
    height = mainWidget_->height();

    static auto func = [this](CORNER corner,Qt::CursorShape cursor){
        if (!isPressedMain_) {
            corner_ = corner;
            mainWidget_->setCursor(cursor);
        }
    };
    if (pos.x() > width - cornerSize_) {
        if (pos.y() < cornerSize_) {
            func(CORNER::RIGHTTOP,Qt::SizeBDiagCursor);             //右上
        }else if (pos.y() > height - cornerSize_) {
            func(CORNER::RIGHTBOTTOM,Qt::SizeFDiagCursor);          //右下
        }else {
            func(CORNER::RIGHT,Qt::SizeHorCursor);                  //右
        }
    }else if (pos.x() < cornerSize_) {
        if (pos.y() < cornerSize_) {
            func(CORNER::LEFTTOP,Qt::SizeFDiagCursor);             //左上
        }else if (pos.y() > height - cornerSize_) {
            func(CORNER::LEFTBOTTOM,Qt::SizeBDiagCursor);          //左下
        }else{
            func(CORNER::LEFT,Qt::SizeHorCursor);                  //左
        }
    }else {
        if (pos.y() < cornerSize_) {
            func(CORNER::TOP,Qt::SizeVerCursor);                   //上
        }else if (pos.y() > height - cornerSize_) {
            func(CORNER::BOTTOM,Qt::SizeVerCursor);                //下
        }else {
            func(CORNER::CENTERAL,Qt::ArrowCursor);                //居中
        }
    }
}

//窗口大小改变
void FrameLessTool::sizeChanged(QMouseEvent *event)
{
    static QPoint globalPos;
    static QRect rect;
    static QSize size;
    size = mainWidget_->minimumSize();

    globalPos = event->globalPos();
    rect = mainWidget_->geometry();

    switch (corner_) {
    case CORNER::LEFT: {                    //左
        if (rect.right() - globalPos.x() >= size.width()) {
            rect.setLeft(globalPos.x());
        }
        break ;
    }
    case CORNER::RIGHT: {                   //右
        rect.setRight(globalPos.x());
        break ;
    }
    case CORNER::TOP: {                     //上
        if (rect.bottom() - globalPos.y() >= size.height()) {
            rect.setTop(globalPos.y());
        }
        break ;
    }
    case CORNER::BOTTOM: {                 //下
        rect.setBottom(globalPos.y());
        break ;
    }
    case CORNER::LEFTTOP: {                //左上
        if (rect.right() - globalPos.x() >= size.width()) {
            rect.setLeft(globalPos.x());
        }
        if (rect.bottom() - globalPos.y() >= size.height()) {
            rect.setTop(globalPos.y());
        }
        break ;
    }
    case CORNER::LEFTBOTTOM: {            //左下
        if (rect.right() - globalPos.x() >= size.width()) {
            rect.setLeft(globalPos.x());
        }
        rect.setBottom(globalPos.y());
        break ;
    }
    case CORNER::RIGHTTOP: {              //右上
        if (rect.bottom() - globalPos.y() >= size.height()) {
            rect.setTop(globalPos.y());
        }
        rect.setRight(globalPos.x());
        break ;
    }
    case CORNER::RIGHTBOTTOM: {          //右下
        rect.setBottomRight(globalPos);
        break ;
    }
    case CORNER::CENTERAL: break ;       //居中
    }

    mainWidget_->setGeometry(rect);
}
