QT       += core gui charts
QT += sql
QT += printsupport
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets charts

CONFIG += c++17

SOURCES += \
    Transaction.cpp \
    chat.cpp \
    main.cpp \
    mainwindow.cpp \
    connexion.cpp \
    qr/qrcodegen.cpp
    qr/qrcodegen.cpp
    chat.cpp

HEADERS += \
    Transaction.h \
    chat.h \
    mainwindow.h \
    connexion.h \
    qr/qrcodegen.hpp
    qr/qrcodegen.hpp
    chat.h

FORMS += \
    chat.ui \
    mainwindow.ui \
    connexion.ui \
    mainwindow.ui \
    connexion.ui
    chat.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
