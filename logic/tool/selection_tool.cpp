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

#include "selection_tool.h"

void SelectionTool::Use(QRect * selection, QPoint * anchor, bool * started,const ToolEvent &event) {
  if (event.action() == ACTION_PRESS) {
    if (event.lmb_down()) {
      *anchor = event.img_pos();
      *started = true;
      *selection = GetRect(*anchor,event.img_pos());
    }else{
      *selection = QRect();
    }
  } else if (event.action() == ACTION_MOVE) {
    if (*started) {
       *selection = GetRect(*anchor,event.img_pos());
    }
  } else if (event.action() == ACTION_RELEASE) {
    *started = false;
  }
}

QRect SelectionTool::GetRect(const QPoint &start, const QPoint &end)
{
  return QRect(
        qMin(start.x(),end.x()),
        qMin(start.y(),end.y()),
        qAbs(start.x()-end.x())+1,
        qAbs(start.y()-end.y())+1
        );
}
