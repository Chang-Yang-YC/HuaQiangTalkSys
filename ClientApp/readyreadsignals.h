#ifndef READYREADSIGNALS_H
#define READYREADSIGNALS_H

#include <QWidget>
#include "TcpClientTool.h"
class readyreadSignals : public QWidget
{
    Q_OBJECT
public:
    explicit readyreadSignals(QWidget *parent = nullptr);
    static readyreadSignals *readyread;
    static readyreadSignals *getins();
public slots:
    void on_received_selected(QString str);

signals:
    void on_LOG_received(QString str);
    void on_ERR_received(QString str);
    void on_FilE_received(QString str);
    void on_ADDFRIEND_received(QString str);
    void on_ADD_received(QString str);
    void on_ADDGP_received(QString str);
    void on_STEXT_received(QString str);
    void on_GPTEXT_received(QString str);
private:
    TcpClientTool *tcpClient;
    QStringList ServList;

};

#endif // READYREADSIGNALS_H
