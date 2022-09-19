#include "onlinesocketnode.h"

OnlineSocketNode::OnlineSocketNode()
{

}

OnlineSocketNode::OnlineSocketNode(int id, quint16 p, QTcpSocket *s)
{
    id = Uid;
    port = p;
    Socket = s;
    next = NULL;
}
