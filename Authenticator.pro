#-------------------------------------------------
#
# Project created by QtCreator 2018-03-21T07:08:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Authenticator
TEMPLATE = app

DEFINES += QT_LOGGING_RULES="*.debug=true"

SOURCES += main.cpp\
        mainwindow.cpp \
    logindialog.cpp \
    authentication.cpp \
    user_management.cpp \
    user_add_update.cpp

HEADERS  += mainwindow.h \
    logindialog.h \
    authentication.h \
    user_management.h \
    user_add_update.h

FORMS    += mainwindow.ui \
    logindialog.ui \
    user_management.ui \
    user_add_update.ui
