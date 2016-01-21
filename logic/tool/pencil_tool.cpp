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

#include "pencil_tool.h"

#include "logic/tool_algorithm.h"

#include <QStatusBar>
#include <QPainter>

void PencilTool::Use(QImage *image, const QColor &color, const ToolEvent &event) {
  if (event.action() == ACTION_PRESS || event.action() == ACTION_MOVE) {
    if (event.lmb_down()) {
      Algorithm(image, event.img_prev_pos(), event.img_pos(), color);
      pApp->main_window()->statusBar()->showMessage("Teste",0);
    } else if (event.rmb_down()) {
      pApp->main_window()->action_handler()->SetMainColor(image->pixel(event.img_pos()));
    }
  }
}

void PencilTool::Algorithm(QImage *image, const QPoint &p1, const QPoint p2, const QColor &color) {
  ToolAlgorithm::BresenhamLine(image,p1,p2,color.rgba());
}
