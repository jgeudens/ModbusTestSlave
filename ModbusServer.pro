
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
    src/registerstatemodel.cpp

HEADERS += \
    src/mainwindow.h \
    testslave/testslavemodbus.h \
    testslave/testslavedata.h \
    src/registerstatemodel.h

FORMS += \
    src/mainwindow.ui
