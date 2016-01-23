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

#ifndef IMAGE_EDIT_WIDGET_H
#define IMAGE_EDIT_WIDGET_H

#include <QImage>
#include <QWidget>

#include "logic/tool_algorithm.h"
#include "logic/undo_redo.h"

class GlobalOptions;
class QScrollArea;

/*!
 * \brief The ImageEditWidget class
 */
class ImageEditWidget : public QWidget {
  Q_OBJECT
public:
  explicit ImageEditWidget(QWidget *parent = 0);

  void Clear(const QSize &size);

  void Undo();
  void Redo();

  void set_scroll_area(QScrollArea *scroll_area);

  void ClearSelection();

protected:
  virtual void paintEvent(QPaintEvent *);
  virtual void mouseMoveEvent(QMouseEvent *event);
  virtual void leaveEvent(QEvent *);
  virtual void mousePressEvent(QMouseEvent *event);
  virtual void mouseReleaseEvent(QMouseEvent *event);
  virtual void mouseClickEvent(QMouseEvent *event);

private:
  QImage image_;
  QRect cursor_;

  QImage image_selection_;

  UndoRedo undo_redo_;

  bool press_right_inside_;
  bool press_left_inside_;

  bool left_button_down_;
  bool right_button_down_;

  GlobalOptions *options_cache_;

  QPoint previous_pos_;
  QPoint action_anchor_;
  bool action_started_;

  QRect selection_;
  QRect zoom_area_;

  QImage overlay_image_;

  void ToolAction(const QMouseEvent *event, ACTION_TOOL action);

  QRect SelectionRect(const QRect &rect);
  QPoint WidgetToImageSpace(const QPoint &pos);

  QScrollArea *scroll_area_;
signals:
  void SendImage(QImage *);
public slots:
  void GetImage(QImage *image);
  void HandleRequest();
  void UpdateWidget();
};

#endif // IMAGE_EDIT_WIDGET_H
