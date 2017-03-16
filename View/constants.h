/***************************************************************************\
*  Pixel::Booster, a simple pixel art image editor.                         *
*  Copyright (C) 2017  Ricardo Bustamante de Queiroz (ricardo@busta.com.br) *
*  Visit the Official Homepage: pixel.busta.com.br                          *
*                                                                           *
*  This program is free software: you can redistribute it and/or modify     *
*  it under the terms of the GNU General Public License as published by     *
*  the Free Software Foundation, either version 3 of the License, or        *
*  (at your option) any later version.                                      *
*                                                                           *
*  This program is distributed in the hope that it will be useful,          *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
*  GNU General Public License for more details.                             *
*                                                                           *
*  You should have received a copy of the GNU General Public License        *
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.    *
\***************************************************************************/

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>
#include <QRect>

const QString kVersionString = "2.0";

const QString kConfigFileName = "config.ini";
const QString kConfigGroupWindow = "window";
const QString kConfigWindowGeometry = "geometry";
const QRect kConfigWindowGeometryDefault = QRect(50, 50, 800, 600);
const QString kConfigGroupState = "state";
const QString kConfigWindowMaximized = "maximized";
const bool kConfigWindowMaximizedDefault = false;
const QString kConfigWindowState = "window state";

#endif // CONSTANTS_H
