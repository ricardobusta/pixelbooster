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

#include "zoom_tool.h"

#include "utils/debug.h"
#include "pb_math.h"

#include <QScrollArea>

void ZoomTool::Use(QRect *selection, QPoint *anchor, bool *started, const QScrollArea *scroll_area, const ToolEvent &event) {
  if (event.action() == ACTION_PRESS) {
    if (event.lmb_down()) {
      *anchor = event.img_pos();
      *started = true;
      *selection = GetRect(*anchor, event.img_pos());
    } else {
      *selection = QRect();
    }
  } else if (event.action() == ACTION_MOVE) {
    if (*started) {
      *selection = GetRect(*anchor, event.img_pos());
    }
  } else if (event.action() == ACTION_RELEASE) {
    *started = false;
    if (selection->isValid() && selection->width()>1 && selection->height()>1) {
      int zoom_w =   scroll_area->rect().width() / selection->width();
      int zoom_h =   scroll_area->rect().height() / selection->height();
      DEBUG_MSG(zoom_w << zoom_h);
      DEBUG_MSG(  scroll_area->rect() << *selection);
      pApp->main_window()->action_handler()->Zoom(qMin(zoom_w, zoom_h));
      //*selection = QRect();
    }else{
      if(event.lmb_down()){
        pApp->main_window()->action_handler()->Zoom(pApp->options()->zoom()+1);
      }else if(event.rmb_down()){
        pApp->main_window()->action_handler()->Zoom(pApp->options()->zoom()-1);
      }
    }
  }
}

QRect ZoomTool::GetRect(const QPoint &start, const QPoint &end) {
  return QRect(
      qMin(start.x(), end.x()),
      qMin(start.y(), end.y()),
      qAbs(start.x() - end.x()) + 1,
      qAbs(start.y() - end.y()) + 1);
}
