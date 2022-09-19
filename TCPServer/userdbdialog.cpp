#include "userdbdialog.h"
#include "ui_userdbdialog.h"

#include <QDebug>
#include <QSqlTableModel>
#include <QSqlQuery>

UserdbDialog::UserdbDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserdbDialog)
{
    ui->setupUi(this);

    this->setWindowTitle("用户数据库");
    Serverdb = QSqlDatabase::addDatabase("QSQLITE");
    Serverdb.setDatabaseName("../MultiUserTcpServer/Server.db");
    bool isopen = Serverdb.open();
    if(false == isopen){
        qDebug() << "数据库打开失败\n";
    }
    else{
        qDebug() << "用户数据库已连接\n";
        QSqlQuery query;
        query.exec("create table User (id int primary key, ip varchar(255), port int, password varchar(255), name varchar(255), pic int);");
    }
    QSqlTableModel* model = new QSqlTableModel(NULL,Serverdb);
    model->setTable("User");
    model->select();
    ui->tableView->setModel(model);


}

UserdbDialog::~UserdbDialog()
{
    delete ui;
}
