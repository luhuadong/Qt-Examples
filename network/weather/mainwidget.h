#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>


namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

private slots:
    void slotReadyRead();
    void downloadFinished();
    void replyFinished(QNetworkReply *reply);
    void parseFromJson(const QString &jsonStr);

    void on_pushButton_clicked();

private:
    Ui::MainWidget *ui;

    QNetworkAccessManager *manager;
    QNetworkReply *reply;
};

#endif // MAINWIDGET_H
