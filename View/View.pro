#-------------------------------------------------
#
# Project created by QtCreator 2017-03-14T01:25:02
#
#-------------------------------------------------

lessThan(QT_MAJOR_VERSION,5){
  error(Must have at least Qt 5.5)
}

lessThan(QT_MINOR_VERSION,5){
  error(Must have at least Qt 5.5)
}

QT       += core gui widgets

RC_ICONS = icon.ico

TARGET = PixelBooster
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    application.cpp

HEADERS  += mainwindow.h \
    application.h

FORMS    += mainwindow.ui

# To update .ts files with the most recent translations run lupdate
# To generate the .qm files that will be needed by the application run lrelease
TRANSLATIONS += \
    resources/translations/pixel_booster_pt_br.ts \
    resources/translations/pixel_booster_en_us.ts \
