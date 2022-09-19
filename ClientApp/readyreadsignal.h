#ifndef READYREADSIGNAL_H
#define READYREADSIGNAL_H
#include "TcpClientTool.h"

class readyReadSignal
{
public:
    readyReadSignal();
public slots:
    void on_received_selected(QString str);
signals:

private:
    TcpClientTool *tcpClient;

};

#endif // READYREADSIGNAL_H
