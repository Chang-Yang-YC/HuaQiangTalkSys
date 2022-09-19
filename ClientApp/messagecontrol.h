#ifndef MESSAGECONTROL_H
#define MESSAGECONTROL_H

//客户端发给服务器 通过一个常量来标识删除消息还是添加消息
//0 代表添加
//1 代表删除

#include <QWidget>
#include<QTime>
#include "TcpClientTool.h"
#include"readyreadsignals.h"

QT_BEGIN_NAMESPACE
namespace Ui { class messageControl; }
QT_END_NAMESPACE

class message{
public:
    QString user1;
    QString user2;
    QString text;
    QTime time;
    QDate date;
    QString type;
    int judge;//判断是否删除
    message(){};
    message(QString u1,QString u2, QString word, QTime t,QDate d, int j, QString t1)
        :user1(u1),user2(u2),text(word),time(t),judge(j),date(d),type(t1){};
};

class messageControl : public QWidget
{
    Q_OBJECT

public:
    messageControl(QWidget *parent = nullptr);
    ~messageControl();

private:
    Ui::messageControl *ui;
    QTime time1;
    QDate date1;
    message msg1;
    TcpClientTool *tcpClient;
   readyreadSignals *readyread;
    //message msg1;
    //接口
    //将一条message初始化；然后进行数据的插入
    //Str = STEXT##发送者ID##对方ID##DATE##Time##message
    //Str = GPTEXT##senderID##群ID##Time##Date##message
    //返回格式：
    //          上面的两个格式,记着根据STEXT还是GTEXT写type

    //接口
};
#endif // MESSAGECONTROL_H
