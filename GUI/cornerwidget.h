#ifndef CORNERWIDGET_H
#define CORNERWIDGET_H

#include <QWidget>

namespace Ui {
class CornerWidget;
}

class CornerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CornerWidget(QWidget *parent = nullptr);
    ~CornerWidget();

private:
    void init();

private:
    Ui::CornerWidget *ui;
};

#endif // CORNERWIDGET_H
