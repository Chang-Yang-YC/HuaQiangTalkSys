#include "undodbdialog.h"
#include "ui_undodbdialog.h"

#include <QDebug>
#include <QSqlTableModel>
#include <QSqlQuery>
#include<QSqlError>


UndodbDialog::UndodbDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UndodbDialog)
{
    ui->setupUi(this);

    this->setWindowTitle("未完成事务数据库");
    Serverdb = QSqlDatabase::addDatabase("QSQLITE");
    Serverdb.setDatabaseName("../MultiUserTcpServer/Server.db");
    bool isopen = Serverdb.open();
    if(false == isopen){
        qDebug() << "数据库打开失败\n";
    }
    else{
        qDebug() << "未完成事务数据库已连接\n";
        QSqlQuery query;
        query.exec("create table Undo ("
                   "sender_id int, "
                   "receive_id int, "
                   "Date varchar(255), "
                   "Time varchar(255), "
                   "text varchar(500),"
                   "primary key(sender_id, receive_id, Date, Time));");
        qDebug()<<query.lastError();
    }


    QSqlTableModel *model = new QSqlTableModel(NULL,Serverdb);
    model->setTable("Undo");
    model->select();
    ui->tableView->setModel(model);


}

UndodbDialog::~UndodbDialog()
{
    delete ui;
}
