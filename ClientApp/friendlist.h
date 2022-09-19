#ifndef FRIENDLIST_H
#define FRIENDLIST_H

#include "setgroupwidget.h"
#include <QListWidget>
#include <QMenu>
#include "chatwidget.h"

class FriendList : public QListWidget
{
    Q_OBJECT
public:
    explicit FriendList(QListWidget *parent = 0);
    void setUserInfo(int);
    //初始化菜单
    void initialUi();
    //鼠标单击事件
    void mousePressEvent(QMouseEvent *event);
    //鼠标双击事件
    void mouseDoubleClickEvent(QMouseEvent *event);
    //菜单事件
    void contextMenuEvent(QContextMenuEvent *event);
    //数据储存
    QMap<QListWidgetItem*,QListWidgetItem*> groupMap;   //储存好友控件与组控件的联系
    QMap<QListWidgetItem*,bool> isHideMap; //储存分组是否隐藏信息
    QMap<QListWidgetItem*, int> friendIdMap; //储存好友卡控件以及好友id的联系以及组控件与组id的联系
    QMap<QListWidgetItem*, int> groupIdMap; //组控件与组id的联系
    QListWidgetItem *currentItem;//当前的项
    QListWidgetItem *ungroupedUsersItem; //默认所有好友项
    QMenu *groupMenu; //组菜单
    QLineEdit *groupNameEdit;//组的名字，重命名的时候需要用到
    QMenu *friendMenu; //好友菜单
    SetGroupWidget *setGroupWidget;  //设置分组界面
    int usedGroupIdNum=0;  //已被使用的groupId数
public slots:
    void on_deleteGroupAction_triggered();
    void on_renameGroupAction_triggered();
    void on_renameGroupAction_finished();
    void on_deleteFriendAction_triggered();
    void on_removeFriendFromGroupAction_triggered();
    void on_setGroupAction_triggered();
    void on_signalSetGroup_received(int);
private:
    void initialData();
    int userId;
    QString userStr;
    QString userName;
    chatWidget chatwidget;
};

#endif // FRIENDLIST_H
