#ifndef SINGLEFRIENDITEM_H
#define SINGLEFRIENDITEM_H

#include <QWidget>
#include <QLabel>

class SingleFriendItem : public QWidget
{
    Q_OBJECT
public:
    explicit SingleFriendItem(QWidget *parent = nullptr);
    void initialUi();
    void setData(QString name, int avatar);
    QLabel *avatar;
    QLabel *name;
    QLabel *sign;
};

#endif // SINGLEFRIENDITEM_H
