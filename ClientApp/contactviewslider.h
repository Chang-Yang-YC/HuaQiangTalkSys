#ifndef CONTACTVIEWSLIDER_H
#define CONTACTVIEWSLIDER_H

#include <QWidget>

namespace Ui {
class contactViewSlider;
}

class contactViewSlider : public QWidget
{
    Q_OBJECT

public:
    explicit contactViewSlider(QWidget *parent = nullptr);
    ~contactViewSlider();

private:
    Ui::contactViewSlider *ui;
};

#endif // CONTACTVIEWSLIDER_H
