#ifndef CONTACTWIDGET_H
#define CONTACTWIDGET_H

#include <QWidget>
#include<QMenu>

namespace Ui {
class contactWidget;
}

class contactWidget : public QWidget
{
    Q_OBJECT

public:
    explicit contactWidget(QWidget *parent = nullptr);
    ~contactWidget();

private:
    Ui::contactWidget *ui;
    QMenu *menu;

};

#endif // CONTACTWIDGET_H
