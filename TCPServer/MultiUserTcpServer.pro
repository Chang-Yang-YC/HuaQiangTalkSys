QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    connectclient.cpp \
    groupdbdialog.cpp \
    main.cpp \
    onlinesocketlist.cpp \
    onlinesocketnode.cpp \
    tcpservermainwindow.cpp \
    undodbdialog.cpp \
    userdbdialog.cpp \


HEADERS += \
    DEBUG.h \
    connectclient.h \
    connectionFun.h \
    groupdbdialog.h \
    onlinesocketlist.h \
    onlinesocketnode.h \
    tcpservermainwindow.h \
    undodbdialog.h \
    userdbdialog.h \


FORMS += \
    groupdbdialog.ui \
    tcpservermainwindow.ui \
    undodbdialog.ui \
    userdbdialog.ui \


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
