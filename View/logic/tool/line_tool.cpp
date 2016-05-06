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

#include "line_tool.h"

#include <QPainter>

#include "logic/tool_algorithm.h"
#include "logic/undo_redo.h"

void LineTool::Use(QImage *image, QImage *overlay, const QColor &color, QPoint *anchor, bool *started, const ToolEvent &event) {
  if (event.action() == ACTION_PRESS) {
    if (event.lmb_down()) {
      event.undo_redo()->Do(*image);
      *anchor = event.img_pos();
      *started = true;
      overlay->fill(0x0);
      ToolAlgorithm::BresenhamLine(overlay, *anchor, event.img_pos(), color.rgba());
    } else if (event.rmb_down()) {
      pApp->main_window()->action_handler()->SetMainColor(image->pixel(event.img_pos()));
    }
  } else if (event.action() == ACTION_MOVE) {
    if (*started) {
      overlay->fill(0x0);
      ToolAlgorithm::BresenhamLine(overlay, *anchor, event.img_pos(), color.rgba());
    }
  } else if (event.action() == ACTION_RELEASE) {
    QPainter apply(image);
    apply.drawImage(image->rect(), *overlay);
    overlay->fill(0x0);
    *started = false;
  }
}
