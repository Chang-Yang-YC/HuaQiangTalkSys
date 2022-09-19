#ifndef GROUPCHATWIDGET_H
#define GROUPCHATWIDGET_H

#include <QWidget>
#include "TcpClientTool.h"
#include <QString>
#include "filesendwidget.h"
#include <QTimer>
QT_BEGIN_NAMESPACE
namespace Ui { class groupchatWidget; }
QT_END_NAMESPACE

class groupchatWidget : public QWidget
{
    Q_OBJECT

public:
    groupchatWidget(QWidget *parent = nullptr);
    ~groupchatWidget();
    void start(int toid,int meid,QString name);
private slots:
    void on_pushButtonsend_clicked();
    void on_txt_received();
    void on_pushButtonfile_clicked();


private:
    Ui::groupchatWidget *ui;
    TcpClientTool *tcpClient;
    fileSendWidget fileSendWin;
    //文件接受
    QFile file; //文件对象
    QString fileName; //文件名字
    qint64 fileSize; //文件大小
    qint64 recvSize; //已经接收文件的大小
    int meId;
    int senderId;
    QString friendname;
    bool isStart;   //标志位，是否为头部信息
    QTimer *timer;
};
#endif // CHATWIDGET_H
