#include "tcpservermainwindow.h"
#include "onlinesocketlist.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TcpServerMainWindow w;
    w.show();
    return a.exec();
}
