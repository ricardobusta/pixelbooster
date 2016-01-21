/***************************************************************************\
*  Pixel::Booster, a simple pixel art image editor.                         *
*  Copyright (C) 2015  Ricardo Bustamante de Queiroz (ricardo@busta.com.br) *
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

#include "rectangle_tool.h"

#include <QPainter>

void RectangleTool::Use(QImage * image, QImage * overlay, const QColor &main_color, const QColor &alt_color, QPoint * anchor, bool * started,const ToolEvent &event) {
  if (event.action() == ACTION_PRESS) {
    if (event.lmb_down()) {
      *anchor = event.img_pos();
      *started = true;
    } else if (event.rmb_down()) {
      pApp->main_window()->action_handler()->SetMainColor(image->pixel(event.img_pos()));
    }
  } else if (event.action() == ACTION_MOVE) {
    if (*started && event.img_pos() != *anchor) {
      overlay->fill(0x0);
      QRect rect = QRect(
            qMin(anchor->x(),event.img_pos().x()),
            qMin(anchor->y(),event.img_pos().y()),
            qAbs(anchor->x()-event.img_pos().x()),
            qAbs(anchor->y()-event.img_pos().y())
            );
      QPainter painter(overlay);
      painter.setPen(main_color);
      painter.setBrush(alt_color);
      painter.drawRect(rect);
    }else{
      if (*started){
        overlay->fill(0x0);
        overlay->setPixel(event.img_pos(),main_color.rgba());
      }
    }
  } else if (event.action() == ACTION_RELEASE) {
    QPainter apply(image);
    apply.drawImage(image->rect(), *overlay);
    overlay->fill(0x0);
    *started = false;
  }
}
