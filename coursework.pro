#-------------------------------------------------
#
# Project created by QtCreator 2020-10-23T18:53:23
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = coursework
TEMPLATE = app
DESTDIR = D:\\Qt\\Projects\\coursework
win32: RC_ICONS += D:\\Qt\\Projects\\coursework\\appicon\\Icon.ico

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        action_window.cpp \
        auth_window.cpp \
        createdb_window.cpp \
        incryption.cpp \
        main.cpp \
        mainwindow.cpp \
        regi_window.cpp \
        sysfunction.cpp \
        type_window.cpp

HEADERS += \
        action_window.h \
        auth_window.h \
        constants.h \
        createdb_window.h \
        incryption.h \
        mainwindow.h \
        regi_window.h \
        type_window.h

FORMS += \
        action_window.ui \
        auth_window.ui \
        createdb_window.ui \
        mainwindow.ui \
        output_menu.ui \
        regi_window.ui \
        type_window.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    css_style.qrc
