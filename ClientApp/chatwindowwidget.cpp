#include "chatwindowwidget.h"
#include "ui_chatwindowwidget.h"

chatWindowWidget::chatWindowWidget(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::chatWindowWidget)
{
    ui->setupUi(this);
}

chatWindowWidget::~chatWindowWidget()
{
    delete ui;

    QIcon iconSmile; //新建QIcon对象
        iconSmile.addFile(tr(":/image/Luffy.png")); //让QIcon对象指向想要的图标
        ui->pushButtonsmile->setIcon(iconSmile); //给按钮添加图标
        ui->pushButtonsmile->setIconSize(QSize(90,90));//重置图标大小
        ui->pushButtonsmile->setStyleSheet("background-color:rgba(0,0,0,0)");
        ui->pushButtonsmile->setFocusPolicy(Qt::NoFocus);
}
