#ifndef USERDBDIALOG_H
#define USERDBDIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>

namespace Ui {
class UserdbDialog;
}

class UserdbDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserdbDialog(QWidget *parent = nullptr);
    ~UserdbDialog();

    QSqlDatabase Serverdb;

private:
    Ui::UserdbDialog *ui;
};

#endif // USERDBDIALOG_H
