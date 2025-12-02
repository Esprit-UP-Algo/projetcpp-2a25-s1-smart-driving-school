QT       += core gui
QT += sql
QT += core gui widgets sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    exam.cpp \
    logindialog.cpp \
    main.cpp \
    mainwindow.cpp \
    connexion.cpp \
    statschartwidget.cpp

HEADERS += \
    exam.h \
    logindialog.h \
    mainwindow.h \
    connexion.h \
    role.h \
    statschartwidget.h

FORMS += \
    logindialog.ui \
    mainwindow.ui \
    connexion.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
