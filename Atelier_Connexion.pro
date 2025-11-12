QT += core gui sql charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Atelier_Connexion
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += c++11

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    connection.cpp \
    moniteur.cpp

HEADERS += \
    mainwindow.h \
    connection.h \
    moniteur.h

FORMS += \
    mainwindow.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
