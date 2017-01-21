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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../UtilsLib/release/ -lUtilsLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../UtilsLib/debug/ -lUtilsLib
else:unix: LIBS += -L$$OUT_PWD/../UtilsLib/ -lUtilsLib

INCLUDEPATH += $$PWD/../UtilsLib
DEPENDPATH += $$PWD/../UtilsLib

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../UtilsLib/release/libUtilsLib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../UtilsLib/debug/libUtilsLib.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../UtilsLib/release/UtilsLib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../UtilsLib/debug/UtilsLib.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../UtilsLib/libUtilsLib.a
