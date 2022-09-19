#ifndef SETGROUPWIDGET_H
#define SETGROUPWIDGET_H

#include <QWidget>
#include <QMap>

namespace Ui {
class SetGroupWidget;
}

class SetGroupWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SetGroupWidget(QWidget *parent = nullptr);
    ~SetGroupWidget();
    void setData();
    void setUserInfo(int);
private slots:
    void on_pushButtonSure_clicked();

    void on_pushButtonCancel_clicked();
signals:
    void signalSetGroup(int);
private:
    Ui::SetGroupWidget *ui;
    QMap<int, QString> idNameMap;
    QMap<int, int> idIndexMap;
    int userId;
    QString userStr;
    QString userName;
};

#endif // SETGROUPWIDGET_H
