#ifndef SINGLECHATGROUPITEM_H
#define SINGLECHATGROUPITEM_H

#include <QWidget>
#include <QLabel>

class SingleChatGroupItem : public QWidget
{
    Q_OBJECT
public:
    explicit SingleChatGroupItem(QWidget *parent = nullptr);
    void initialUi();
    void setData(QString name);
    QLabel *avatar;
    QLabel *name;
    QLabel *sign;

};

#endif // SINGLECHATGROUPITEM_H
