#include "chatgrouplist.h"

#include "singlechatgroupitem.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QMouseEvent>
#include <QtDebug>
#include <QAction>

ChatGroupList::ChatGroupList(QListWidget* parent)
    : QListWidget(parent)
{
    initialUi();
    //初始化userInfo
    userId = 0;
    userName = "";
    userStr = "";
    //设置chatGroupList大小
    resize(parent->width(),parent->height());
}

void ChatGroupList::initialUi()
{
    chatGroupMenu = new QMenu();
    QAction *actionDeleteChatGroup = new QAction("删除群聊", this);
    chatGroupMenu->addAction(actionDeleteChatGroup);
    //挂接菜单与槽函数
    connect(actionDeleteChatGroup, &QAction::triggered, this, &ChatGroupList::on_deleteChatGroupAction_triggered);
}

void ChatGroupList::mousePressEvent(QMouseEvent *event)
{
    //调用鼠标事件
    QListWidget::mousePressEvent(event);
    currentItem = this->itemAt(mapFromGlobal(QCursor::pos()));//鼠标位置的Item，不管右键左键都获取
}

void ChatGroupList::setUserInfo(int id)
{
    userId = id;
    userName = QString("用户%1").arg(id);
    userStr = QString("User%1").arg(id);
    setData();
}

void ChatGroupList::mouseDoubleClickEvent(QMouseEvent *event)
{
    //调用鼠标双击事件
    QListWidget::mouseDoubleClickEvent(event);
    if(event->button()==Qt::LeftButton && currentItem!=NULL)
    {
        //如果点击的左键并且是点击的是群聊，弹出页面
        int meID = userId;
        int chatGroupID = idMap.value(currentItem);
        QString chatGroupName = QString("群聊%1").arg(chatGroupID);
        //void start(int toid,int meid,QString name);
        gchatwidget.start(chatGroupID,meID,chatGroupName);
    }
}

void ChatGroupList::contextMenuEvent(QContextMenuEvent *event)
{
    QListWidget::contextMenuEvent(event);//调用基类事件
    if(currentItem != NULL)
    {
        chatGroupMenu->exec(QCursor::pos());
    }
}

void ChatGroupList::on_deleteChatGroupAction_triggered()
{
    //删除群聊
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    //设置数据库
    db.setDatabaseName("../friendMessage.db");
    //打开数据库
    bool isOk = db.open();
    if(false == isOk)
    {
        QMessageBox::warning(this,"数据库打开错误",db.lastError().text());
        return;
    }
    int id = idMap.value(currentItem);
    delete currentItem;
    QSqlQuery query;
    query.exec(QString("Delete From %1ChatGroupInfo Where id = %2;").arg(userStr).arg(id));
}

void ChatGroupList::setData()
{
    //从数据库中读取数据
    //添加一个数据库
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    //设置数据库
    db.setDatabaseName("../friendMessage.db");
    //打开数据库
    bool isOk = db.open();
    if(false == isOk)
    {
        QMessageBox::warning(this,"数据库打开错误",db.lastError().text());
        return;
    }
    QSqlQuery query;
    query.exec(QString("Select * From %1ChatGroupInfo;").arg(userStr));
    while(true == query.next())
    {
        int chatGroupId = query.value(0).toInt();
        QListWidgetItem *newItem = new QListWidgetItem();
        newItem->setSizeHint(QSize(width()-10,50));
        addItem(newItem);
        //群聊卡片
        SingleChatGroupItem *newChatGroupCard = new SingleChatGroupItem();
        newChatGroupCard->setData(QString("群聊：%1").arg(chatGroupId));
        setItemWidget(newItem, newChatGroupCard);
        idMap.insert(newItem,chatGroupId);
    }
}
