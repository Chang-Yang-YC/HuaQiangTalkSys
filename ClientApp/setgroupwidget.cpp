#include "setgroupwidget.h"
#include "ui_setgroupwidget.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QtDebug>

SetGroupWidget::SetGroupWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetGroupWidget)
{
    ui->setupUi(this);
    //初始化userInfo
    userId = 0;
    userName = "";
    userStr = "";
}

SetGroupWidget::~SetGroupWidget()
{
    delete ui;
}

void SetGroupWidget::setData()
{
    //重置ComboBox
    ui->comboBox->clear();
    //读取组信息列表
    //添加一个数据库
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    //设置数据库
    db.setDatabaseName("../friendMessage.db");
    //打开数据库
    bool isOk = db.open();
   if(false == isOk)
    {
        QMessageBox::warning(this,"数据库打开错误",db.lastError().text());
        return;
    }
    //先加入未命名分组的选项
    idNameMap.insert(0,"未命名用户");
    idIndexMap.insert(0,0);
    ui->comboBox->addItem("未命名用户");
    //读取数据
    QSqlQuery query;
    query.exec(QString("Select * From %1GroupInfo;").arg(userStr));
    while(true == query.next())
    {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        idNameMap.insert(id,name);
        ui->comboBox->addItem(name);
        int index = ui->comboBox->count()-1;
        idIndexMap.insert(id,index);
    }
    //默认显示
    ui->comboBox->setCurrentIndex(0);
}

void SetGroupWidget::setUserInfo(int id)
{
    userId = id;
    userName = QString("用户%1").arg(id);
    userStr = QString("User%1").arg(id);
    setData();
}

void SetGroupWidget::on_pushButtonSure_clicked()
{
    int id=0;
    int index = ui->comboBox->currentIndex();
    id = idIndexMap.key(index);
    //qDebug() <<id;
    emit signalSetGroup(id);
    //qDebug() <<"发送信号";
    this->hide();
}

void SetGroupWidget::on_pushButtonCancel_clicked()
{
    this->hide();
}
