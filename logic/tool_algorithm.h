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

#ifndef TOOLALGORITHM_H
#define TOOLALGORITHM_H

#include <QColor>
#include <QImage>
#include <QPoint>

#include "application/pixel_booster.h"
#include "logic/action_handler.h"
#include "screens/main_window.h"

class UndoRedo;

enum ACTION_TOOL : int {
  ACTION_PRESS,
  ACTION_RELEASE,
  ACTION_MOVE,
  ACTION_CLICK
};

class ToolEvent {
public:
  ToolEvent(ACTION_TOOL action,
            bool lmb_down,
            bool rmb_down,
            const QPoint &img_pos,
            const QPoint &img_prev_pos,
            UndoRedo *undo_redo) : action_(action),
                                   lmb_down_(lmb_down),
                                   rmb_down_(rmb_down),
                                   img_pos_(img_pos),
                                   img_prev_pos_(img_prev_pos),
                                   undo_redo_(undo_redo) {
  }
  ACTION_TOOL action() const { return action_; }
  bool lmb_down() const { return lmb_down_; }
  bool rmb_down() const { return rmb_down_; }
  QPoint img_pos() const { return img_pos_; }
  QPoint img_prev_pos() const { return img_prev_pos_; }
  UndoRedo *undo_redo() const { return undo_redo_; }

private:
  ACTION_TOOL action_;
  bool lmb_down_;
  bool rmb_down_;
  QPoint img_pos_;
  QPoint img_prev_pos_;
  UndoRedo *undo_redo_;
};

namespace ToolAlgorithm {
void FloodFill(QImage *image, const QPoint &seed, const QColor &color);
void BresenhamLine(QImage *image, const QPoint &p1, const QPoint &p2, const QRgb &color);
void BresenhamEllipse(QImage *image, const QRect &rect, bool fill, const QRgb &color);

void Plot4EllipsePoints(QImage *image, const QPoint &c, const QPoint &p, const QPoint &e, const QRgb &color);
void SetPixel(QImage *image, const QPoint &p, const QRgb &color);
void SetPixel(QImage *image, const int x, const int y, const QRgb &color);
}

#endif // TOOLALGORITHM_H
