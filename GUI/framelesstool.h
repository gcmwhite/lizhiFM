#ifndef FRAMELESSTOOL_H
#define FRAMELESSTOOL_H

#include <QObject>
#include <QWidget>
#include <QEvent>

class FrameLessTool : public QObject
{
    Q_OBJECT
public:
    explicit FrameLessTool(QObject *parent = nullptr);
    void setTitleWidget(QWidget* titleWidget);                  //设置标题栏窗口
    void setMinSize(const QSize& size);                         //设置最小大小
    void setMinSize(int w,int h);                               //设置最小大小

protected:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;

private:
    inline bool move(QEvent* event);                        //窗口移动
    inline bool resize(QEvent* event);                      //窗口缩放
    inline void cornerChanged(QMouseEvent* event);          //鼠标相对于窗口位置改变
    inline void sizeChanged(QMouseEvent* event);            //窗口大小改变

private:
    QWidget* titleWidget_{nullptr};
    QWidget* mainWidget_{nullptr};

    QPoint pressedPoint_;               //鼠标按下时相对于窗口的位置

    bool isPressedTitle_{false};        //标题栏是否按下
    bool isPressedMain_{false};         //主窗口是否按下

    static constexpr int cornerSize_{5};
    enum CORNER{                //鼠标位置
        TOP,                    //上
        BOTTOM,                 //下
        LEFT,                   //左
        RIGHT,                  //右
        LEFTTOP,                //左上
        RIGHTTOP,               //右上
        LEFTBOTTOM,             //左下
        RIGHTBOTTOM,            //右下
        CENTERAL,               //居中
    } corner_{CORNER::CENTERAL};
};

#endif // FRAMELESSTOOL_H
