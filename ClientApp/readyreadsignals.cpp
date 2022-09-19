#include "readyreadsignals.h"

readyreadSignals::readyreadSignals(QWidget *parent) : QWidget(parent)
{
    tcpClient = TcpClientTool::getins();
    //接受的数据类型分类
    ServList << "LOG" << "ERR" << "FILE" << "ADDFRIEND" << "ADDGP" << "STEXT" << "ADD" << "GPTEXT";
    //            0        1         2          3            4           5          6        7

    connect(tcpClient,&TcpClientTool::receiveData,this,&readyreadSignals::on_received_selected);
}

void readyreadSignals::on_received_selected(QString str)
{

    QString type = str.section("##",0,0);
    switch (ServList.indexOf(type))
    {
        case 0:
    {
        emit on_LOG_received(str);
        break;
    }
        case 1:
    {
        emit on_ERR_received(str);
        break;
    }
        case 2:
    {
        emit on_FilE_received(str);
        break;
    }
        case 3:
    {
        emit on_ADDFRIEND_received(str);
        break;
    }
    case 4:
    {
        emit on_ADDGP_received(str);
        break;
    }
    case 5:
    {
        emit on_STEXT_received(str);
        break;
    }
    case 6:
    {
        emit on_ADD_received(str);
        break;
    }
    case 7:
    {
        emit on_GPTEXT_received(str);
        break;
    }
    }
}
readyreadSignals *readyreadSignals::readyread = nullptr;
readyreadSignals *readyreadSignals::getins()
{
    if(readyread == nullptr) readyread = new readyreadSignals();
    return readyread;
}
