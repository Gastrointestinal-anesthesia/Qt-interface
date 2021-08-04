#-------------------------------------------------
#
# Project created by QtCreator 2021-06-28T22:44:52
#
#-------------------------------------------------

QT       += core gui charts sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = SmartAn
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


#LIBS += /opt/ros/kinetic/lib

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        messagethread.cpp \
        qcustomplot.cpp

HEADERS += \
        anesthesia.h \
        connection.h \
        mainwindow.h \
        messagethread.hpp \
        qcustomplot.h

FORMS += \
        mainwindow.ui

RESOURCES += \
    icon.qrc

DISTFILES += \
    ../../test.jpg \
    config.ini \
    screen.jpg
