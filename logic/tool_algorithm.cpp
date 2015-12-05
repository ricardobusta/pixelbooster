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

#include "tool_algorithm.h"

#include "widgets/image_edit_widget.h"

#include <QPainter>

void ToolAlgorithm::Pencil(QImage *image, const ACTION_TOOL action, const QPoint &p1, const QPoint p2, const QColor &color) {
  QPainter painter(image);
  painter.setPen(color);
  painter.drawLine(p1,p2);
}

void ToolAlgorithm::FloodFill(QImage * image, const ACTION_TOOL action, const QPoint &seed, const QColor &color) {
  if(action == ACTION_PRESS){
    QRgb new_color = color.rgba();
    QRgb old_color = image->pixel(seed);
    if(new_color == old_color){
      return;
    }
    QList<QPoint> to_do_list = {seed};

    QVector<QPoint> expansion = {
      QPoint(1,0),
      QPoint(0,1),
      QPoint(-1,0),
      QPoint(0,-1)
    };

    image->setPixel(seed,new_color);

    while(!to_do_list.isEmpty()){
      //DEBUG_MSG(to_do_list.length());
      QPoint target = to_do_list.takeFirst();

      for(QPoint e : expansion){
        QPoint new_target = target+e;
        //DEBUG_MSG(new_target);
        if(image->rect().contains(new_target) && image->pixel(new_target) == old_color){
          to_do_list.push_back(new_target);
          image->setPixel(new_target,new_color);
        }
      }
    }
  }
}

void ToolAlgorithm::BresenhamLine(QImage *image, const QPoint &p1, const QPoint &p2, const QRgb &color) {
  // Algorithm taken from http://www.roguebasin.com/index.php?title=Bresenham%27s_Line_Algorithm

  int x1 = p1.x();
  int y1 = p1.y();
  const int x2 = p2.x();
  const int y2 = p2.y();

  int delta_x(x2 - x1);
  signed char const ix((delta_x > 0) - (delta_x < 0));
  delta_x = std::abs(delta_x) << 1;

  int delta_y(y2 - y1);
  signed char const iy((delta_y > 0) - (delta_y < 0));
  delta_y = std::abs(delta_y) << 1;

  image->setPixel(x1,y1,color);

  if (delta_x >= delta_y) {
    int error(delta_y - (delta_x >> 1));
    while (x1 != x2) {
      if ((error >= 0) && (error || (ix > 0))) {
        error -= delta_x;
        y1 += iy;
      }
      error += delta_y;
      x1 += ix;

      image->setPixel(x1,y1,color);
    }
  } else {
    int error(delta_x - (delta_y >> 1));
    while (y1 != y2) {
      if ((error >= 0) && (error || (iy > 0))) {
        error -= delta_y;
        x1 += ix;
      }
      error += delta_x;
      y1 += iy;

      image->setPixel(x1,y1,color);
    }
  }
}
