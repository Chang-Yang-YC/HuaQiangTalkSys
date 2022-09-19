#include "readyreadsignal.h"

readyReadSignal::readyReadSignal()
{

    tcpClient = TcpClientTool::getins();

    connect(tcpClient,&TcpClientTool::receiveData,this,&readyReadSignal::on_received_selected);
}

void readyReadSignal::on_received_selected(QString str)
{
    QString type = str.section("##",0,0);
}
