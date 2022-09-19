#ifndef CHATGROUPLIST_H
#define CHATGROUPLIST_H

#include <QListWidget>
#include <QMap>
#include <QMenu>
#include "groupchatwidget.h"
class ChatGroupList : public QListWidget
{
    Q_OBJECT
public:
    explicit ChatGroupList(QListWidget *parent);
    QMap<QListWidgetItem*, int> idMap;
    void initialUi();
    //鼠标单击事件
    void mousePressEvent(QMouseEvent *event);
    void setUserInfo(int);
    //鼠标双击事件
    void mouseDoubleClickEvent(QMouseEvent *event);
    //菜单事件
    void contextMenuEvent(QContextMenuEvent *event);
    //菜单
    QMenu *chatGroupMenu;
    QListWidgetItem* currentItem; //当前选项卡
public slots:
    void on_deleteChatGroupAction_triggered();
private:
    void setData();
    int userId;
    QString userStr;
    QString userName;
    groupchatWidget gchatwidget;
};

#endif // CHATGROUPLIST_H
