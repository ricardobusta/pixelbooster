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

#ifndef TOOLALGORITHM_H
#define TOOLALGORITHM_H

#include <QColor>
#include <QImage>
#include <QPoint>

enum ACTION_TOOL : int {
  ACTION_PRESS,
  ACTION_RELEASE,
  ACTION_MOVE,
  ACTION_CLICK
};

class ToolAlgorithm {
public:
  static void Pencil(QImage *image, const QPoint &p1, const QPoint p2, const QColor &color);
  static void FloodFill(QImage *image, const ACTION_TOOL action, const QPoint &seed, const QColor &color);
  //private:
  static void BresenhamLine(QImage *image, const QPoint &p1, const QPoint &p2, const QRgb &color);
  static void BresenhamEllipse(QImage *image, const QRect &rect, bool fill, const QRgb &color);

private:
  static void Plot4EllipsePoints(QImage *image, const QPoint &c, const QPoint &p, const QPoint &e, const QRgb &color);

  static void SetPixel(QImage *image, const QPoint &p, const QRgb &color);
  static void SetPixel(QImage *image, const int x, const int y, const QRgb &color);
  ToolAlgorithm();
};

#endif // TOOLALGORITHM_H
