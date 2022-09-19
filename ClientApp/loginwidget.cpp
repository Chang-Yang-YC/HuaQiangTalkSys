#include "loginwidget.h"
#include "ui_loginwidget.h"

#include<QDialog>
#include<QDebug>
#include<QMessageBox>
#include<QSqlDatabase>
#include<QSqlError>
#include<QSqlQuery>
#include<QSqlQueryModel>
#include<QString>
#include<QSqlRecord>
#include<QSqlTableModel>
#include<QTableView>
#include<QCommandLinkButton>
loginWidget::loginWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::loginWidget)
{
    ui->setupUi(this);
    //创建tcpClient对象
    tcpClient = TcpClientTool::getins();
    readyread = readyreadSignals::getins();
    tcpClient->connectTcp("192.168.159.128",9980);

    Booler = "";
    connect(&reg,&registerWidget::signal_ToLogin,this,&loginWidget::loginAfterRegister);
}

loginWidget::~loginWidget()
{
    delete ui;
}


void loginWidget::on_commandLinkButton_clicked()
{
    this->hide();
    reg.show();
}

void loginWidget::loginAfterRegister()
{
    reg.hide();
    this->show();
}


void loginWidget::on_pushButtonLog_clicked()
{
        QString id_;
        QString code_;
        id_ = ui->lineEdit->text();
        code_ = ui->lineEdit_2->text();
        //传给服务器数据
        //登录操作    id_  code_
        //Str = LOG##账号##密码
        QString str = QString("LOG##%1##%2").arg(id_).arg(code_);
        //%%%%
        tcpClient->write(str);

        connect(readyread,&readyreadSignals::on_LOG_received,
                [=](QString buf)
        {   //LOG##DONE
            //LOG##ERR
            Booler = buf.section("##",1,1);
            if(!Booler.compare("DONE"))//登陆成功
            {
                //跳转到好友界面
                mainWin.setUserInfo(id_.toInt());
                mainWin.show();
                //隐藏登录界面；
                this->hide();
            }
            else
            {
                QMessageBox msg(this);
                msg.setWindowTitle("提示");
                //根据传输回来的字符串做一个简单的if-else
                msg.setText("请输入正确的密码或账号！");
                msg.exec();
            }
        });
        //%%%%
        //返回的串：
        //STEXT##string1##string2....
        //LOG##DONE
        //LOG##ERR

}
