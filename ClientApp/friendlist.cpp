#include "friendlist.h"
#include "singlefrienditem.h"

#include <QSize>
#include <QIcon>
#include <QMouseEvent>
#include <QtDebug>
#include <QAction>
#include <QLineEdit>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QMap>

FriendList::FriendList(QListWidget *parent):
    QListWidget(parent)
{
    //初始化Ui
    initialUi();
    //初始化userInfo
    userId = 0;
    userName = "";
    userStr = "";
    //设置FriendList大小
    resize(parent->width(),parent->height());
}

void FriendList::setUserInfo(int id)
{
    userId = id;
    userName = QString("用户%1").arg(id);
    userStr = QString("User%1").arg(id);
    setGroupWidget->setUserInfo(id);
    initialData();
}

void FriendList::initialUi()
{
    //生成组菜单
    groupMenu = new QMenu();
    QAction *deleteGroup = new QAction("删除分组", this);
    QAction *renameGroup = new QAction("重命名", this);
    groupMenu->addAction(deleteGroup);
    groupMenu->addAction(renameGroup);
    //生成组命名控件
    groupNameEdit = new QLineEdit(this);
    groupNameEdit->hide(); //设置初始时隐藏
    groupNameEdit->setPlaceholderText("未命名");//设置初始时的内容
    //生成好友菜单
    friendMenu = new QMenu;
    QAction *deleteFriend = new QAction("删除好友", this);
    QAction *setGroup = new QAction("设置分组", this);
    QAction *removeFriendFromGroup = new QAction("从分组中移除", this); 
    friendMenu->addAction(deleteFriend);
    friendMenu->addAction(removeFriendFromGroup);
    friendMenu->addAction(setGroup);
    //生成设置分组页面
    setGroupWidget = new SetGroupWidget();
    setGroupWidget->setFixedWidth(330);
    setGroupWidget->setFixedHeight(100);
    setGroupWidget->setWindowTitle("设置分组");
    //连接信号与槽
    connect(deleteGroup, &QAction::triggered, this, &FriendList::on_deleteGroupAction_triggered);
    connect(renameGroup, &QAction::triggered, this, &FriendList::on_renameGroupAction_triggered);
    connect(groupNameEdit, &QLineEdit::editingFinished, this, &FriendList::on_renameGroupAction_finished);
    connect(deleteFriend, &QAction::triggered, this, &FriendList::on_deleteFriendAction_triggered);
    connect(removeFriendFromGroup, &QAction::triggered, this, &FriendList::on_removeFriendFromGroupAction_triggered);
    connect(setGroup, &QAction::triggered, this, &FriendList::on_setGroupAction_triggered);
    connect(setGroupWidget, &SetGroupWidget::signalSetGroup, this, &FriendList::on_signalSetGroup_received);
}

void FriendList::mousePressEvent(QMouseEvent *event)
{
    //调用鼠标事件
    QListWidget::mousePressEvent(event);
    //防止一种特殊情况：给新item命名、点击其他item或空白处时，指向新item的currentItem被赋予其他item
    if(groupNameEdit->isVisible() && !(groupNameEdit->rect().contains(event->pos())))
    {
        if(groupNameEdit->text()!=NULL)
            currentItem->setText(groupNameEdit->text());
        groupNameEdit->setText("");
        groupNameEdit->hide();
    }
    currentItem = this->itemAt(mapFromGlobal(QCursor::pos()));//鼠标位置的Item，不管右键左键都获取
    if(event->button()==Qt::LeftButton && currentItem!=NULL && currentItem==groupMap.value(currentItem))//如果点击的左键并且是点击的是组
    {
        if(true == isHideMap.value(currentItem)) //目前为隐藏状态
        {
            foreach(QListWidgetItem* subItem, groupMap.keys(currentItem))
            {   //遍历组的对应的项（包括自身和好友）
                if(subItem!=currentItem)  //如果是组的话不进行处理
                {
                    //好友全部显示
                    subItem->setHidden(false);
                }
            }
            //更新该组为显示状态
            isHideMap.insert(currentItem,false);
            //设置图标
            currentItem->setIcon(QIcon(":/Icon&Avatar/ArrowDown.png"));
         }
         else  //目前为显示状态
         {
            foreach(QListWidgetItem* subItem, groupMap.keys(currentItem))
            {
                //遍历组的对应的项（包括自身和好友）
                if(subItem!=currentItem)  //如果是组的话不进行处理
                {
                     subItem->setHidden(true); //好友全部隐藏
                }
            }
            //更新该组为隐藏状态
            isHideMap.insert(currentItem,true);
            //设置图标
            currentItem->setIcon(QIcon(":/Icon&Avatar/ArrowRight.png"));
         }
    }
}

void FriendList::mouseDoubleClickEvent(QMouseEvent *event)
{
    //调用鼠标双击事件
    QListWidget::mouseDoubleClickEvent(event);
    if(event->button()==Qt::LeftButton && currentItem!=NULL && currentItem != groupMap.value(currentItem))
    {
        //如果点击的左键并且是点击的是好友，弹出页面
        int meID = userId;
        int friendID = friendIdMap.value(currentItem);
        QString friendName = QString("用户%1").arg(friendID);
        //void start(int toid,int meid,QString name);
        chatwidget.start(friendID,meID,friendName);
        qDebug() << "目标用户id:" << friendID <<"对话页面弹出";
    }
}

void FriendList::contextMenuEvent(QContextMenuEvent *event)
{
    QListWidget::contextMenuEvent(event);//调用基类事件
    if(currentItem==NULL)//如果点击到的是空白处
    {
        return;
    }
    if(currentItem==groupMap.value(currentItem))
    {
        // 如果点击到的是组
        groupMenu->exec(QCursor::pos());
    }
    else //否则点击到的是好友
    {
        friendMenu->exec(QCursor::pos());
    }
}

void FriendList::on_deleteGroupAction_triggered()
{
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
    //如果是ungroupedUsers，则无法删除
    if(currentItem != ungroupedUsersItem)
    {
        //遍历该组的所有好友和自身的组
        foreach(QListWidgetItem* item, groupMap.keys(currentItem))
        {
            if(item != currentItem) //不为组本身
            {
                int id = friendIdMap.value(item);
                groupMap.remove(item);
                groupIdMap.remove(item);
                delete item;
                //将新item加入未分组用户组中
                //创建好友卡
                SingleFriendItem *newFriendCard = new SingleFriendItem();
                query.exec(QString("Select * From %1FriendInfo Where id = %2").arg(userStr).arg(id));
                QString name = QString("");
                while(true == query.next())
                {
                    name = query.value(1).toString();
                }
                newFriendCard->setData(name, 1);
                //创建Item，设置格式
                QListWidgetItem *newItem = new QListWidgetItem();
                newItem->setSizeHint(QSize(width()-10,50));
                //将ListWidgetItem与id绑定
                friendIdMap.insert(newItem,id);
                //将好友信息加入friendList
                QList<QListWidgetItem*> tem = groupMap.keys(ungroupedUsersItem);
                insertItem(row(ungroupedUsersItem)+tem.count(), newItem);
                //将好友卡与Item绑定
                setItemWidget(newItem, newFriendCard);
                //将好友信息加入groupMap
                groupMap.insert(newItem, ungroupedUsersItem);
                //根据组信息决定好友卡是否显示
                if(true == isHideMap.value(ungroupedUsersItem))
                {
                    newItem->setHidden(true);
                }
                else
                {
                    newItem->setHidden(false);
                }
                //修改FriendInfo分组信息
                query.exec(QString("Update %1FriendInfo Set groupid = 0 Where id = %2;").arg(userStr).arg(id));
            }
        }
        isHideMap.remove(currentItem);
        int groupid = groupIdMap.value(currentItem);
        groupIdMap.remove(currentItem);
        groupMap.remove(currentItem);
        delete currentItem;
        //将组从数据库中删除
        query.exec(QString("Delete From %1GroupInfo Where groupid = %2 ;").arg(userStr).arg(groupid));
        return;
    }
    //弹出对话警告框
    qDebug() << "默认分组无法删除！";
}

void FriendList::on_renameGroupAction_triggered()
{
    //如果是All_Users，则无法修改
    if(currentItem != ungroupedUsersItem)
    {
        groupNameEdit->raise();
        groupNameEdit->setGeometry(this->visualItemRect(currentItem).left()+30,this->visualItemRect(currentItem).top(),this->visualItemRect(currentItem).width(),this->visualItemRect(currentItem).height()-2);//出现的位置
        groupNameEdit->setText(currentItem->text());//获取该组名内容
        groupNameEdit->show();  //显示
        groupNameEdit->selectAll();//全选
        groupNameEdit->setFocus();//获取焦点
        return;
    }
    //弹出对话警告框
    qDebug() << "默认分组无法重命名！";
}

void FriendList::on_renameGroupAction_finished()
{
    QString name = groupNameEdit->text();
    if(groupNameEdit->text()!=NULL) //如果重命名编辑框不为空
    {
        //更新组名
        currentItem->setText(name);
    }
    //重置文本框
    groupNameEdit->setText("");
    //隐藏重命名编辑框
    groupNameEdit->hide();
    //在数据库中更新group信息
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
    query.exec(QString("Update %1GroupInfo Set groupname = '%2' Where groupid = %3;").arg(userStr).arg(name).arg(groupIdMap.value(currentItem)));
}

void FriendList::on_deleteFriendAction_triggered()
{
    //获得待删除好友信息
    int id = friendIdMap.value(currentItem);
    groupMap.remove(currentItem);
    friendIdMap.remove(currentItem);
    delete currentItem;
    //在数据库中更新Friend信息
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
    query.exec(QString("Delete From %1FriendInfo Where id = %2;").arg(userStr).arg(id));
}

void FriendList::on_removeFriendFromGroupAction_triggered()
{
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
    //获得id
    int id = friendIdMap.value(currentItem);
    groupMap.remove(currentItem);
    friendIdMap.remove(currentItem);
    delete currentItem;
    //将新item加入未分组用户组中
    //创建好友卡
    SingleFriendItem *newFriendCard = new SingleFriendItem();
    newFriendCard->setData(QString("用户%1").arg(id), 1);
    //创建Item，设置格式
    QListWidgetItem *newItem = new QListWidgetItem();
    newItem->setSizeHint(QSize(width()-10,50));
    //将ListWidgetItem与id绑定
    friendIdMap.insert(newItem,id);
    //将好友信息加入friendList
    QList<QListWidgetItem*> tem = groupMap.keys(ungroupedUsersItem);
    insertItem(row(ungroupedUsersItem)+tem.count(), newItem);
    //将好友卡与Item绑定
    setItemWidget(newItem, newFriendCard);
    //将好友信息加入groupMap
    groupMap.insert(newItem, ungroupedUsersItem);
    //根据组信息决定好友卡是否显示
    if(true == isHideMap.value(ungroupedUsersItem))
    {
        newItem->setHidden(true);
    }
    else
    {
        newItem->setHidden(false);
    }
    //修改FriendInfo分组信息
    query.exec(QString("Update %1FriendInfo Set groupid = 0 Where id = %2;").arg(userStr).arg(id));
}

void FriendList::on_setGroupAction_triggered()
{
    //更新数据
    setGroupWidget->setData();
    //显示界面
    setGroupWidget->show();
}

void FriendList::on_signalSetGroup_received(int groupid)
{
    qDebug() << groupid;
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
    //获得id
    int id = friendIdMap.value(currentItem);
    groupMap.remove(currentItem);
    friendIdMap.remove(currentItem);
    delete currentItem;
    //将新item加入指定的组中
    QListWidgetItem *targetGroup = groupIdMap.key(groupid);
    //创建好友卡
    SingleFriendItem *newFriendCard = new SingleFriendItem();
    newFriendCard->setData(QString("用户%1").arg(id), 1);
    //创建Item，设置格式
    QListWidgetItem *newItem = new QListWidgetItem();
    newItem->setSizeHint(QSize(width()-10,50));
    //将ListWidgetItem与id绑定
    friendIdMap.insert(newItem,id);
    //将好友信息加入friendList
    QList<QListWidgetItem*> tem = groupMap.keys(targetGroup);
    insertItem(row(targetGroup)+tem.count(), newItem);
    //将好友卡与Item绑定
    setItemWidget(newItem, newFriendCard);
    //将好友信息加入groupMap
    groupMap.insert(newItem, targetGroup);
    //根据组信息决定好友卡是否显示
    if(true == isHideMap.value(targetGroup))
    {
        newItem->setHidden(true);
    }
    else
    {
        newItem->setHidden(false);
    }
    //修改FriendInfo分组信息
    query.exec(QString("Update %1FriendInfo Set groupid = %2 Where id = %3;").arg(userStr).arg(groupid).arg(id));
}

void FriendList::initialData()
{
    //读取数据
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
    //先读取未分组的好友
    ungroupedUsersItem = new QListWidgetItem(QIcon("://Icon&Avatar/ArrowRight.png"),"未分组好友");
    this->addItem(ungroupedUsersItem);
    //将组控件隐藏信息加入isHeadMap
    isHideMap.insert(ungroupedUsersItem, true);
    //将组控件自我绑定加入groupMap
    groupMap.insert(ungroupedUsersItem, ungroupedUsersItem);
    //将组控件与id绑定
    groupIdMap.insert(ungroupedUsersItem, 0);
    query.exec(QString("Select * From %1FriendInfo Where groupid = 0;").arg(userStr));
    while(true == query.next())
    {
        //创建好友卡
        SingleFriendItem *newFriendCard = new SingleFriendItem();
        newFriendCard->setData(QString("用户%1").arg(query.value(0).toInt()), query.value(2).toInt());
        //创建Item，设置格式/
        QListWidgetItem *newItem = new QListWidgetItem();
        newItem->setSizeHint(QSize(width()-10,50));
        //将ListWidgetItem与id绑定
        friendIdMap.insert(newItem,query.value(0).toInt());
        //将好友信息加入friendList
        QList<QListWidgetItem*> tem = groupMap.keys(ungroupedUsersItem);
        insertItem(row(ungroupedUsersItem)+tem.count(), newItem);
        //将好友卡与Item绑定
        setItemWidget(newItem, newFriendCard);
        //将好友信息加入groupMap
        groupMap.insert(newItem, ungroupedUsersItem);
        //根据组信息决定好友卡是否显示
        if(true == isHideMap.value(ungroupedUsersItem))
        {
            newItem->setHidden(true);
        }
        else
        {
            newItem->setHidden(false);
        }
    }
    //再添加其他分组
    query.exec(QString("Select * From %1GroupInfo;").arg(userStr));
    while(true == query.next())
    {
        int groupId =query.value(0).toInt();
        QString groupName = query.value(1).toString();
        QListWidgetItem *newHead = new QListWidgetItem(QIcon("://Icon&Avatar/ArrowRight.png"),groupName);
        this->addItem(newHead);
        //将组控件隐藏信息加入isHeadMap
        isHideMap.insert(newHead, true);
        //将组控件自我绑定加入groupMap
        groupMap.insert(newHead, newHead);
        //将组控件与id绑定
        groupIdMap.insert(newHead, groupId);
        usedGroupIdNum++;
    }
    //再为其他分组加入好友
    query.exec(QString("Select * From %1FriendInfo;").arg(userStr));
    while(true == query.next())
    {
        //当不是未分组用户时，开始生成好友卡并分组
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        int avatar = query.value(2).toInt();
        int groupId = query.value(3).toInt();
        if( groupId == 0) continue;
        //根据groupId获取组控件
        QListWidgetItem *target = groupIdMap.key(groupId);
        //创建好友卡
        SingleFriendItem *newFriendCard = new SingleFriendItem();
        newFriendCard->setData(QString("用户%1").arg(id), avatar);
        //创建Item，设置格式
        QListWidgetItem *newItem = new QListWidgetItem();
        newItem->setSizeHint(QSize(width()-10,50));
        //将ListWidgetItem与id绑定
        friendIdMap.insert(newItem,id);
        //将好友信息加入friendList
        QList<QListWidgetItem*> tem = groupMap.keys(target);
        insertItem(row(target)+tem.count(), newItem);
        //将好友卡与Item绑定
        setItemWidget(newItem, newFriendCard);
        //将好友信息加入groupMap
        groupMap.insert(newItem, target);
        //根据组信息决定好友卡是否显示
        if(true == isHideMap.value(target))
        {
            newItem->setHidden(true);
        }
        else
        {
            newItem->setHidden(false);
        }
    }
}
