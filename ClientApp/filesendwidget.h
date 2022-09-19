#ifndef FILESENDWIDGET_H
#define FILESENDWIDGET_H

#include <QMainWindow>
#include "TcpClientTool.h"
#include "readyreadsignals.h"
namespace Ui {
class fileSendWidget;
}

class fileSendWidget : public QMainWindow
{
    Q_OBJECT

public:
    explicit fileSendWidget(QWidget *parent = nullptr);
    ~fileSendWidget();
    void sendData();
    void setId(int toid,int meid);
private slots:
    void on_pushButtonSelect_clicked();

    void on_pushButtonsend_clicked();

private:
    Ui::fileSendWidget *ui;
    TcpClientTool *tcpClient;

    QFile file; //文件对象
    QString fileName; //文件名字
    qint64 fileSize; //文件大小
    qint64 sendSize; //已经发送文件的大小
    int ToId;//接受者ID            此处缺少查找ID函数
    int MeId;
    QTimer timer; //定时器
    QTimer timer1;
};

#endif // FILESENDWIDGET_H
