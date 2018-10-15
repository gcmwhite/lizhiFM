#include "aboutwidget.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QTextBrowser>
#include <QDesktopServices>
#include <QUrl>

AboutWidget::AboutWidget(QWidget *parent) : QWidget(parent)
{
//    this->resize(390,320);
    this->setFixedSize(400,500);
    this->setWindowTitle("关于");
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
//    QLabel *label = new QLabel;
    QTextBrowser *text_browser = new QTextBrowser;
    text_browser->setStyleSheet("background:transparent;");
//    mainLayout->addWidget(label);
    mainLayout->addWidget(text_browser);
    QString str = "<html>"
            "<head/>"

            "<body>"
                "<p align=\"center\">"
                    "<span style=\" font-weight:600;\">声明</span>"
                "</p>"
                "<p>版本：v2.0-release</p>"
                "<p>更新日期：2018年10月15日</p>"
                "<p>本次更新内容：</p>"
                "<p>修复ubuntu 18.04下进度条不能动的问题</p>"
                "<p>将列表的保存方式从SQLITE变为json文件</p>"
                "<p>新增双击播放当前曲目</p>"
                "<p>新增列表的右键管理</p>"
                "<p>修复大量BUG</p>"
                "<p>已知BUG：在Ubuntu 18.04下特殊字体会导致Qt显示异常，导致界面紊乱。目前还未找到原因和合适的解决方案。</p>"
                "<p>注意：</p>"
                "<p>本软件基于Qt 5.11.2编写</p>"
                "<p>FM音频资源全部引用自"
                    "<a href=\"http://www.lizhi.fm/\">"
                        "<span style=\" text-decoration: underline; color:#0000ff;\">荔枝FM</span>"
                    "</a>网页版，本软件不具有FM音频资源版权</p>"
                "<p>目的仅为学习和交流，以及方便更多的"
                    "<a href=\"http://www.lizhi.fm/\">"
                         "<span style=\" text-decoration: underline; color:#0000ff;\">荔枝FM</span>"
                    "</a>的爱好者收听节目</p>"
                "<p>本软件请勿用于任何商业行为</p>"
                "<p>欢迎大家上Github上加个小心心，支持本软件请进行捐赠。</p>"
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
                "<p>项目主页："
                    "<a href=\"http://project.51-games.top/\">"
                        "<span style=\" text-decoration: underline; color:#0000ff;\">http://project.51-games.top/</span>"
                    "</a>"
                "</p>"
                "<p>"
                    "软件部分素材来自互联网，有任何侵权行为请发送邮件到：itoday_06@qq.com"
                "</p>"
            "</body>"

            "</html>";
//    label->setText(str);
    text_browser->setOpenLinks(false);
    text_browser->setText(str);
    connect(text_browser,&QTextBrowser::anchorClicked,this,[=](const QUrl &url){
        QDesktopServices::openUrl(QUrl(url));
    });
//    connect(label,&QLabel::linkActivated,this,[=](QString url){
//        QDesktopServices::openUrl(QUrl(url));
//    });
}
