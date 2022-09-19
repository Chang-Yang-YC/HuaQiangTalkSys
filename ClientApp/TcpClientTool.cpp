#include "TcpClientTool.h"
#include <QFileDialog>
TcpClientTool::TcpClientTool(QObject *parent)
	: QObject(parent)
{

	m_socket = new QTcpSocket(this);
    fileSocket = new QTcpSocket(this);
	connect(m_socket, &QTcpSocket::disconnected, this
		, &TcpClientTool::disConnect_signal);
	connect(m_socket, &QTcpSocket::readyRead, this
		, &TcpClientTool::receivData);
    //(fileSocket,&QTcpSocket::readyRead,this,&TcpClientTool::receivFile);
}

TcpClientTool::~TcpClientTool()
{
}

TcpClientTool *TcpClientTool::tcpClient = nullptr;
TcpClientTool *TcpClientTool::getins()
{
    if(tcpClient == nullptr) tcpClient = new TcpClientTool();
    return tcpClient;
}
bool TcpClientTool::connectTcp(QString ip, int portNum)
{
	m_socket->abort();

	m_socket->connectToHost(ip, portNum);
    //文件套接字连接
    fileSocket->abort();
    fileSocket->connectToHost(ip,portNum);

    if (m_socket->waitForConnected(30000)&&fileSocket->waitForConnected(30000))
	{
		return true;
	}
	else
		return false;
}

void TcpClientTool::disConnect()
{
	m_socket->abort();
    m_socket->disconnect();
}

void TcpClientTool::flushs()
{
    m_socket->flush();
}

void TcpClientTool::write(QString str)
{
    m_socket->write(str.toUtf8().data());
}

QByteArray TcpClientTool::readfileall()
{
    QByteArray buf = m_socket->readAll();
    return buf;
}



qint64 TcpClientTool::sendFile(QString str,qint64 len)
{
    qint64 byte = m_socket->write(str.toUtf8(),len);
    return byte;
}

qint64 TcpClientTool::sendFile(QString str)
{
    qint64 byte = m_socket->write(str.toUtf8());
    return byte;
}


void TcpClientTool::receivData()
{
    QString str = m_socket->readAll();
    emit receiveData(str);
}

//void TcpClientTool::receivFile()
//{
//    emit receiveFile();
//}

