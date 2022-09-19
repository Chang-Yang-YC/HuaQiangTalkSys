#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include"contactwidget.h"
#include"registerwidget.h"
#include "TcpClientTool.h"
#include"readyreadsignals.h"
#include"friendpagewidget.h"
QT_BEGIN_NAMESPACE
namespace Ui { class loginWidget; }
QT_END_NAMESPACE

class loginWidget : public QWidget
{
    Q_OBJECT

public:
    loginWidget(QWidget *parent = nullptr);
    ~loginWidget();
    bool connection(const QString &dbName);
private slots:
    void on_pushButtonLog_clicked();
    void loginAfterRegister();
    void on_commandLinkButton_clicked();
signals:
    void signalToConnectionWin(QString);
private:
    Ui::loginWidget *ui;
    FriendPageWidget mainWin;
    registerWidget reg;
     TcpClientTool *tcpClient;
    readyreadSignals *readyread;
    QString Booler;

};
#endif // LOGINWIDGET_H
