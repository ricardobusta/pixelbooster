#-------------------------------------------------
#
# Project created by QtCreator 2016-06-24T12:43:28
#
#-------------------------------------------------

QT       += testlib

#QT       -= gui

TARGET = tst_clamptest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    tst_colorlerp.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

unix|win32: LIBS += -L$$OUT_PWD/../UtilsLib/ -lUtilsLib

INCLUDEPATH += $$PWD/../UtilsLib
DEPENDPATH += $$PWD/../UtilsLib

win32:!win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../UtilsLib/UtilsLib.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../UtilsLib/libUtilsLib.a
