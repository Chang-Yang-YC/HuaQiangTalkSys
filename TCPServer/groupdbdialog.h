#ifndef GROUPDBDIALOG_H
#define GROUPDBDIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>

namespace Ui {
class GroupdbDialog;
}

class GroupdbDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GroupdbDialog(QWidget *parent = nullptr);
    ~GroupdbDialog();
    QSqlDatabase Serverdb;

private:
    Ui::GroupdbDialog *ui;
};

#endif // GROUPDBDIALOG_H
