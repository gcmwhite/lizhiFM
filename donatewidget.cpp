#include "donatewidget.h"
#include <QVBoxLayout>
#include <QLabel>

DonateWidget::DonateWidget(QWidget *parent) : QWidget(parent)
{
    this->setWindowTitle("捐赠");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QLabel *label = new QLabel;
    mainLayout->addWidget(label);
    QString str = "<html>"
            "<head/>"

            "<body>"
                "<p>"
                    "<img src=\":/imgs/donate_alipay.jpg\" width=\"300\" />&nbsp;&nbsp;&nbsp;"
                    "<img src=\":/imgs/donate_wechat.jpg\" width=\"300\" />&nbsp;&nbsp;&nbsp;"
//                    "<img src=\":/imgs/alipay.jpg\" width=\"300\" />"
                "</p>"
                "<p>支持本软件的小伙伴们请进行￥10的小额捐赠（一顿饭钱(*^__^*)）</p>"
//                "<p>囊肿羞涩的朋友可以点击领取支付宝赏金，赏金可以在支付宝现金支付，转账等场合使用，使用赏金后我也将获得一部分赏金。</p>"
//                "<p>赏金每天可领一次，未使用赏金将不能领取第二天的赏金。第六期截止时间是2018年2月31日。</p>"
                "<p>二维码顺序为：支付宝、微信赞赏</p>"
            "</body>"

            "</html>";
    label->setText(str);
}
