#include "loginwidget.h"
#include "chatwidget.h"
#include <QApplication>
#include "receivefilewidget.h"
#include "messagecontrol.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    loginWidget w;
    receiveFileWidget receiveFileW;
    messageControl mw;
    w.show();
    return a.exec();
}
