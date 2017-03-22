#-------------------------------------------------
#
# Project created by QtCreator 2016-09-22T10:51:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = server
TEMPLATE = app


SOURCES += main.cpp\
        serverstream.cpp

HEADERS  += serverstream.h
QT +=network

RESOURCES += \
    images.qrc
