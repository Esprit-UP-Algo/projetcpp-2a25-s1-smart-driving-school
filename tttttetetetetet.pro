QT += core gui widgets serialport network sql printsupport charts multimedia

# Make sure we see ALL compilation errors
CONFIG += warn_on

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    arduinovh.cpp \
    connection.cpp \
    emailsender.cpp \
    mainV.cpp \
    mainwindowV.cpp \
    vehicule.cpp \
    voicerecognition.cpp

HEADERS += \
    arduinovh.h \
    connection.h \
    emailsender.h \
    mainwindowV.h \
    vehicule.h \
    voicerecognition.h

FORMS += \
    mainwindowV.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
