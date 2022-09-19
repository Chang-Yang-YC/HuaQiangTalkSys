#include "friendpagewidget.h"
#include "ui_friendpagewidget.h"
#include "singlefrienditem.h"
#include "singlechatgroupitem.h"

#include <QPixmap>
#include <QList>
#include <QtDebug>
#include <QLineEdit>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

FriendPageWidget::FriendPageWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FriendPageWidget)
{
    ui->setupUi(this);
    tcpClient = TcpClientTool::getins();
    readyread = readyreadSignals::getins();
    setWindowTitle("华强聊天软件");
    setFixedWidth(331);
    setFixedHeight(671);
    //初始化userInfo
    userId = 0;
    userName = "";
    userStr = "";
    friendList = new FriendList(ui->listWidgetFriend);
    chatGroupList = new ChatGroupList(ui->listWidgetChatGroup);
    //显示头像
    ui->labelAvatar->setPixmap(QPixmap(":/Icon&Avatar/Avatar.jpeg"));
    ui->labelAvatar->setScaledContents(true); //自适应大小
    //创建添加好友界面
    addFriendWidget = new AddFriendWidget();
    addFriendWidget->setWindowTitle("添加好友");
    addFriendWidget->hide();
    //创建添加群聊页面
    addChatGroupWidget = new AddChatGroupWidget();
    addFriendWidget->setWindowTitle("添加群聊");
    addFriendWidget->hide();
    //连接信号与槽
    connect(addFriendWidget, &AddFriendWidget::signalAddFriend, this, &FriendPageWidget::on_signalAddFriend_received);
    connect(addChatGroupWidget, &AddChatGroupWidget::signalAddChatGroup, this, &FriendPageWidget::on_signalAddChatGroup_received);
    //输出未完成的功能
    qDebug() << "addfriendwidget.cpp: line 27";
    qDebug() << "addchatgroupwidget.cpp: line 24";
    qDebug() << "friendpagewidget.cpp: line 161";
    qDebug() << "friendpagewidget.cpp: line 48";
    qDebug() << "friendlist.cpp: line 128";
}

FriendPageWidget::~FriendPageWidget()
{
    delete ui;
}

void FriendPageWidget::setUserInfo(int id)
{
    userId = id;
    userName = QString("用户%1").arg(id);
    userStr = QString("User%1").arg(id);
    ui->labelName->setText(userName);
    //给子类widget加入user信息
    chatGroupList->setUserInfo(id);
    friendList->setUserInfo(id);
    //添加数据库
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
    //建立一个好友表和分组表、群聊表
    QSqlQuery query;
    query.exec(QString("Create Table %1FriendInfo(id int primary key, name vchar(255), avatar int, groupid int);").arg(userStr));
    query.exec(QString("Create Table %1GroupInfo(groupid int primary key, groupname vchar(255));").arg(userStr));
    query.exec(QString("Create Table %1ChatGroupInfo(id int primary key);").arg(userStr));
}


void FriendPageWidget::on_pushButtonAddGroup_clicked()
{
    //创建组
    QListWidgetItem *newHead = new QListWidgetItem(QIcon("://Icon&Avatar/ArrowRight.png"),"未命名");
    friendList->addItem(newHead);
    //将组信息加入isHideMap和groupMap
    friendList->isHideMap.insert(newHead, true);
    friendList->groupMap.insert(newHead, newHead);
    friendList->usedGroupIdNum++;
    friendList->groupIdMap.insert(newHead, friendList->usedGroupIdNum);
    //为新加入的分组重命名
    friendList->groupNameEdit->raise();
    friendList->groupNameEdit->setText(tr("未命名"));//设置默认内容
    friendList->groupNameEdit->selectAll();//设置全选
    friendList->groupNameEdit->setGeometry(friendList->visualItemRect(newHead).left()+30,friendList->visualItemRect(newHead).top(),friendList->visualItemRect(newHead).width(),friendList->visualItemRect(newHead).height()-2);//出现的位置
    friendList->groupNameEdit->show();//显示
    friendList->groupNameEdit->setFocus();//获取焦点
    friendList->currentItem = newHead;// 因为要给group命名，所以当前的currentItem设为该group
    //将group的信息写入数据库
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
    query.exec(QString("Insert Into %1GroupInfo(groupid,groupname) values(%2,'%3');").arg(userStr).arg(friendList->usedGroupIdNum).arg("未命名"));

}

void FriendPageWidget::on_pushButtonAddFriend_clicked()
{
    addFriendWidget->show();
}

void FriendPageWidget::on_signalAddFriend_received(int id, QString name)
{
    //创建好友卡
    SingleFriendItem *newFriendCard = new SingleFriendItem();
    newFriendCard->setData(QString("用户%1").arg(id), 1);
    //创建Item，设置格式
    QListWidgetItem *newItem = new QListWidgetItem();
    newItem->setSizeHint(QSize(friendList->width()-10,50));
    //将ListWidgetItem与id绑定
    friendList->friendIdMap.insert(newItem,id);
    //将好友信息加入friendList
    QList<QListWidgetItem*> tem = friendList->groupMap.keys(friendList->ungroupedUsersItem);
    friendList->insertItem(friendList->row(friendList->ungroupedUsersItem)+tem.count(), newItem);
    //将好友卡与Item绑定
    friendList->setItemWidget(newItem, newFriendCard);
    //将好友信息加入groupMap
    friendList->groupMap.insert(newItem, friendList->ungroupedUsersItem);
    //根据组信息决定好友卡是否显示
    if(true == friendList->isHideMap.value(friendList->ungroupedUsersItem))
    {
        newItem->setHidden(true);
    }
    else
    {
        newItem->setHidden(false);
    }
    //好友信息加入数据库
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
    query.exec(QString("Insert Into %1FriendInfo(id,name,avatar,groupid) values(%2,'%3',1,0);").arg(userStr).arg(id).arg(name));
    //发送给服务器


    qDebug() << "TODO:将好友信息发送给服务器";

}

void FriendPageWidget::on_signalAddChatGroup_received(int id)
{
    //新建群聊信息
    QListWidgetItem *newItem = new QListWidgetItem();
    newItem->setSizeHint(QSize(chatGroupList->width()-10,50));
    chatGroupList->addItem(newItem);
    //群聊卡片
    SingleChatGroupItem *newChatGroupCard = new SingleChatGroupItem();
    newChatGroupCard->setData(QString("群聊：%1").arg(id));
    chatGroupList->setItemWidget(newItem, newChatGroupCard);
    chatGroupList->idMap.insert(newItem, id);
    //群聊信息加入数据库
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
    query.exec(QString("Insert Into %1ChatGroupInfo(id) values(%2);").arg(userStr).arg(id));
    chatGroupList->setUserInfo(userId);
}

void FriendPageWidget::on_pushButtonAddChatGroup_clicked()
{
    addChatGroupWidget->show();
    addChatGroupWidget->userinfo(userId);
}
