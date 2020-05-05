#ifndef RADIOLISTWIDGET_H
#define RADIOLISTWIDGET_H

#include <QWidget>
#include <QVector>

namespace Ui {
class RadioListWidget;
}

class RadioListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RadioListWidget(QWidget *parent = nullptr);
    ~RadioListWidget();
    QWidget* getCentralWidget();                //获取中央窗体
    void setTitleLabel(const QString& str);     //设置标题
    void setHeadWidgetVisible(bool);            //标题隐藏
    void setPrevBtnEnable(bool enable);            //设置上一页可用
    void setNextBtnEnable(bool enable);            //设置下一页可用
    void setCurrentPage(int page);                 //设置当前页
    void setPrecvBtnPropertyVar(const char* name,const QVariant& var);           //设置上一页按钮属性参数
    void setNextBtnPropertyVar(const char* name,const  QVariant& var);           //设置下一页按钮属性参数
    QVariant getPrecvBtnPropertyVar(const char *name) const;                    //获取上一页按钮属性参数
    QVariant getNextBtnPropertyVar(const char* name) const;                     //获取下一页按钮属性参数


signals:
    void signalPrecvBtnClicked();           //上一页按下
    void signalNextBtnClicked();            //下一页按下
    void signalBackBtnClicked();            //返回按钮按下

private:
    void init();
    inline void elideStr(QString str);      //截断字符

private:
    Ui::RadioListWidget *ui;
};

#endif // RADIOLISTWIDGET_H
