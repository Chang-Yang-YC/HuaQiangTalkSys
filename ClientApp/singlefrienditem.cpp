#include "singlefrienditem.h"

#include <QWidget>
#include <QLabel>
#include <QPainter>
#include <QPixmap>
#include <QtDebug>

SingleFriendItem::SingleFriendItem(QWidget *parent) : QWidget(parent)
{
    initialUi();
}

void SingleFriendItem::initialUi()
{
    //初始化
    avatar=new QLabel(this);
    name=new QLabel(this);
    sign=new QLabel(this);
    //设置头像大小
    avatar->setFixedSize(40,40);
    //设置默认头像
    avatar->setPixmap(QPixmap(":/Icon&Avatar/Avatar.jpeg"));
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
    sign->setText("此用户未设置签名");
}

void SingleFriendItem::setData(QString name, int avatar)
{
    this->name->setText(name);
    this->avatar->setPixmap(QPixmap(":/Icon&Avatar/Avatar.jpeg"));
}
