#ifndef CONNECTCLIENT_H
#define CONNECTCLIENT_H

#include <QDebug>
#include <QThread>
#include <QTcpSocket>
#include <QHostAddress>
#include <QStringList>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include "tcpservermainwindow.h"
#include <QFile>
#include <QTimer>
#include "undodbdialog.h"
#include "onlinesocketlist.h"


class ConnectClient : public QThread
{
    Q_OBJECT
public:
    explicit ConnectClient(QTcpSocket *socket, QObject *parent = nullptr);
    void sendFile();
protected:
    void run() override;
    void on_tcpSocket_readyRead();

    QStringList ServList;
    QTcpSocket *thisSocket;
    QSqlDatabase Serverdb;
private:
    UndodbDialog undodb;//直接对数据库开一个连接

    QFile file;  //文件对象
    QString fileName; //文件名字
    qint64 fileSize; //文件大小
    qint64 sendSize; //已经发送文件的大小

    OnlineSocketList *nowOnline;  //在线用户链表
    QTimer timer; //定时器
    int receiverPort;
    qint64 recvSize; //已经接收文件的大小

    bool isStart;   //标志位，是否为头部信息
    int ToId;
signals:

};

#endif // CONNECTCLIENT_H
