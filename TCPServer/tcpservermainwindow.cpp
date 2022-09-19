#include "tcpservermainwindow.h"
#include "ui_tcpservermainwindow.h"
#include "connectclient.h"


#include <QDialog>
#include <QMenu>

TcpServerMainWindow::TcpServerMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TcpServerMainWindow)
{
    ui->setupUi(this);

    m_Server = new QTcpServer(this);  //建立连接套接字

    //nowOnline = NULL;
    //nowOnline = new OnlineSocketList;

    //服务器端口固定为9980，开机就开始监听
    m_Server->listen(QHostAddress::Any,9980);

    connect(m_Server,&QTcpServer::newConnection,this,[=]()
    {
        QTcpSocket* m_Socket = m_Server->nextPendingConnection();
        //创建子线程
        ConnectClient *subThread = new ConnectClient(m_Socket);
        int Snum = m_Socket->peerPort();
        QString Show = QString("线程：%1已建立\n").arg(Snum);
        ui->textBrowserShow->append(Show);
        subThread->start();
    });

}

TcpServerMainWindow::~TcpServerMainWindow()
{
    delete ui;
}

void TcpServerMainWindow::on_actionshowUserList_triggered()
{
    UserListDialog = new UserdbDialog(this);
    UserListDialog->setModal(false);
    UserListDialog->show();  //显示用户列表
}

void TcpServerMainWindow::on_actionshowGroupList_triggered()
{
    GroupListDialog = new GroupdbDialog(this);
    GroupListDialog->setModal(false);
    GroupListDialog->show();  //显示群组列表
}

void TcpServerMainWindow::on_actionshowUndoList_triggered()
{
    UndoListDialog = new UndodbDialog(this);
    UndoListDialog->setModal(false);
    UndoListDialog->show();  //显示未完成事务列表
}

void TcpServerMainWindow::on_actionshowServiceList_triggered()
{

}
