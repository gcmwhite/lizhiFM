#include "cornerwidget.h"
#include "ui_cornerwidget.h"
#include "Common.h"

CornerWidget::CornerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CornerWidget)
{
    ui->setupUi(this);
    init();
}

CornerWidget::~CornerWidget()
{
    delete ui;
}

void CornerWidget::init()
{
    ui->aboutBtn->setFont(LiZhiFM::iconFont);
    ui->themeBtn->setFont(LiZhiFM::iconFont);
    ui->githubBtn->setFont(LiZhiFM::iconFont);
    ui->settingBtn->setFont(LiZhiFM::iconFont);
    ui->donationBtn->setFont(LiZhiFM::iconFont);
}
