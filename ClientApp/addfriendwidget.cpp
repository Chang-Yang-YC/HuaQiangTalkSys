#include "addfriendwidget.h"
#include "ui_addfriendwidget.h"

#include <QPushButton>
#include <QtDebug>

AddFriendWidget::AddFriendWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddFriendWidget)
{
    ui->setupUi(this);
    tcpClient = TcpClientTool::getins();
    readyread = readyreadSignals::getins();
    //将确认添加按钮关闭
    ui->pushButtonAdd->setEnabled(false);

}

AddFriendWidget::~AddFriendWidget()
{
    delete ui;
}

void AddFriendWidget::on_pushButtonSearch_clicked()
{
    //当内容不为空时与服务器通讯
    if(ui->lineEdit->text()!= "")
    {
        qDebug()<< "TODO: 用id检索待添加用户是否存在";
        //添加好友
                //ADDFRIEND##目标好友ID
                /*BackWord返回形式：
                 * ADDFRIEND## +
                 *      DONE  找到该用户
                 *      ERR   不存在
                 */
        QString head = QString("ADDFRIEND##%1").arg(ui->lineEdit->text());
        tcpClient->write(head);
    }
    connect(readyread,&readyreadSignals::on_ADDFRIEND_received,
            [=](QString str)
    {
        QString isDONE = str.section("##",1,1);

        if(isDONE == "DONE"){
            //假设完成通讯
            ui->labelMessage->setText("用户存在");
            ui->pushButtonAdd->setEnabled(true);
        }
        else if(isDONE == "ERR"){
            //用户不存在
            ui->labelMessage->setText("用户不存在，无法添加！");
        }
    });
}

void AddFriendWidget::on_pushButtonCancel_clicked()
{
    //取消，重置页面
    ui->lineEdit->setText("");
    ui->labelMessage->setText("暂无搜索消息。");
    ui->pushButtonAdd->setEnabled(false);
    this->hide();
}

void AddFriendWidget::on_pushButtonAdd_clicked()
{
    int id = ui->lineEdit->text().toInt();
    emit signalAddFriend(id,"逆天小子");
    //重置页面
    ui->lineEdit->setText("");
    ui->labelMessage->setText("暂无搜索消息。");
    ui->pushButtonAdd->setEnabled(false);
    this->hide();
}
