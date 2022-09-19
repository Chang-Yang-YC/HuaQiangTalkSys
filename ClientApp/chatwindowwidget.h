#ifndef CHATWINDOWWIDGET_H
#define CHATWINDOWWIDGET_H

#include <QMainWindow>

namespace Ui {
class chatWindowWidget;
}

class chatWindowWidget : public QMainWindow
{
    Q_OBJECT

public:
    explicit chatWindowWidget(QWidget *parent = nullptr);
    ~chatWindowWidget();

private:
    Ui::chatWindowWidget *ui;
};

#endif // CHATWINDOWWIDGET_H
