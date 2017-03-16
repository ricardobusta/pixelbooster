#***************************************************************************#
#  Pixel::Booster, a simple pixel art image editor.                         #
#  Copyright (C) 2017  Ricardo Bustamante de Queiroz (ricardo@busta.com.br) #
#  Visit the Official Homepage: pixel.busta.com.br                          #
#                                                                           #
#  This program is free software: you can redistribute it and/or modify     #
#  it under the terms of the GNU General Public License as published by     #
#  the Free Software Foundation, either version 3 of the License, or        #
#  (at your option) any later version.                                      #
#                                                                           #
#  This program is distributed in the hope that it will be useful,          #
#  but WITHOUT ANY WARRANTY; without even the implied warranty of           #
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            #
#  GNU General Public License for more details.                             #
#                                                                           #
#  You should have received a copy of the GNU General Public License        #
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.    #
#***************************************************************************#

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
    application.cpp \
    widgets/aboutdialog.cpp \
    widgets/editcanvaswidget.cpp \
    widgets/imagewidget.cpp \
    logic/imageproject.cpp \
    widgets/newimagedialog.cpp \
    widgets/imagewidgetcontainer.cpp

HEADERS  += mainwindow.h \
    application.h \
    widgets/aboutdialog.h \
    resources/constants.h \
    widgets/editcanvaswidget.h \
    widgets/imagewidget.h \
    logic/imageproject.h \
    widgets/newimagedialog.h \
    widgets/imagewidgetcontainer.h

FORMS    += mainwindow.ui \
    widgets/aboutdialog.ui \
    widgets/newimagedialog.ui \
    widgets/imagewidgetcontainer.ui

# To update .ts files with the most recent translations run lupdate
# To generate the .qm files that will be needed by the application run lrelease
TRANSLATIONS += \
    resources/translations/pixel_booster_pt_br.ts \
    resources/translations/pixel_booster_en_us.ts \
