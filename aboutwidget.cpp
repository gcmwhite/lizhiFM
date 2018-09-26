#include "aboutwidget.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QDesktopServices>
#include <QUrl>

AboutWidget::AboutWidget(QWidget *parent) : QWidget(parent)
{
//    this->resize(390,320);
    this->setWindowTitle("关于");
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QLabel *label = new QLabel;
    mainLayout->addWidget(label);
    QString str = "<html>"
            "<head/>"

            "<body>"
                "<p align=\"center\">"
                    "<span style=\" font-weight:600;\">声明</span>"
                "</p>"
                "<p>本软件基于Qt 5.10编写</p>"
                "<p>FM音频资源全部引用自"
                    "<a href=\"http://www.lizhi.fm/\">"
                        "<span style=\" text-decoration: underline; color:#0000ff;\">荔枝FM</span>"
                    "</a>网页版，本软件不具有FM音频资源版权</p>"
                "<p>目的仅为学习和交流，以及方便更多的"
                    "<a href=\"http://www.lizhi.fm/\">"
                        "<span style=\" text-decoration: underline; color:#0000ff;\">荔枝FM</span>"
                    "</a>的爱好者收听节目</p>"
                "<p>本软件请勿用于任何商业行为</p>"
                "<p>版本：v1.1</p>"
                "<p>更新日期：2018年2月11日</p>"
                "<p>本次更新内容：</p>"
                "<p>优化界面风格，添加了8款皮肤背景，并且可以设置自定义背景</p>"
                "添加了皮肤、github、设置、捐赠、关于按钮，其中设置项不可用"
                "<p>保存播放列表、进度，不用每次打开都重新开始</p>"
                "<p>荔枝FM："
                    "<a href=\"http://www.lizhi.fm/\">"
                        "<span style=\" text-decoration: underline; color:#0000ff;\">http://www.lizhi.fm/</span>"
                    "</a>"
                "</p>"
                "<p>微博："
                    "<a href=\"https://weibo.com/535789608\">"
                        "<span style=\" text-decoration: underline; color:#0000ff;\">https://weibo.com/535789608</span>"
                    "</a>"
                "</p>"
                "<p>github："
                    "<a href=\"https://github.com/gcmwhite/lizhiFM\">"
                        "<span style=\" text-decoration: underline; color:#0000ff;\">https://github.com/gcmwhite/lizhiFM</span>"
                    "</a>"
                "</p>"
               " <p>百度云下载地址："
                    "<a href=\"https://pan.baidu.com/s/1nw6Vzcd\">"
                        "<span style=\" text-decoration: underline; color:#0000ff;\">https://pan.baidu.com/s/1nw6Vzcd</span>"
                    "</a> 密码: 3f4m</p>"
                "<p>个人主页："
                    "<a href=\"http://www.51-games.com/\">"
                        "<span style=\" text-decoration: underline; color:#0000ff;\">http://www.51-games.com/</span>"
                    "</a>"
                "</p>"
                "<p>"
                    "软件部分素材来自互联网，有任何侵权行为请发送邮件到：itoday_06@qq.com"
                "</p>"
            "</body>"

            "</html>";
    label->setText(str);
    connect(label,&QLabel::linkActivated,this,[=](QString url){
        QDesktopServices::openUrl(QUrl(url));
    });
}
