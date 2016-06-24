#---------------------------------------------------------------------------#
#  Pixel::Booster, a simple pixel art image editor.                         #
#  Copyright (C) 2015  Ricardo Bustamante de Queiroz (ricardo@busta.com.br) #
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
#---------------------------------------------------------------------------#

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

include(ViewSources.pri)

FORMS    += \
    screens/new_image_file_dialog.ui \
    widgets/image_canvas_container.ui \
    screens/about_dialog.ui \
    screens/set_tile_size_dialog.ui \
    screens/main_window.ui \
    widgets/color_dialog.ui \
    screens/resize_image_dialog.ui \
    screens/help_dialog.ui

RESOURCES += \
    resources/icons/icons.qrc \
    resources/images/images.qrc \
    resources/translations/translations.qrc \
    resources/cursors/cursors.qrc \
    resources/help/help.qrc

# To update .ts files with the most recent translations run lupdate
# To generate the .qm files that will be needed by the application run lrelease
TRANSLATIONS += \
    resources/translations/pixel_booster_pt_br.ts \
    resources/translations/pixel_booster_en_us.ts \

#DISTFILES += \
#    ViewSources.pri

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
