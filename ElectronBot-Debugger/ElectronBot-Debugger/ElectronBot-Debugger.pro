
QT       += core gui
QT       += printsupport
QT       += network

VERSION = 1.1.2.0
QT += serialport
//QT       += core gui serialport
//QT += QDateTime
//QT += QTime
//CONFIG += c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ElectronBot-Debugger
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DEFINES += APP_VERSION=\\\"$$VERSION\\\"

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    protocol.cpp \
    usb_user.cpp \
    crcLib.c

HEADERS += \
        mainwindow.h \
    protocol.h \
    usb_device.h \
    usb_user.h \
    crcLib.h

FORMS += \
        mainwindow.ui

#win32:CONFIG(debug, debug|release):
LIBS += -L$$PWD/lib/ -lUSB2XXX

RC_ICONS=serialPort.ico

#win32:CONFIG(release, debug|release):
LIBS += -L$$PWD/libusb-win32-bin-1.2.6.0/lib/msvc/ -llibusb
