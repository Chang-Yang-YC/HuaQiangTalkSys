#include "groupdbdialog.h"
#include "ui_groupdbdialog.h"

#include <QDebug>
#include <QSqlTableModel>
#include <QSqlQuery>

GroupdbDialog::GroupdbDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GroupdbDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("群组数据库");
    Serverdb = QSqlDatabase::addDatabase("QSQLITE");
    Serverdb.setDatabaseName("../MultiUserTcpServer/Server.db");
    bool isopen = Serverdb.open();
    if(false == isopen){
        qDebug() << "数据库打开失败\n";
    }
    else{
        qDebug() << "群组数据库已连接\n";
        QSqlQuery query;
        query.exec("create table Grou (id int primary key);");//初始时没有一个群组
    }

    QSqlQuery query("create table GrouID(id int primary key,name1 varchar(255));");
    query.exec();
    query.exec("create table GrouMsg(id int, mesg varchar(255), Time varchar(255), Date varchar(255),"
               "primary key(id, mesg, Time, Date));");



    QSqlTableModel *model = new QSqlTableModel(NULL,Serverdb);
    model->setTable("Grou");
    model->select();
    ui->tableView->setModel(model);

}

GroupdbDialog::~GroupdbDialog()
{
    delete ui;
}
