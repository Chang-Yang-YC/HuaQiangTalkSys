#include "contactviewslider.h"
#include "ui_contactviewslider.h"

#include<QComboBox>

contactViewSlider::contactViewSlider(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::contactViewSlider)
{
    ui->setupUi(this);
    ui->comboBox->setWindowTitle("联系人");


}

contactViewSlider::~contactViewSlider()
{
    delete ui;
}
