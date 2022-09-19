#include "addchatgroupwidget.h"
#include "ui_addchatgroupwidget.h"

#include <QtDebug>

AddChatGroupWidget::AddChatGroupWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddChatGroupWidget)
{
    ui->setupUi(this);
    tcpClient = TcpClientTool::getins();
    readyread = readyreadSignals::getins();
}

AddChatGroupWidget::~AddChatGroupWidget()
{
    delete ui;
}

void AddChatGroupWidget::userinfo(int userid)
{
    meid = userid;
}

void AddChatGroupWidget::on_pushButtonSure_clicked()
{
    if(ui->lineEdit->text() != "")
    {
        int id = ui->lineEdit->text().toInt();
        QString chatGroupName = QString("群聊%1").arg(id);
        //将加入的群聊信息写入群的数据库，且对群数据库做修改
        //添加群聊；ADDGP##群ID##群名##本人ID
                //添加群聊；ADDGP##群ID##群名
                /*BackWord返回形式：
                 * ADDGP## +
                 *      JOINGP  群已经存在；则添加进去
                 *      CRTEGP  群不存在，则创建
                 */

        //与服务器通讯
        QString head = QString("ADDGP##%1##%2##%3").arg(id).arg(chatGroupName).arg(meid);
        tcpClient->write(head);
        qDebug() << head;
        connect(readyread,&readyreadSignals::on_ADDGP_received,
                [=](QString str)
        {
            QString isDONE = str.section("##",1,1);

        });
            emit signalAddChatGroup(id);
            //发送信号
        this->hide();
    }

}

void AddChatGroupWidget::on_pushButtonCancel_clicked()
{
    ui->lineEdit->setText("");
    this->hide();
}
