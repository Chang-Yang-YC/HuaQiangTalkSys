#include "onlinesocketlist.h"
#include <QtDebug>

OnlineSocketList::OnlineSocketList()
{
    head->Uid = 0;
    head->port = 0;
    head->Socket =NULL;
    head->next = NULL;
}

OnlineSocketList *OnlineSocketList::nowOnline = nullptr;
OnlineSocketList *OnlineSocketList::getins()
{
    if(nowOnline == nullptr)
    {
        nowOnline = new OnlineSocketList;
    }
    return nowOnline;
}

void OnlineSocketList::addOnlineSocketNode(int id, quint16 p, QTcpSocket *s)
{
    OnlineSocketNode* temp = new OnlineSocketNode;
    temp->Uid = id;
    temp->port = p;
    temp->Socket = s;
    temp->next = NULL;

    //头插法插入链表
    temp->next = head;
    head = temp;
}

void OnlineSocketList::deleteOnlineSocketNode(int id)
{
    if(id == head->Uid)
    {
        head = head->next;
    }
    else
    {
        OnlineSocketNode* temp = head;
        while(temp->next != NULL)
        {
            if(id == temp->next->Uid)
            {
                temp->next = temp->next->next;
                qDebug() << "删除ID：" << id << "成功！";
                break;
            }
            temp = temp->next;
        }
    }

}

QTcpSocket *OnlineSocketList::GetSocket(quint16 p)
{
    OnlineSocketNode* temp = head;
    while(temp->next != NULL)
    {
        if(p == temp->port)
        {
            return temp->Socket;
        }
        temp = temp->next;
    }
    return 0;
}

QTcpSocket *OnlineSocketList::GetSocket(int id)
{
    OnlineSocketNode* temp = head;
    while(temp->next != NULL)
    {
        if(id == temp->Uid)
        {
            return temp->Socket;
        }
        temp = temp->next;
    }
    return 0;
}
