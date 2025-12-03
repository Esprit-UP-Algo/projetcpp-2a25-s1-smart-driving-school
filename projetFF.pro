QT       += core gui sql widgets
QT       += printsupport charts network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# Disable deprecated APIs
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

SOURCES += \
    # Main file
    main.cpp \
    # Examens sources
    exam.cpp \
    logindialog.cpp \
    mainwindow.cpp \
    connexion.cpp \
    statschartwidget.cpp \
    # Vehicule sources (NEW)
    mainwindowV.cpp \
    vehicule.cpp \
    emailsender.cpp \
    voicerecognition.cpp \
    # Shared connection
    connection.cpp

HEADERS += \
    # Examens headers
    exam.h \
    logindialog.h \
    mainwindow.h \
    connexion.h \
    role.h \
    statschartwidget.h \
    # Vehicule headers (NEW)
    mainwindowV.h \
    vehicule.h \
    emailsender.h \
    voicerecognition.h \
    # Shared connection
    connection.h

FORMS += \
    # Examens forms
    logindialog.ui \
    mainwindow.ui \
    connexion.ui \
    # Vehicule forms (NEW)
    mainwindowV.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
