#-------------------------------------------------
#
# Project created by QtCreator 2014-07-30T10:30:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestProj
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

CONFIG += mobility
MOBILITY = 

QMAKE_CXXFLAGS += -std=c++11
