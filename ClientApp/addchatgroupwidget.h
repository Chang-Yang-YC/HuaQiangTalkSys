#ifndef ADDCHATGROUPWIDGET_H
#define ADDCHATGROUPWIDGET_H

#include <QWidget>
#include "TcpClientTool.h"
#include "readyreadsignals.h"
namespace Ui {
class AddChatGroupWidget;
}

class AddChatGroupWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AddChatGroupWidget(QWidget *parent = nullptr);
    ~AddChatGroupWidget();
    void userinfo(int meid);
signals:
    void signalAddChatGroup(int);
private slots:
    void on_pushButtonSure_clicked();
    void on_pushButtonCancel_clicked();

private:
    Ui::AddChatGroupWidget *ui;
    TcpClientTool *tcpClient;
   readyreadSignals *readyread;
   int meid;
};

#endif // ADDCHATGROUPWIDGET_H
