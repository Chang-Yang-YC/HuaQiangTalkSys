#ifndef REGISTERWIDGET_H
#define REGISTERWIDGET_H

#include <QWidget>
#include "TcpClientTool.h"
#include"readyreadsignals.h"
namespace Ui {
class registerWidget;
}

class registerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit registerWidget(QWidget *parent = nullptr);
    ~registerWidget();
private slots:
    void on_pushButton_clicked();

signals:
    void signal_ToLogin();

private:
    Ui::registerWidget *ui;
    TcpClientTool *tcpClient;
   readyreadSignals *readyread;
};

#endif // REGISTERWIDGET_H
