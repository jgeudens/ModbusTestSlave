#-------------------------------------------------
#
# Project created by QtCreator 2014-09-11T21:45:28
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = ModbusTestSlave
CONFIG   += console
CONFIG   -= app_bundle

win32 {
LIBS += -lws2_32
#LIBS += -lkernel32
}

TEMPLATE = app

DEFINES += QT_DEBUG_OUTPUT

INCLUDEPATH += \
    ../libraries/libmodbus/src
    ../src \

SOURCES += \
    ../src/main.cpp \
    ../src/mainApp.cpp \
    ../src/modbusSlaveTCP.cpp \
    ../libraries/libmodbus/src/modbus-data.c \
    ../libraries/libmodbus/src/modbus.c \
    ../libraries/libmodbus/src/modbus-tcp.c \
    ../src/workerThread.cpp \
    ../src/pollQuit.cpp

HEADERS += \
    ../src/mainApp.h \
    ../src/modbusSlaveTCP.h \
    ../libraries/libmodbus/src/modbus-private.h \
    ../libraries/libmodbus/src/modbus-version.h \
    ../libraries/libmodbus/src/modbus.h \
    ../libraries/libmodbus/src/modbus-tcp.h \
    ../libraries/libmodbus/src/modbus-tcp-private.h \
    ../libraries/libmodbus/src/config.h \
    ../src/workerThread.h \
    ../src/pollQuit.h
