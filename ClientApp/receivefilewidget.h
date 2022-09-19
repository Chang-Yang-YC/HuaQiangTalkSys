#ifndef RECEIVEFILEWIDGET_H
#define RECEIVEFILEWIDGET_H

#include <QWidget>
#include <TcpClientTool.h>
#include <readyreadsignals.h>
#include<QFile>
#include<QString>
namespace Ui {
class receiveFileWidget;
}

class receiveFileWidget : public QWidget
{
    Q_OBJECT

public:
    explicit receiveFileWidget(QWidget *parent = nullptr);
    ~receiveFileWidget();
    void on_file_received(QString str);
private:
    Ui::receiveFileWidget *ui;
    TcpClientTool *tcpClient;
    readyreadSignals *readyread; //信号接受对象
    //文件接受
    QFile file; //文件对象
    QString fileName; //文件名字
    qint64 fileSize; //文件大小
    qint64 recvSize; //已经接收文件的大小

    bool isStart;   //标志位，是否为头部信息
};

#endif // RECEIVEFILEWIDGET_H
