
VERSION = v1.0.0

DEFINES += DEBUG

DEFINES += GIT_HASH="\\\"$(shell git --git-dir \""$$PWD/.git"\" rev-parse --short HEAD)\\\""
DEFINES += GIT_BRANCH="\\\"$(shell git --git-dir \""$$PWD/.git"\" rev-parse --abbrev-ref HEAD)\\\""

DEFINES += APP_VERSION=\\\"$$VERSION\\\"

QT       += core gui widgets serialbus

TARGET = ModbusTestSlave
TEMPLATE = app

CONFIG += c++11

INCLUDEPATH += \
    testslave

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    testslave/testslavemodbus.cpp \
    testslave/testslavedata.cpp \
    src/registerdatamodel.cpp

HEADERS += \
    src/mainwindow.h \
    testslave/testslavemodbus.h \
    testslave/testslavedata.h \
    src/registerdatamodel.h

FORMS += \
    src/mainwindow.ui
