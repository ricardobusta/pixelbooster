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

#include "image_edit_widget.h"

#include <QClipboard>
#include <QMouseEvent>
#include <QPainter>
#include <QStatusbar>

#include "application/pixel_booster.h"
#include "logic/action_handler.h"
#include "logic/tool/ellipse_tool.h"
#include "logic/tool/flood_fill_tool.h"
#include "logic/tool/line_tool.h"
#include "logic/tool/pencil_tool.h"
#include "logic/tool/rectangle_tool.h"
#include "logic/tool/selection_tool.h"
#include "logic/tool/zoom_tool.h"
#include "screens/main_window.h"
#include "utils/debug.h"
#include "utils/pb_math.h"

ImageEditWidget::ImageEditWidget(QWidget *parent)
    : QWidget(parent),
      left_button_down_(false),
      right_button_down_(false),
      press_right_inside_(false),
      press_left_inside_(false),
      action_started_(false) {
  setMouseTracking(true);
  image_ = QImage(0, 0, QImage::Format_ARGB32_Premultiplied);
  overlay_image_ = QImage(image_.size(), image_.format());
  overlay_image_.fill(0x0);
  options_cache_ = pApp->options();
  this->setFixedSize(0, 0);

  this->setCursor(QCursor(QPixmap(":/pencil.png"), 0, 0));
}

void ImageEditWidget::Clear(const QSize &size) {
  image_ = QImage(size, QImage::Format_ARGB32_Premultiplied);
  overlay_image_ = QImage(image_.size(), image_.format());
  overlay_image_.fill(0x0);
  image_.fill(Qt::white);
  this->setFixedSize(image_.size());
  update();
}

void ImageEditWidget::Undo() {
  QImage img = undo_redo_.Undo(image_);
  if (!img.isNull()) {
    if (image_.size() != img.size()) {
      image_.scaled(img.size());
    }
    image_ = img;
    UpdateWidget();
  }
}

void ImageEditWidget::Redo() {
  QImage img = undo_redo_.Redo(image_);
  if (!img.isNull()) {
    image_ = img;
    UpdateWidget();
  }
}

void ImageEditWidget::set_scroll_area(QScrollArea *scroll_area) {
  scroll_area_ = scroll_area;
}

void ImageEditWidget::ClearSelection() {
  SelectionTool::ClearSelection(&image_, &selection_, &image_selection_);
  zoom_area_ = QRect();
  repaint();
}

void ImageEditWidget::Rotate(bool cw) {
  QTransform t;
  t.rotate(cw?90:-90);
  if(selection_.isValid()){
    QImage i = QImage(image_selection_.height(),image_selection_.width(),image_selection_.format());
    QPainter p(&i);
    p.drawImage(i.rect(),image_selection_.transformed(t));
    image_selection_ = i;
    QPoint c = selection_.center();
    selection_.setSize(QSize(selection_.height(),selection_.width()));
    selection_.moveCenter(c);
  }else{
    QImage i = QImage(image_.height(),image_.width(),image_.format());
    QPainter p(&i);
    p.drawImage(i.rect(),image_.transformed(t));
    GetImage(&i);
    options_cache_->set_tile_selection(QRect(QPoint(),i.size()));
  }
  repaint();
}

void ImageEditWidget::Flip(bool h, bool v) {
  if(selection_.isValid()){
    image_selection_ = image_selection_.mirrored(h,v);
  }else{
    image_ = image_.mirrored(h,v);
  }
  repaint();
}

void ImageEditWidget::Copy() {
  if (!image_selection_.isNull()) {
    QApplication::clipboard()->setImage(image_selection_);
  }
}

void ImageEditWidget::Cut() {
  Copy();
  selection_ = QRect();
  image_selection_ = QImage();
  repaint();
}

void ImageEditWidget::Paste() {
  QImage img = QApplication::clipboard()->image();
  if (!img.isNull()) {
    if (!selection_.isValid()) {
      selection_.setTopLeft(QPoint(0, 0));
    } else {
      QPainter p(&image_);
      p.drawImage(selection_, image_selection_);
    }
    selection_.setSize(img.size());
    image_selection_ = img;
    repaint();
  }
}

void ImageEditWidget::Delete() {
  selection_ = QRect();
  image_selection_ = QImage();
  repaint();
}

void ImageEditWidget::SelectAll() {
  selection_ = image_.rect();
  image_selection_ = image_;
  image_.fill(options_cache_->alt_color());
  repaint();
}

void ImageEditWidget::paintEvent(QPaintEvent *) {
  QPainter painter(this);

  if (image_.isNull()) {
    return;
  }

  int zoom = options_cache_->zoom();

  QRect image_rect = image_.rect();
  image_rect.setSize(QSize(image_.width() * zoom, image_.height() * zoom));

  painter.drawImage(image_rect, image_);

  if (action_started_) {
    painter.drawImage(image_rect, overlay_image_);
  }

  QRect selection = SelectionRect(selection_);
  if (!image_selection_.isNull()) {
    painter.drawImage(selection.adjusted(0, 0, 1, 1), image_selection_);
  }

  // Draw Grid
  painter.setPen(QColor(0, 0, 0, 50));
  if (zoom > 1 && options_cache_->show_pixel_grid()) {
    for (int x = 0; x < image_.width(); x++) {
      painter.drawLine(x * zoom, 0, x * zoom, image_.height() * zoom);
    }
    for (int y = 0; y < image_.height(); y++) {
      painter.drawLine(0, y * zoom, image_.width() * zoom, y * zoom);
    }
  }
  if (options_cache_->show_grid()) {
    QSize grid_size = options_cache_->grid_size();
    painter.setPen(QColor(255, 0, 0, 100));
    for (int x = 0; x < image_.width(); x += grid_size.width()) {
      painter.drawLine(x * zoom, 0, x * zoom, image_.height() * zoom);
    }
    for (int y = 0; y < image_.height(); y += grid_size.height()) {
      painter.drawLine(0, y * zoom, image_.width() * zoom, y * zoom);
    }
  }

  // Draw Divisions

  // Draw Selection;
  if (selection_.isValid()) {
    painter.setPen(Qt::blue);
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(selection);
    QVector<qreal> dashes = {2, 2};
    QPen pen;
    pen.setColor(Qt::cyan);
    pen.setDashPattern(dashes);
    painter.setPen(pen);
    painter.drawRect(selection);
  }

  // Draw Zoom;
  if (zoom_area_.isValid()) {
    QRect zoom_area = SelectionRect(zoom_area_);
    painter.setPen(Qt::darkGreen);
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(zoom_area);
    QVector<qreal> dashes = {2, 2};
    QPen pen;
    pen.setColor(Qt::green);
    pen.setDashPattern(dashes);
    painter.setPen(pen);
    painter.drawRect(zoom_area);
  }
  // Draw Cursor
  painter.setPen(QColor(Qt::black));
  painter.drawRect(cursor_);
}

void ImageEditWidget::mouseMoveEvent(QMouseEvent *event) {
  int zoom = options_cache_->zoom();
  QPoint pos = event->pos();

  if (rect().contains(pos)) {
    int z = clamp(zoom, 1, 32);
    QPoint p = QPoint((pos.x() / z) * z, (pos.y() / z) * z);
    int cursor_size = zoom - 1;
    cursor_ = QRect(p, QSize(cursor_size, cursor_size));
  } else {
    cursor_ = QRect(0, 0, 0, 0);
  }

  ToolAction(event, ACTION_MOVE);
  previous_pos_ = pos;
  update();
}

void ImageEditWidget::leaveEvent(QEvent *) {
  cursor_ = QRect(0, 0, 0, 0);
  update();
}

void ImageEditWidget::mousePressEvent(QMouseEvent *event) {
  previous_pos_ = event->pos();

  bool contains_pos = rect().contains(event->pos());
  press_left_inside_ = (event->button() == Qt::LeftButton && contains_pos);
  press_right_inside_ = (event->button() == Qt::RightButton && contains_pos);

  switch (event->button()) {
  case Qt::LeftButton:
    left_button_down_ = true;
    break;
  case Qt::RightButton:
    right_button_down_ = true;
    break;
  default:
    break;
  }

  ToolAction(event, ACTION_PRESS);
  update();
}

void ImageEditWidget::mouseReleaseEvent(QMouseEvent *event) {
  switch (event->button()) {
  case Qt::LeftButton:
    left_button_down_ = false;
    break;
  case Qt::RightButton:
    right_button_down_ = false;
    break;
  default:
    break;
  }

  if ((press_left_inside_ || press_right_inside_) && rect().contains(event->pos())) {
    mouseClickEvent(event);
  }

  ToolAction(event, ACTION_RELEASE);
}

void ImageEditWidget::mouseClickEvent(QMouseEvent *event) {
  ToolAction(event, ACTION_CLICK);
}

void ImageEditWidget::ToolAction(const QMouseEvent *event, ACTION_TOOL action) {
  QPoint pos = event->pos();
  ToolEvent tool_event(action, left_button_down_, right_button_down_, WidgetToImageSpace(pos), WidgetToImageSpace(previous_pos_), &undo_redo_);

  switch (options_cache_->tool()) {
  case TOOL_PENCIL:
    PencilTool::Use(&image_, options_cache_->main_color(), tool_event);
    break;
  case TOOL_FLOOD_FILL:
    FloodFillTool::Use(&image_, options_cache_->main_color(), tool_event);
    break;
  case TOOL_LINE:
    LineTool::Use(&image_, &overlay_image_, options_cache_->main_color(), &action_anchor_, &action_started_, tool_event);
    break;
  case TOOL_ELLIPSE:
    EllipseTool::Use(&image_, &overlay_image_, options_cache_->main_color(), options_cache_->alt_color(), &action_anchor_, &action_started_, tool_event);
    break;
  case TOOL_RECTANGLE:
    RectangleTool::Use(&image_, &overlay_image_, options_cache_->main_color(), options_cache_->alt_color(), &action_anchor_, &action_started_, tool_event);
    break;
  case TOOL_SELECTION:
    SelectionTool::Use(&image_, &selection_, &image_selection_, options_cache_->alt_color(), &action_anchor_, &action_started_, tool_event);
    break;
  case TOOL_ZOOM:
    ZoomTool::Use(&zoom_area_, &action_anchor_, &action_started_, scroll_area_, tool_event);
    break;
  default:
    break;
  }
}

QRect ImageEditWidget::SelectionRect(const QRect &rect) {
  int zoom = options_cache_->zoom();
  QPoint topleft = rect.topLeft() * zoom;
  QSize size = (rect.size()) * zoom;
  QRect sel = QRect(topleft, size);
  sel.adjust(0, 0, -1, -1);
  return sel;
}

QPoint ImageEditWidget::WidgetToImageSpace(const QPoint &pos) {
  int zoom = options_cache_->zoom();
  return QPoint(pos.x() / zoom, pos.y() / zoom);
}

void ImageEditWidget::GetImage(QImage *image) {
  if (nullptr == image || image->isNull()) {
    return;
  }

  selection_ = QRect();

  undo_redo_.Do(image_);

  image_ = *image;
  overlay_image_ = QImage(image_.size(), image_.format());
  overlay_image_.fill(0x0);

  UpdateWidget();
}

void ImageEditWidget::HandleRequest() {
  emit SendImage(&image_);
}

void ImageEditWidget::UpdateWidget() {
  int zoom = pApp->options()->zoom();

  QSize image_size = image_.size().scaled(image_.width() * zoom, image_.height() * zoom, Qt::KeepAspectRatio);

  this->setFixedSize(image_size);
  update();
}
