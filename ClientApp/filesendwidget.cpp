#include "filesendwidget.h"
#include "ui_filesendwidget.h"
#include <QFileDialog>
fileSendWidget::fileSendWidget(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::fileSendWidget)
{
    ui->setupUi(this);

    tcpClient = TcpClientTool::getins();

    ui->progressBar->setValue(0);

    connect(&timer, &QTimer::timeout,
            [=]()
            {
                //关闭定时器
                timer.stop();

                //发送文件
                sendData();
            }

            );

//    connect(tcpClient, &,
//            [=]()
//            {
//                //取客户端的信息
//                QByteArray buf = tcpClient->readfileall();
//                if(QString(buf) == "file done")
//                {//文件接收完毕
//                     ui->textEdit->append("文件发送完毕");
//                     file.close();
//                }
//            }
//            );
}

fileSendWidget::~fileSendWidget()
{
    delete ui;
}

void fileSendWidget::sendData()
{
    ui->textEdit->append("正在发送文件……");
     qint64 len = 0;
     do
     {
         //文件头
         QString filehead = QString("FILE##%1##%2##").arg(ToId).arg(MeId);
        //每次发送数据的大小
        char buf[64] = {0};
        len = 0;

        //往文件中读数据
        len = file.read(buf,sizeof(buf));
        QString sender = filehead + QString(buf);
        ui->textEdit->append(QString("%1").arg(len));
        ui->textEdit->append(sender);
        //发送数据，读多少，发多少
        tcpClient->sendFile(sender);
        tcpClient->flushs();
        //发送的数据需要累积
        sendSize += len;

        ui->progressBar->setMinimum(0); //最小值
        ui->progressBar->setMaximum(fileSize/1024); //最大值
        ui->progressBar->setValue(sendSize/1024); //当前值
        QTime time;
        time.start();
        while(time.elapsed() < 1000)             //等待时间流逝5秒钟
            QCoreApplication::processEvents();   //处理事件
     }while(len > 0);
     ui->textEdit->append("发送完毕");
}

void fileSendWidget::setId(int toid, int meid)
{
    ToId = toid;
    MeId = meid;
}

void fileSendWidget::on_pushButtonSelect_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "open", "../");
    if(false == filePath.isEmpty()) //如果选择文件路径有效
    {
        fileName.clear();
        fileSize = 0;

        //获取文件信息
        QFileInfo info(filePath);
        fileName = info.fileName(); //获取文件名字
        fileSize = info.size(); //获取文件大小

        sendSize = 0; //发送文件的大小

        //只读方式打开文件
        //指定文件的名字
        file.setFileName(filePath);

        //打开文件
        bool isOk = file.open(QIODevice::ReadOnly);
        if(false == isOk)
        {
            ui->textEdit->append( "只读方式打开文件失败 106");
        }

        //提示打开文件的路径
        ui->textEdit->append(filePath);

        ui->pushButtonSelect->setEnabled(false);
        ui->pushButtonsend->setEnabled(true);

    }
    else
    {
        ui->textEdit->append( "选择文件路径出错 118");
    }
}

void fileSendWidget::on_pushButtonsend_clicked()
{
    ui->pushButtonsend->setEnabled(false);
    QString filehead = QString("FILE##%1##%2##").arg(ToId).arg(MeId);
    //先发送文件头信息  文件名$$文件大小
    QString head = QString("%1$$%2").arg(fileName).arg(fileSize);
    head = filehead + head;
    ui->textEdit->append(head);
    //发送头部信息
    qint64 len = tcpClient->sendFile(head);
    tcpClient->flushs();
    if(len > 0)//头部信息发送成功
    {
        //发送真正的文件信息
        //防止TCP黏包
        //需要通过定时器延时 20 ms
        timer.start(1000);
    }
    else
    {
        ui->textEdit->append("头部信息发送失败 142");
        file.close();
        ui->pushButtonSelect->setEnabled(true);
        ui->pushButtonsend->setEnabled(false);
    }
}
