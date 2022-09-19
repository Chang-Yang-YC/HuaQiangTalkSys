#ifndef ONLINESOCKETLIST_H
#define ONLINESOCKETLIST_H
#include <onlinesocketnode.h>


class OnlineSocketList
{
public:
    OnlineSocketList();

    //实例化
    static OnlineSocketList *nowOnline;
    static OnlineSocketList *getins();

    //增加节点功能
    void addOnlineSocketNode(int id, quint16 p, QTcpSocket *s);
    //删除节点功能
    void deleteOnlineSocketNode(int id);
    //在链表中查询端口p对应的socket
    QTcpSocket* GetSocket(quint16 p);
    //重载GetSocket函数，在链表中查询ID对应的socket
    QTcpSocket* GetSocket(int id);
private:
    OnlineSocketNode* head = new OnlineSocketNode;

};

extern OnlineSocketList *nowOnline;

#endif // ONLINESOCKETLIST_H
