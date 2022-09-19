#ifndef FRIENDPAGEWIDGET_H
#define FRIENDPAGEWIDGET_H

#include "friendlist.h"
#include "addfriendwidget.h"
#include "chatgrouplist.h"
#include "addchatgroupwidget.h"

#include <QWidget>
#include "TcpClientTool.h"
#include "readyreadsignals.h"
QT_BEGIN_NAMESPACE
namespace Ui { class FriendPageWidget; }
QT_END_NAMESPACE

class FriendPageWidget : public QWidget
{
    Q_OBJECT

public:
    FriendPageWidget(QWidget *parent = nullptr);
    ~FriendPageWidget();
    void setUserInfo(int);
private slots:
    void on_pushButtonAddGroup_clicked();
    void on_pushButtonAddFriend_clicked();
    void on_signalAddFriend_received(int, QString);
    void on_signalAddChatGroup_received(int);
    void on_pushButtonAddChatGroup_clicked();
private:
    Ui::FriendPageWidget *ui;
    FriendList *friendList;
    ChatGroupList *chatGroupList;
    AddFriendWidget *addFriendWidget;
    AddChatGroupWidget *addChatGroupWidget;
    int userId;
    QString userStr;
    QString userName;
    TcpClientTool *tcpClient;
    readyreadSignals *readyread;
};
#endif // FRIENDPAGEWIDGET_H
