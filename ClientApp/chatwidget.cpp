#include "chatwidget.h"
#include "ui_chatwidget.h"
#include "messagecontrol.h"
#include "connectionFun.h"
#include<QMessageBox>
#include <QDateTime>
#include<QSqlQuery>
#include<QDebug>
#include<QSqlError>
#include <QSqlRecord>
chatWidget::chatWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::chatWidget)
{
    ui->setupUi(this);

        //定义tcpClienttool类
        tcpClient = TcpClientTool::getins();
        timer = new QTimer(this);

        //工具栏按钮设置
        QIcon iconsmile; //新建QIcon对象
        iconsmile.addFile(tr(":/icon/emoji.png")); //让QIcon对象指向想要的图标
        ui->pushButtonsmile->setIcon(iconsmile); //给按钮添加图标
        ui->pushButtonsmile->setIconSize(QSize(30,30));//重置图标大小
        ui->pushButtonsmile->setStyleSheet("background-color:rgba(0,0,0,0)");
        ui->pushButtonsmile->setFocusPolicy(Qt::NoFocus);


        QIcon icontxtsize; //新建QIcon对象
        icontxtsize.addFile(tr(":/icon/font.png")); //让QIcon对象指向想要的图标
        ui->pushButtontxtsize->setIcon(icontxtsize); //给按钮添加图标
        ui->pushButtontxtsize->setIconSize(QSize(30,30));//重置图标大小
        ui->pushButtontxtsize->setStyleSheet("background-color:rgba(0,0,0,0)");
        ui->pushButtontxtsize->setFocusPolicy(Qt::NoFocus);

        QIcon iconfile; //新建QIcon对象
        iconfile.addFile(tr(":/icon/file.png")); //让QIcon对象指向想要的图标
        ui->pushButtonfile->setIcon(iconfile); //给按钮添加图标
        ui->pushButtonfile->setIconSize(QSize(30,30));//重置图标大小
        ui->pushButtonfile->setStyleSheet("background-color:rgba(0,0,0,0)");
        ui->pushButtonfile->setFocusPolicy(Qt::NoFocus);

        QIcon icontelephone; //新建QIcon对象
        icontelephone.addFile(tr(":/icon/telephone.png")); //让QIcon对象指向想要的图标
        ui->pushButtontelephone->setIcon(icontelephone); //给按钮添加图标
        ui->pushButtontelephone->setIconSize(QSize(25,25));//重置图标大小
        ui->pushButtontelephone->setStyleSheet("background-color:rgba(0,0,0,0)");
        ui->pushButtontelephone->setFocusPolicy(Qt::StrongFocus);
        //工具栏样式设置
        ui->frame->setLineWidth(3);
        ui->frame->setMidLineWidth(0);
        ui->frame->setFrameShape(QFrame::Panel);
        ui->frame->setFrameShadow(QFrame::Raised);
        //用户名  即好友ID
        ui->label_username->setText("LUFF");

        //接受信息  此处连接数据库
        connect(timer,&QTimer::timeout,this,&chatWidget::on_txt_received);

}

chatWidget::~chatWidget()
{
    delete ui;
}

void chatWidget::start(int toid, int meid,QString name)
{
    senderId = toid;
    meId = meid;
    friendname = name;
    timer->start(1000);
    ui->label_username->setText(friendname);
    this->show();
}


void chatWidget::on_pushButtonsend_clicked()
{
    //Str = STEXT##发送者ID##对方ID##DATE##Time##内容
    QDateTime current_date_time =QDateTime::currentDateTime();
    //current_date字符串结果为"2016.05.20 12:17:01.445"
    QString current_date =current_date_time.toString("yyyy.MM.dd");
    QString current_Time = current_date_time.toString("hh:mm:ss.zzz");
    QString head = QString("STEXT##%1##%2##%3##%4##").arg(meId).arg(senderId).arg(current_date).arg(current_Time);
    QString str = head + ui->textEditsend->toPlainText();
    tcpClient->write(str);
    ui->textEditsend->clear();
}

void chatWidget::on_txt_received()
{
    //id还没传进来
    if(connection("../message.db"))
        {
            //选择所有的消息
        //选择所有的消息
            QSqlQuery query;
                    //现将消息按照时间顺序排序
           QString str1 = QString("SELECT * FROM friendMessage order by date desc, time desc;");
           query.exec(str1);

           QString str = QString("SELECT * FROM friendMessage WHERE (receiver = %1 AND sender = %2) OR "
                                          "(receiver = %2 AND sender = %1);").arg(meId).arg(senderId);

            query.exec(str);
            qDebug()<<query.lastError();
            //建一个很大的string来显示
            QString showString = NULL;
            while(query.next())
            {
                //[from a]/time/date:message/n
                showString += "[from" + query.record().value("sender").toString() + "]/" +
                        query.record().value("date").toString() +"/" +
                        query.record().value("time").toString() + ":" +
                        query.record().value("msg").toString() + "\n";//可能会出现在一行的情况。。。。
            }
            ui->textEditreceive->setText(showString);
        }
}

void chatWidget::on_pushButtonfile_clicked()
{
    fileSendWin.show();
    fileSendWin.setId(senderId,meId);
}

