#ifndef UNDODBDIALOG_H
#define UNDODBDIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>

namespace Ui {
class UndodbDialog;
}

class UndodbDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UndodbDialog(QWidget *parent = nullptr);
    ~UndodbDialog();
    QSqlDatabase Serverdb;

private:
    Ui::UndodbDialog *ui;


};

#endif // UNDODBDIALOG_H
