/***************************************************************************\
*  Pixel::Booster, a simple pixel art image editor.                         *
*  Copyright (C) 2015  Ricardo Bustamante de Queiroz (ricardo@busta.com.br) *
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

#include "pb_math.h"

QColor ColorLerp(QColor &c1, QColor &c2, float t) {
  QColor out;
  out.setRedF(clamp<float>(c1.redF() * (1 - t) + c2.redF() * t, 0.0f, 1.0f));
  out.setGreenF(
      clamp<float>(c1.greenF() * (1 - t) + c2.greenF() * t, 0.0f, 1.0f));
  out.setBlueF(clamp<float>(c1.blueF() * (1 - t) + c2.blueF() * t, 0.0f, 1.0f));
  out.setAlphaF(
      clamp<float>(c1.alphaF() * (1 - t) + c2.alphaF() * t, 0.0f, 1.0f));
  return out;
}
