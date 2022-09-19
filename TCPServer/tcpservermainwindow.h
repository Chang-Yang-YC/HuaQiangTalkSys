#ifndef TCPSERVERMAINWINDOW_H
#define TCPSERVERMAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QSqlDatabase>
#include <QTcpServer>
#include <QTcpSocket>


#include "userdbdialog.h"
#include "groupdbdialog.h"
#include "undodbdialog.h"
#include "onlinesocketlist.h"

//static OnlineSocketList *nowOnline;

QT_BEGIN_NAMESPACE
namespace Ui { class TcpServerMainWindow; }
QT_END_NAMESPACE

class TcpServerMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    TcpServerMainWindow(QWidget *parent = nullptr);
    ~TcpServerMainWindow();

    UserdbDialog *UserListDialog;
    GroupdbDialog *GroupListDialog;
    UndodbDialog *UndoListDialog;


    QTcpServer *m_Server;


private slots:
    void on_actionshowUserList_triggered();

    void on_actionshowGroupList_triggered();

    void on_actionshowUndoList_triggered();

    void on_actionshowServiceList_triggered();

private:
    Ui::TcpServerMainWindow *ui;
};
#endif // TCPSERVERMAINWINDOW_H
