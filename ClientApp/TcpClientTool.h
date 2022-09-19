#pragma once

#include <QObject>
#include <QtNetwork>
#include <QTcpSocket>


class TcpClientTool : public QObject
{
	Q_OBJECT

public:
	explicit TcpClientTool(QObject *parent = nullptr);

	~TcpClientTool();

	bool connectTcp(QString ip, int portNum);

	void disConnect();
    void flushs();

    void write(QString str);
    //读取文件套接字中的信息
    QByteArray readfileall();

    //发送文件
    qint64 sendFile(QString str,qint64 len);
    qint64 sendFile(QString str);
    //更新文件发送进度
    void updateSendedFileProgress(qint64);
    //更新文件接收进度
    void updateReceivedFileProgress();
    //函数实例化
    static TcpClientTool *tcpClient;
    static TcpClientTool *getins();

signals:

	void disConnect_signal();

    void receiveData(QString data);

    void receiveFile();
public slots:

private:

	QTcpSocket* m_socket;
    QTcpSocket *fileSocket;

    //文件传送
        QFile *localFile;
        //文件大小
        qint64 totalBytes;      //文件总字节数
        qint64 bytesWritten;    //已发送的字节数
        qint64 bytestoWrite;    //尚未发送的字节数
        qint64 filenameSize;    //文件名字的字节数
        qint64 bytesReceived;   //接收的字节数
        //每次发送数据大小
        qint64 perDataSize;
        QString filename;
        //数据缓冲区
        QByteArray inBlock;
        QByteArray outBlock;

	void receivData();
    void receivFile();
};

extern TcpClientTool *tcpClient;
