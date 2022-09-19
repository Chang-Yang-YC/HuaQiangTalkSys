#include "receivefilewidget.h"
#include "ui_receivefilewidget.h"

receiveFileWidget::receiveFileWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::receiveFileWidget)
{
    ui->setupUi(this);

    tcpClient = TcpClientTool::getins();
    readyread = readyreadSignals::getins();
    //初始化

    ui->progressBar->setValue(0);
    isStart = true;
    connect(readyread,&readyreadSignals::on_FilE_received,this,&receiveFileWidget::on_file_received);

}

receiveFileWidget::~receiveFileWidget()
{
    delete ui;
}

void receiveFileWidget::on_file_received(QString str)
{
    this->show();
    //取出接收的内容
    //str = FILE##对方ID##MeId##文件内容
    ui->textEdit->append(str);
    QString receivefile = str;
    int toid = receivefile.section("##",1,1).toInt();
    int meid = receivefile.section("##",2,2).toInt();
    QString buf = receivefile.section("##",3,3);
    ui->textEdit->append( buf);
    if(true == isStart)
    {//接收头
        isStart = false;
        //解析头部信息 QString buf = "hello##1024"
        //                    QString str = "hello##1024#mike";
        //                            str.section("##", 0, 0)

        //初始化
        //文件名
        fileName = buf.section("$$", 0, 0);
        //文件大小
        fileSize = buf.section("$$", 1, 1).toInt();
        recvSize = 0;   //已经接收文件大小

        //打开文件
        //关联文件名字
        file.setFileName(fileName);

        //只写方式方式，打开文件
        bool isOk = file.open(QIODevice::WriteOnly);
        if(false == isOk)
        {
            ui->textEdit->append("文件写入失败");
            return; //如果打开文件失败，中断函数
        }

        //弹出对话框，显示接收文件的信息
        QString str = QString("接收的文件: [%1: %2kb]").arg(fileName).arg(fileSize/1024);
        //QMessageBox::information(this, "文件信息", str);
        ui->textEdit->append(str);
        ui->textEdit->append("正在接收文件……");

        //设置进度条
        ui->progressBar->setMinimum(0); //最小值
        ui->progressBar->setMaximum(fileSize/1024); //最大值
        ui->progressBar->setValue(0); //当前值


    }
    else //文件信息
    {
        qint64 len = file.write(buf.toUtf8().data());
        if(len >0) //接收数据大于0
        {
            recvSize += len; //累计接收大小
          ui->textEdit->append(QString("%1").arg(recvSize));
        }

        //更新进度条
        ui->progressBar->setValue(recvSize/1024);

        if(recvSize == fileSize) //文件接收完毕
        {

//            //先给服务发送(接收文件完成的信息)
//            tcpClient->sendFile("file done");

            ui->textEdit->append("文件接收完成");
            file.close(); //关闭文件
            isStart = true;
        }
    }
}




