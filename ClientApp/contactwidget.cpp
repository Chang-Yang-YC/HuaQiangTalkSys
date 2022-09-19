#include "contactwidget.h"
#include "ui_contactwidget.h"

#include<QPixmap>
#include<QToolButton>
#include<QMenu>
#include<QAction>

contactWidget::contactWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::contactWidget)
{
    ui->setupUi(this);
    //写联系人的Menu内容
    menu = new QMenu(ui->toolButtonContact);
    QAction *action1 = new QAction(menu);
    action1->setText("张三");
    menu->resize(200,20);
    menu->addAction(action1);
    ui->toolButtonContact->setMenu(menu);





}

contactWidget::~contactWidget()
{
    delete ui;
}
