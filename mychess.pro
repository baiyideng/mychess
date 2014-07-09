#-------------------------------------------------
#
# Project created by QtCreator 2014-07-07T15:47:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mychess
TEMPLATE = app


SOURCES += main.cpp \
    laws.cpp \
    chessboard.cpp

HEADERS  += \
    laws.h \
    chesspieces.h \
    chessboard.h
