#include "singlechatgroupitem.h"

SingleChatGroupItem::SingleChatGroupItem(QWidget *parent) : QWidget(parent)
{
    initialUi();
}

void SingleChatGroupItem::initialUi()
{
    //初始化
    avatar=new QLabel(this);
    name=new QLabel(this);
    sign=new QLabel(this);
    //设置头像大小
    avatar->setFixedSize(40,40);
    //设置默认头像
    avatar->setPixmap(QPixmap(":/Icon&Avatar/GroupChatAvatar.webp"));
    avatar->setScaledContents(true);
    //设置个性签名字体为灰色
    QPalette color;
    color.setColor(QPalette::Text,Qt::gray);
    sign->setPalette(color);
    //布局
    avatar->move(17,7);
    name->move(64,10);
    sign->move(64,27);
    //设置默认文字
    name->setText("未命名");
    sign->setText("这是一个群聊！");
}

void SingleChatGroupItem::setData(QString name)
{
    this->name->setText(name);
}
