#include "messagecontrol.h"
#include "connectionFun.h"
#include"DEBUG.h"
#include "ui_messageControl.h"
#include<QSqlQuery>
#include<QDebug>
#include<QSqlError>

messageControl::messageControl(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::messageControl)
{
     ui->setupUi(this);
    tcpClient = TcpClientTool::getins();
    readyread = readyreadSignals::getins();
    connect(readyread,&readyreadSignals::on_STEXT_received,
            [=](QString str)
    {
        // 一对一聊天，直接转发
                //Str = STEXT##发送者ID##对方ID##DATE##Time##内容
                /*BackWord返回形式：
                 * STEXT## +
                 *      DONE  私聊消息发送成功
                 *      ERR##OFFLINE  对方不在线
                 */
        //message(QString u1,QString u2, QString word, QTime t,QDate d, int j, QString t1)
         //:user1(u1),user2(u2),text(word),time(t),judge(j),date(d),type(t1){};
        ui->textEdit->append(str);
        QString sendId = str.section("##",1,1);
        QString TOId = str.section("##",2,2);
        QString word = str.section("##",5,5);
        QTime t = QTime::fromString(str.section("##",4,4),"hh:mm:ss.zzz");
        QDate date = QDate::fromString(str.section("##",3,3),"yyyy.MM.dd");

       msg1 = message(sendId,TOId,word,t,date,0,"STEXT");//不知道最后一个类型的表示符
       if (connection("../message.db"))
          {
              //先建立一个表，然后根据传送来的信息判断是群组消息还是个人消息。删除还是添加
              QSqlQuery createTable("CREATE TABLE friendMessage(sender VARCHAR(255),receiver VARCHAR(255),"
                  "msg VARCHAR(255),time VARCHAR(255),date VARCHAR(255), type_msg VARCHAR(255),"
                  "PRIMARY KEY(sender, receiver, time, date));");
              debug(createTable);//执行加在控制台反馈执行情况

              //传输过程中会存在类型的转化，注意一下(QTime->QString)
              QString convertedTime = msg1.time.toString();//格式问题
              QString convertedDate = msg1.date.toString();
              if (msg1.judge)//删除消息
              {
                  QSqlQuery deleteMsg;

                  QString sql = QString("DELETE FROM friendMessage WHERE "
                      "sender = '%1' AND receiver = '%2' AND time = '%3' AND date = '%4';").arg(msg1.user1).arg(msg1.user2).arg(convertedTime)
                      .arg(convertedDate);
                  deleteMsg.exec(sql);
              }
              else
              {
                  QSqlQuery add;
                  QString sql = QString("INSERT INTO friendMessage(sender, receiver, msg, time, date, type_msg)"
                      "VALUES('%1', '%2', '%3', '%4', '%5', '%6');").arg(msg1.user1).arg(msg1.user2).arg(msg1.text)
                      .arg(convertedTime).arg(convertedDate).arg(msg1.type);
                  add.exec(sql);
              }
          }
    });

    connect(readyread,&readyreadSignals::on_GPTEXT_received,
            [=](QString str)
    {
        //在群聊中发消息； GPTEXT##群ID##Time##Date##message
                //BackWord返回格式：
                //GPTEXT## +
                //      DONE  存在该群且消息发出（可能会因群成员不在线而转入undo数据库）
                //      ERR##NOGP   代表没有这个群    *未实现?
                //
        //message(QString u1,QString u2, QString word, QTime t,QDate d, int j, QString t1)
         //:user1(u1),user2(u2),text(word),time(t),judge(j),date(d),type(t1){};
        qDebug() << str;
        QString sendId = str.section("##",1,1);
        QString TOId = str.section("##",2,2);
        QString word = str.section("##",5,5);
        QTime t = QTime::fromString(str.section("##",4,4),"hh:mm:ss.zzz");
        QDate date = QDate::fromString(str.section("##",3,3),"yyyy.MM.dd");

        //存入数据库
                QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
                //设置数据库
                db.setDatabaseName("../message.db");
                //打开数据库
                bool isOk = db.open();
                if(false == isOk)
                {
                    QMessageBox::warning(this,"数据库打开错误",db.lastError().text());
                    return;
                }
                                  QSqlQuery add;
                                  add.prepare("INSERT INTO friendMessage(sender, receiver, date, time, msg, type_msg)"
                                                        "VALUES(?, ?, ?, ?, ?, ?);");
                                  add.addBindValue(str.section("##",1,1));
                                  add.addBindValue(str.section("##",2,2));
                                  add.addBindValue(str.section("##",3,3));
                                  add.addBindValue(str.section("##",4,4));
                                  add.addBindValue(str.section("##",5,5));
                                  add.addBindValue(str.section("##",0,0));

                                  add.exec();
    });


//    if (connection("../message.db"))
//       {
//           //先建立一个表，然后根据传送来的信息判断是群组消息还是个人消息。删除还是添加
//           QSqlQuery createTable("CREATE TABLE friendMessage(sender VARCHAR(255),receiver VARCHAR(255),"
//               "msg VARCHAR(255),time VARCHAR(255),date VARCHAR(255), type_msg VARCHAR(255),"
//               "PRIMARY KEY(sender, receiver, time, date));");
//           debug(createTable);//执行加在控制台反馈执行情况

//           //传输过程中会存在类型的转化，注意一下(QTime->QString)
//           QString convertedTime = msg1.time.toString();//格式问题
//           QString convertedDate = msg1.date.toString();
//           if (msg1.judge)//删除消息
//           {
//               QSqlQuery deleteMsg;

//               QString sql = QString("DELETE FROM friendMessage WHERE "
//                   "sender = '%1' AND receiver = '%2' AND time = '%3' AND date = '%4';").arg(msg1.user1).arg(msg1.user2).arg(convertedTime)
//                   .arg(convertedDate);
//               deleteMsg.exec(sql);
//           }
//           else
//           {
//               QSqlQuery add;
//               QString sql = QString("INSERT INTO friendMessage(sender, receiver, msg, time, date, type_msg)"
//                   "VALUES('%1', '%2', '%3', '%4', '%5', '%6');").arg(msg1.user1).arg(msg1.user2).arg(msg1.text)
//                   .arg(convertedTime).arg(convertedDate).arg(msg1.type);
//               add.exec(sql);
//           }
//       }
}

messageControl::~messageControl()
{
    delete ui;
}

