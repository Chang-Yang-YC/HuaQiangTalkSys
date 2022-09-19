#ifndef ONLINESOCKETNODE_H
#define ONLINESOCKETNODE_H
#include <QtGlobal>
#include <QTcpSocket>

class OnlineSocketNode
{
public:
    quint16 port;
    QTcpSocket *Socket;
    int Uid;
    OnlineSocketNode* next;

    OnlineSocketNode();
    OnlineSocketNode(int id, quint16 p, QTcpSocket *s);  //含参构造函数

};

#endif // ONLINESOCKETNODE_H
