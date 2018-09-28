#include "update.h"

#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>

Update::Update(const QString &old_version,QObject *parent)
    : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
    const QString url = "http://project.51-games.top/update_project/lizhi_fm_update.json";
    request.setUrl(QUrl(url));

    request.setHeader(QNetworkRequest::ContentTypeHeader,"text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    request.setHeader(QNetworkRequest::UserAgentHeader,"Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:62.0) Gecko/20100101 Firefox/62.0");

    QEventLoop loop;
    QNetworkReply *reply = manager->get(request);
    connect(manager,&QNetworkAccessManager::finished,&loop,&QEventLoop::quit);
    loop.exec();

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject object = document.object();

    const QString name = object["name"].toString();
    const QString version = object["version"].toString();
    const QString details = object["details"].toString();

    if (old_version != version)
    {
        QDialog *dialog = new QDialog;
        dialog->setWindowTitle(name);
        dialog->resize(300,300);
        QVBoxLayout *layout = new QVBoxLayout(dialog);

        QLabel *label_now = new QLabel;
        QLabel *label_new = new QLabel;
        QLabel *label_detail = new QLabel;

        layout->setMargin(0);
        layout->addWidget(label_now);
        layout->addWidget(label_new);
        layout->addWidget(label_detail);

        label_now->setText(QString("当前版本：%1").arg(old_version));
        label_new->setText(QString("最新版本：%1").arg(version));
        label_detail->setText(QString("更新详情：%1").arg(details));

        dialog->exec();
    }

}
