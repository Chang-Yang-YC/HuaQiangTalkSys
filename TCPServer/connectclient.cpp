#include "connectclient.h"
#include"connectionFun.h"
#include"DEBUG.h"
#include "connectclient.h"

#include<QSqlRecord>

ConnectClient::ConnectClient(QTcpSocket *socket, QObject *parent) : QThread(parent)
{
    thisSocket = socket;
    ServList << "LOG" << "ADD" << "STEXT" << "ADDFRIEND" << "ADDGP" << "GPTEXT" << "FILE";
    //            0        1         2            3            4           5          6
    nowOnline = OnlineSocketList::getins();
    //建立数据库的连接
    Serverdb = QSqlDatabase::addDatabase("QSQLITE");
    Serverdb.setDatabaseName("../MultiUserTcpServer/Server.db");
    bool isopen = Serverdb.open();
    if(false == isopen){
        qDebug() << "数据库打开失败\n";
    }
    else
    {
        qDebug() << "数据库打开成功\n";
    }
}


void ConnectClient::run()
{
    QString ip = thisSocket->peerAddress().toString();
    quint16 port = thisSocket->peerPort();
    QString temp = QString("[%1,%2]:连接成功\n").arg(ip),arg(port);
    qDebug() << temp;

    //在数据库中找到对应端口的用户ID
    int thisID = 0;
    QSqlQuery query;
    QString QueStr = QString("SELECT id FROM User WHERE port = %1;").arg(port);
    //获取用户的id
    query.exec(QueStr);
    query.next();
    thisID = query.record().value("id").toInt();

    if(thisID == 0){
        qDebug() << "TCP客户端已连接：未能获取用户ID\n";
    }
    else
    {
        nowOnline->addOnlineSocketNode(thisID,port,thisSocket);  //插入在线用户列表，方便找套接字
    }
    //开始通讯,读写套接字的过程，对方发送完后产生readyread（）
    connect(thisSocket,&QTcpSocket::readyRead,this,&ConnectClient::on_tcpSocket_readyRead);
    //将收到的信息进行转发
}

void ConnectClient::on_tcpSocket_readyRead()
{

    QString Str = thisSocket->readAll();
    QString ServType = Str.section("##",0,0);
    switch (ServList.indexOf(ServType))
    {
//#############################################  LOG  ######################################################
    case 0://登录的同时处理掉Undolist中与这个账号有关的信息。
    {
        //登录操作
        //Str = LOG##账号##密码
        /*BackWord返回形式：
         * LOG## +
         *       DONE##SUCCESS 登陆成功
         *       ERR##NOID  找不到此ID
         *       ERR##NOPW  密码错误
         */
        int ID = Str.section("##",1,1).toInt();
        QString Password = Str.section("##",2,2);
        QString CheckPW = NULL;
        QString BackWord = "LOG##";
        QString Undo = NULL;
        QSqlQuery query;
        QString QueStr = QString("SELECT password FROM User WHERE id = %1;").arg(ID);
        query.exec(QueStr);
        query.next();
        CheckPW = query.record().value("password").toString();//获取值

        if(CheckPW == NULL)  //找不到此ID
        {
            BackWord += "ERR##NOID";
            qDebug() << BackWord << ":" << ID << "\n";
        }
        else if(Password.compare(CheckPW) == 0)  //密码一样
        {
            qDebug() << "用户" << Str.section("##",1,1) << "已登录，正在查找undo\n";
            BackWord += "DONE##SUCCESS";
            thisSocket->write(BackWord.toUtf8().data());
            nowOnline->addOnlineSocketNode(ID,thisSocket->peerPort(),thisSocket);  //将用户ID与套接字加入在线链表，port不重要
            //查找这个id的所有信息并在undolist里删除记录；

                QSqlQuery query;
                QString str = QString("SELECT * FROM Undo WHERE receive_id = '%1';").arg(ID);
                query.exec(str);
                //获取所有的undo的信息；
                //查找它的套接字并传输信息；
                debug(query);
                while(query.next())
                {  //Undo初始值为“STEXT”
                    Undo = "STEXT##" +
                            query.record().value("sender_id").toString() +"##"+
                            query.record().value("receive_id").toString() + "##" +
                            query.record().value("Date").toString() +"##"+
                            query.record().value("Time").toString() +"##"+
                            query.record().value("text").toString()
                            ;//改成传输字符串的格式就行
                    //Undo = query.record().value("text").toString();
                    //利用套接字传输
                    thisSocket->write(Undo.toUtf8().data());
                    //BackWord += "##" + str;
                    qDebug() << 1;
                }
                //利用套接字传输
                //thisSocket->write(Undo.toUtf8().data());
                //qDebug() << Undo;
                //将所有与上线的号相关的内容删掉
                query.exec("DELETE FROM Undo WHERE receive_id = '"+Str.section("##",1,1)+"';");
        }
        else           //密码错误
        {
            BackWord += "ERR##NOPW";
            qDebug() << BackWord << "\nInput:" << Password << "\nReal:" << CheckPW << "\n";
            thisSocket->write(BackWord.toUtf8().data());
        }
        //thisSocket->write(BackWord.toUtf8().data());
        break;
    }
//###################################################  ADD  #################################################
    case 1:
    {
        //新建用户操作
        //Str = ADD##账号##密码##用户名##头像号##ip##Port
        /*BackWord返回形式：
         * ADD## +
         *      DONE   成功添加
         *      ERR##SAMEID  已存在该ID
         *      ERR##SAMEPORT  已存在该PORT **不再要此项（port是自动分配的，不是指定的）
         */
        int NewID = Str.section("##",1,1).toInt();
        QString NewPW = Str.section("##",2,2);
        QString NewName = Str.section("##",3,3);
        int NewPic = Str.section("##",4,4).toInt();
        QString Newip = Str.section("##",5,5);
        int NewPort = Str.section("##",6,6).toInt();
        QString BackWord = "ADD##";
        QSqlQuery query;
        QString str = QString("SELECT * FROM User WHERE ID = %1;").arg(NewID);
        query.exec(str);
        if(query.next())//已存在该ID
        {
            BackWord += "SAMEID";
        }
        else//不存在该ID
        {
            BackWord += "DONE";
            QString QueStr = QString("insert into User values(%1,'%2',%3,'%4','%5',%6);").arg(NewID).arg(Newip).arg(NewPort).arg(NewPW).arg(NewName).arg(NewPic);
            query.exec(QueStr);
            //debug(query);
            query.lastError();
            //在Grou表中加一个新的用户
            QueStr = QString("insert into Grou (id) values(%1);").arg(NewID);
            query.exec(QueStr);
        }
        qDebug() << BackWord;
        thisSocket->write(BackWord.toUtf8().data());
        break;
    }
//##################################################  STEXT  ################################################
    case 2:
    {
        // 一对一聊天，直接转发
        //Str = STEXT##发送者ID##对方ID##DATE##Time##内容
        /*BackWord返回形式：
         * STEXT## +
         *      DONE  私聊消息发送成功
         *      ERR##OFFLINE  对方不在线
         */
        int senderID = Str.section("##",1,1).toInt();
        int ToID = Str.section("##",2,2).toInt();
        QTcpSocket *ToSocket = NULL;
        QString BackWord = "STEXT##";
        ToSocket = nowOnline->GetSocket(ToID);
        if(ToSocket == 0)  //在线列表中无此用户
        {
            BackWord += "ERR##OFFLINE";
            qDebug() << "目标用户离线\n";
            thisSocket->write(BackWord.toUtf8().data());
            //存入undo数据库

                QSqlQuery que;
                QString str = QString("INSERT INTO Undo(sender_id, receive_id, Date, Time, text)"
                                                      "VALUES(%1, %2, '%3', '%4', '%5');").arg(senderID).arg(ToID)
                                        .arg(Str.section("##",3,3)).arg(Str.section("##",4,4)).arg(Str.section("##",5,5));
                                que.exec(str);
                                qDebug() << que.lastError();
//                que.prepare("INSERT INTO Undo(sender_id, receive_id, Date, Time, text)"
//                               "VALUES(?, ?, ?, ?, ?);");
//                que.addBindValue(senderID);//发送者的ID也要传过来。。。
//                que.addBindValue(ToID);
//                que.addBindValue(Str.section("##",3,3));
//                que.addBindValue(Str.section("##",4,4));
//                que.addBindValue(Str.section("##",5,5));
//                debug(que);

        }
        else  //对方在线，得到套接字
        {
            ToSocket->write(Str.toUtf8().data());
            qDebug() << Str;
            BackWord += "DONE";
            //nowOnline->GetSocket(ToID)->write(Str.toUtf8().data());
            thisSocket->write(BackWord.toUtf8().data());
        }
        break;
    }
//#############################################  ADDFRIEND  ######################################################
    case 3:
    {
        //添加好友
        //ADDFRIEND##目标好友ID
        /*BackWord返回形式：
         * ADDFRIEND## +
         *      DONE  找到该用户
         *      ERR   不存在
         *      AGREE##对方账号
         */
        int ID = Str.section("##", 1, 1).toInt();
        QString BackWord = "ADDFRIEND##";
        QSqlQuery query;
        QString str = QString("SELECT * FROM User WHERE id = %1;").arg(ID);
        query.exec(str);
        if(query.next())//找到这个注册人了
        {
            BackWord += "DONE";
        }
        else//不存在该ID
        {
            BackWord += "ERR";
        }
        QString Agree = QString("ADDFRIEND##AGREE##%1").arg(ID);
        //nowOnline->GetSocket(ID)->write(Agree.toUtf8().data());
        thisSocket->write(BackWord.toUtf8().data());
        break;
    }
//#############################################  ADDGP  ######################################################

    case 4:
    {
        //将加入的群聊信息写入群的数据库，且对群数据库做修改
        //添加群聊；ADDGP##群ID##群名##本人ID
        /*BackWord返回形式：
         * ADDGP## +
         *      JOINGP  群已经存在；则添加进去
         *      CRTEGP  群不存在，则创建
         */
        int ID = Str.section("##", 1, 1).toInt();
        int clientID = Str.section("##", 3, 3).toInt();

        QString name_ = Str.section("##", 2, 2);
        QString BackWord = "ADDGP##";
        QSqlQuery query;
        QString str = QString("SELECT * FROM GrouID WHERE id = %1;").arg(ID);
        query.exec(str);
        qDebug() << query.lastError();
        if(query.next())//群已经存在；则添加进去；
        {
            BackWord += "JOINGP";

            //注意要固定一下每个群聊的ip 和port 啥的。。。
            //修改群的数据库
            QSqlQuery que;
            QString str = QString("UPDATE Grou SET groupid%1 = %2 WHERE id = %3;").arg(ID).arg(1).arg(clientID);
            que.exec(str);
            qDebug() << que.lastError();
        }
        else//不存在该ID
        {
            BackWord += "CRTEGP";
            // 添加一个群聊，在数据库中加一列,在GrouID里添加数据

            QSqlQuery que("INSERT INTO GrouID(id, name1) VALUES(?, ?);");
            que.addBindValue(ID);
            que.addBindValue(name_);
            que.exec();
            qDebug()<<ID;
            qDebug()<< clientID;
            str = QString("ALTER TABLE Grou ADD COLUMN groupid%1 int;").arg(ID);
            que.exec(str);
            str = QString("UPDATE Grou SET groupid%1 = %2 WHERE id = %3;").arg(ID).arg(1).arg(clientID);
            que.exec(str);
            qDebug() << que.lastError();
        }
        thisSocket->write(BackWord.toUtf8().data());
        break;
    }
//#############################################  GPTEXT  ######################################################

    case 5://在群聊中发消息； GPTEXT##群ID##memberID##Time##Date##message
        //BackWord返回格式：
        //GPTEXT## +
        //      DONE  存在该群且消息发出（可能会因群成员不在线而转入undo数据库）
        //      ERR##NOGP   代表不在这个群，可能因为没加入群，也可能这个群不存在
        //
    {
        int ID = Str.section("##",1 ,1).toInt();
        qDebug() << Str;
        QTcpSocket *ToSocket = NULL;
        QString BackWord = "GPTEXT##";
        //利用ID搜出每个群用户在线否
        QSqlQuery query;
        //先判断是否存在这个群；
        QString str = QString("SELECT * FROM GrouID WHERE id = %1;").arg(ID);
        query.exec(str);
        if(query.next())//代表有这个群id存在
        {
            str = QString("SELECT id FROM Grou WHERE groupid%1 = %2;").arg(ID).arg(1);
            query.exec(str);
            while(query.next())//在群里的所有成员
            {
                BackWord += "DONE";
                int memberID = query.record().value("id").toInt();
                //STEXT##发送者ID##对方ID##DATE##Time##内容
                QString Send = QString("STEXT##%1##%2##%3##%4##%5").arg(Str.section("##",1,1)).arg(memberID).arg(Str.section("##",2,2)).arg(Str.section("##",3,3)).arg(Str.section("##",4,4));
                ToSocket = nowOnline->GetSocket(memberID);
                qDebug() << Send;
                if(ToSocket == 0)//群聊用户不在线；
                {
                    //可以同理写到undo的数据库里，但是端口啥的不太确定，再说
                }
                else//某群友在线
                {
                    ToSocket->write(Send.toUtf8().data());
                }
            }
        }
        else
        {   //无这个群号
            BackWord += "ERR##NOGP";
        }
        thisSocket->write(BackWord.toUtf8().data());
        break;
    }
//#############################################  FILE  ######################################################
    case 6:
    {
        //文件接受发送
        //str = FILE##对方ID##MeId##文件内容
        /*BackWord返回格式：
         * FILE## +
         *   RECIVED  正在接收
         *   OFFLINE  对方不在线
         */
        ToId = Str.section("##",1,1).toInt();

        QString BackWord = "GPTEXT##";
        QTcpSocket *ToSocket = NULL;
        ToSocket = nowOnline->GetSocket(ToId);
        if(ToSocket == 0){ //对方不在线,转入undo数据库  *未实现
            BackWord += "OFFLINE";
            //
            //
            //
        }
        else
        {
            ToSocket->write(Str.toUtf8().data());
        }
        thisSocket->write(BackWord.toUtf8().data());
    }
    }

}

void ConnectClient::sendFile()   //目前用不到，可不管
{
    QTcpSocket *ToSocket = NULL;
    QString BackWord;
    ToSocket = nowOnline->GetSocket(ToId);
    file.open(QIODevice::ReadOnly);
    //先发送文件头信息  文件名##文件大小
    QString head = QString("%1##%2").arg(fileName).arg(fileSize);
    //发送头部信息
    qint64 len = ToSocket->write( head.toUtf8() );
    if(len > 0)//头部信息发送成功
    {
        //发送真正的文件信息
        //防止TCP黏包
        //需要通过定时器延时 20 ms
        timer.start(20);
    }
    else
    {
        qDebug() << "头部信息发送失败 142";
        file.close();
    }


    connect(&timer, &QTimer::timeout,
            [=]()
            {
                //关闭定时器
                timer.stop();

                //发送文件

                 qint64 len = 0;
                 do
                 {
                    //每次发送数据的大小
                    char buf[4*1024] = {0};
                    len = 0;

                    //往文件中读数据
                    len = file.read(buf, sizeof(buf));

                    //发送数据，读多少，发多少
                    ToSocket->write(buf, len);

                    //发送的数据需要累积
                    sendSize += len;


                 }while(len > 0);
            }
            );
}
