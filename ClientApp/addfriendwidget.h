#ifndef ADDFRIENDWIDGET_H
#define ADDFRIENDWIDGET_H

#include <QWidget>
#include "TcpClientTool.h"
#include "readyreadsignals.h"
namespace Ui {
class AddFriendWidget;
}

class AddFriendWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AddFriendWidget(QWidget *parent = nullptr);
    ~AddFriendWidget();
signals:
    void signalAddFriend(int , QString);
private slots:
    void on_pushButtonSearch_clicked();
    void on_pushButtonCancel_clicked();
    void on_pushButtonAdd_clicked();

private:
    Ui::AddFriendWidget *ui;
    TcpClientTool *tcpClient;
   readyreadSignals *readyread;
};

#endif // ADDFRIENDWIDGET_H
