#include "radiolistwidget.h"
#include "ui_radiolistwidget.h"
#include "Common.h"

RadioListWidget::RadioListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RadioListWidget)
{
    ui->setupUi(this);
    init();
}

RadioListWidget::~RadioListWidget()
{
    delete ui;
}

//获取中央窗体
QWidget *RadioListWidget::getCentralWidget()
{
    return ui->centralWidget;
}

//设置标题
void RadioListWidget::setTitleLabel(const QString &str)
{
    ui->titleLabel->setText(str);
}

//标题隐藏
void RadioListWidget::setHeadWidgetVisible(bool visible)
{
    ui->headerWidget->setVisible(visible);
}

//设置上一页可用
void RadioListWidget::setPrevBtnEnable(bool enable)
{
    ui->preBtn->setEnabled(enable);
}

//设置下一页可用
void RadioListWidget::setNextBtnEnable(bool enable)
{
    ui->nextBtn->setEnabled(enable);
}

//设置当前页
void RadioListWidget::setCurrentPage(int page)
{
    ui->pageLabel->setText(QString("第 %1 页").arg(page));
}

//设置上一页按钮属性参数
void RadioListWidget::setPrecvBtnPropertyVar(const char *name, const QVariant &var)
{
    ui->preBtn->setProperty(name,var);
}

//设置下一页按钮属性参数
void RadioListWidget::setNextBtnPropertyVar(const char* name,const QVariant &var)
{
    ui->nextBtn->setProperty(name,var);
}

//获取下一页按钮属性参数
QVariant RadioListWidget::getNextBtnPropertyVar(const char* name) const
{
    return ui->nextBtn->property(name);
}

//获取上一页按钮属性参数
QVariant RadioListWidget::getPrecvBtnPropertyVar(const char* name) const
{
    return ui->preBtn->property(name);
}

void RadioListWidget::init()
{
    ui->headerWidget->setVisible(false);
    ui->footerWidget->layout()->setAlignment(Qt::AlignCenter);

    ui->backBtn->setFont(LiZhiFM::iconFont);
    ui->preBtn->setFont(LiZhiFM::iconFont);
    ui->nextBtn->setFont(LiZhiFM::iconFont);

    connect(ui->preBtn,&QPushButton::clicked,this,&RadioListWidget::signalPrecvBtnClicked);
    connect(ui->nextBtn,&QPushButton::clicked,this,&RadioListWidget::signalNextBtnClicked);
    connect(ui->backBtn,&QPushButton::clicked,this,&RadioListWidget::signalBackBtnClicked);
}
