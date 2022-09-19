#include "registerwidget.h"
#include "ui_registerwidget.h"
#include"globel.h"

#include<QSqlDatabase>
#include<QDebug>
#include<QMessageBox>
#include<QSqlError>
#include<QSqlQuery>
#include<QLineEdit>
#include<QString>
registerWidget::registerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::registerWidget)
{
    ui->setupUi(this);
    tcpClient = TcpClientTool::getins();
    readyread = readyreadSignals::getins();
}

registerWidget::~registerWidget()
{
    delete ui;
}

void registerWidget::on_pushButton_clicked()
{
    //按下注册按钮后提交所有内容
    //新建用户操作
    //Str = ADD##账号##密码##用户名##头像号##ip##Port
    QString str = QString("ADD##%1##%2##%3##%4##%5##%6").arg(ui->lineEditID->text()).arg(ui->lineEditCode->text())
            .arg(ui->lineEditName->text()).arg(1).arg(ui->lineEditIP->text()).arg(ui->lineEditPort->text());
    //头像传输一个默认值1
    //传输数据
    tcpClient->write(str);
    //%%%%
    //新建用户操作
            //Str = ADD##账号##密码##用户名##头像号##ip##Port
            /*BackWord返回形式： *未实现
             * ADD## +
             *      DONE   成功添加
             *      ERR##SAMEID  已存在该ID
             *      ERR##SAMEPORT  已存在该PORT
             */
    connect(readyread,&readyreadSignals::on_ADD_received,
            [=](QString str)
    {
        QString ISDONE = str.section("##",1,1);
        if(ISDONE.compare("DONE")){
            emit signal_ToLogin();
        }
        else if(ISDONE.compare("SAMEID")){
            QMessageBox::about(this,"注册失败","ID重复");
        }
        else if(ISDONE.compare("SAMEPORT")){
            QMessageBox::about(this,"注册失败","PORT重复");
        }
        else {
            qDebug() << "注册失败";
        }
    });
    //%%%%
    //后面不需要写了，自动跳转回去

}

