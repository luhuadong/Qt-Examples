#include "mainwidget.h"
#include "ui_mainwidget.h"

#include <QTextCodec>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("天气查询"));

    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::on_pushButton_clicked()
{
    qDebug() << "clicked";

    QNetworkRequest request;
    request.setUrl(QUrl(QString("http://weather.com.cn/data/sk/%1.html").arg(ui->lineEdit->text())));

    /* Set header */
    request.setRawHeader(QByteArray("User-Agent"), QByteArray("Mozilla/5.0"));
    /* Auto redirect */
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);

    reply = manager->get(request);
    connect(reply, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    connect(reply, SIGNAL(finished()), this, SLOT(downloadFinished()));
}

void MainWidget::slotReadyRead()
{
    qDebug() << "ready";
}

void MainWidget::downloadFinished()
{
    qDebug() << "download";
    reply->deleteLater();
}

void MainWidget::replyFinished(QNetworkReply *reply)
{
    qDebug() << "reply";

    /* Get http status code */
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(statusCode.isValid()) {
        qDebug() << "status code:" << statusCode.toInt();
    }

    QVariant reason = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute);
    if(reason.isValid()) {
        qDebug() << "reason:" << reason.toString();
    }

    QNetworkReply::NetworkError err = reply->error();
    if(err != QNetworkReply::NoError) {
        qDebug() << "Failed:" << reply->errorString();
    }
    else {
        QByteArray bytes = reply->readAll();
        QString string = QString::fromUtf8(bytes);
        qDebug() << string;

        parseFromJson(string);
    }
}

void MainWidget::parseFromJson(const QString &jsonStr)
{
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(jsonStr.toUtf8(), &err);

    if(err.error != QJsonParseError::NoError) {
        qDebug() << "Parsed error";
        return;
    }
    QJsonObject obj = doc.object();
    if(obj.contains(QLatin1Literal("weatherinfo"))) {
        qDebug() << "Parsing";
        QJsonObject subObj = obj.value("weatherinfo").toObject();
        QString city = subObj["city"].toString();
        QString temp = subObj["temp"].toString();
        QString WD = subObj["WD"].toString();
        QString WS = subObj["WS"].toString();
        QString time = subObj["time"].toString();
        ui->textBrowser->setText(QString("城市：%1\n气温：%2\n风向：%3\n风速：%4\n时间：%5").arg(city).arg(temp).arg(WD).arg(WS).arg(time));
    }
}
