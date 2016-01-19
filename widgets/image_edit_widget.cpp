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

#include "image_edit_widget.h"

#include <QMouseEvent>
#include <QPainter>

#include "application/pixel_booster.h"
#include "logic/action_handler.h"
#include "screens/main_window.h"
#include "utils/debug.h"

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
    image_ = img;
    update();
  }
}

void ImageEditWidget::Redo() {
  QImage img = undo_redo_.Redo(image_);
  if (!img.isNull()) {
    image_ = img;
    update();
  }
}

void ImageEditWidget::paintEvent(QPaintEvent *) {
  QPainter painter(this);

  if (image_.isNull()) {
    return;
  }

  int zoom = pApp->options()->zoom();

  QRect image_rect = image_.rect();
  image_rect.setSize(QSize(image_.width() * zoom, image_.height() * zoom));

  painter.drawImage(image_rect, image_);
  painter.drawImage(image_rect, overlay_image_);

  painter.drawRect(cursor_);

  if (cursor_.isValid()) {
    painter.drawPoint(cursor_.topLeft());
  }
}

void ImageEditWidget::mouseMoveEvent(QMouseEvent *event) {
  int zoom = pApp->options()->zoom();
  QPoint pos = event->pos();

  if (rect().contains(pos)) {
    QPoint p = QPoint((pos.x() / zoom) * zoom, (pos.y() / zoom) * zoom);
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
  QPoint img_pos = WidgetToImageSpace(pos);

  if (action == ACTION_PRESS) {
    undo_redo_.Do(image_);
  }

  switch (options_cache_->tool()) {
  case TOOL_PENCIL:
    if (action == ACTION_PRESS || action == ACTION_MOVE) {
      if (left_button_down_) {
        QPoint img_previous_pos = WidgetToImageSpace(previous_pos_);
        ToolAlgorithm::Pencil(&image_, img_previous_pos, img_pos, options_cache_->main_color());
      } else if (right_button_down_) {
        pApp->main_window()->action_handler()->SetMainColor(image_.pixel(img_pos));
      }
    }
    break;
  case TOOL_FLOOD_FILL:
    if (left_button_down_) {
      ToolAlgorithm::FloodFill(&image_, action, img_pos, options_cache_->main_color());
    } else if (right_button_down_) {
      pApp->main_window()->action_handler()->SetMainColor(image_.pixel(img_pos));
    }
    break;
  case TOOL_LINE:
    if (action == ACTION_PRESS) {
      if (left_button_down_) {
        action_anchor_ = WidgetToImageSpace(pos);
        action_started_ = true;
        overlay_image_.fill(0x0);
        ToolAlgorithm::BresenhamLine(&overlay_image_, action_anchor_, img_pos, options_cache_->main_color().rgba());
      } else if (right_button_down_) {
        pApp->main_window()->action_handler()->SetMainColor(image_.pixel(img_pos));
      }
    } else if (action == ACTION_MOVE) {
      if (action_started_) {
        overlay_image_.fill(0x0);
        ToolAlgorithm::BresenhamLine(&overlay_image_, action_anchor_, img_pos, options_cache_->main_color().rgba());
      }
    } else if (action == ACTION_RELEASE) {
      QPainter apply(&image_);
      apply.drawImage(image_.rect(), overlay_image_);
      overlay_image_.fill(0x0);
      action_started_ = false;
    }
    break;
  case TOOL_ELLIPSE:
    if (action == ACTION_PRESS) {
      if (left_button_down_) {
        action_anchor_ = WidgetToImageSpace(pos);
        action_started_ = true;
      } else if (right_button_down_) {
        pApp->main_window()->action_handler()->SetMainColor(image_.pixel(img_pos));
      }
    } else if (action == ACTION_MOVE) {
      if (action_started_ && img_pos != action_anchor_) {

        overlay_image_.fill(0x0);
        QRect rect = QRect(qMin(action_anchor_.x(),img_pos.x()),
                           qMin(action_anchor_.y(),img_pos.y()),
                           qAbs(action_anchor_.x()-img_pos.x())+1,
                           qAbs(action_anchor_.y()-img_pos.y())+1);
        ToolAlgorithm::BresenhamEllipse(&overlay_image_, rect, true, options_cache_->alt_color().rgba());
        ToolAlgorithm::BresenhamEllipse(&overlay_image_, rect, false, options_cache_->main_color().rgba());
      }else{
        if (action_started_){
          overlay_image_.fill(0x0);
          overlay_image_.setPixel(img_pos,options_cache_->main_color().rgba());
        }
      }
    } else if (action == ACTION_RELEASE) {
      QPainter apply(&image_);
      apply.drawImage(image_.rect(), overlay_image_);
      overlay_image_.fill(0x0);
      action_started_ = false;
    }
    break;
  case TOOL_RECTANGLE:
    if (action == ACTION_PRESS) {
      if (left_button_down_) {
        action_anchor_ = WidgetToImageSpace(pos);
        action_started_ = true;
      } else if (right_button_down_) {
        pApp->main_window()->action_handler()->SetMainColor(image_.pixel(img_pos));
      }
    } else if (action == ACTION_MOVE) {
      if (action_started_ && img_pos != action_anchor_) {
        overlay_image_.fill(0x0);
        QRect rect = QRect(qMin(action_anchor_.x(),img_pos.x()),
                           qMin(action_anchor_.y(),img_pos.y()),
                           qAbs(action_anchor_.x()-img_pos.x()),
                           qAbs(action_anchor_.y()-img_pos.y()));
        QPainter overlay(&overlay_image_);
        overlay.setPen(options_cache_->main_color());
        overlay.setBrush(options_cache_->alt_color());
        overlay.drawRect(rect);
      }else{
        if (action_started_){
          overlay_image_.fill(0x0);
          overlay_image_.setPixel(img_pos,options_cache_->main_color().rgba());
        }
      }
    } else if (action == ACTION_RELEASE) {
      QPainter apply(&image_);
      apply.drawImage(image_.rect(), overlay_image_);
      overlay_image_.fill(0x0);
      action_started_ = false;
    }
    break;
  case TOOL_SELECTION:
    break;
  default:
    break;
  }
}

QPoint ImageEditWidget::WidgetToImageSpace(const QPoint &pos) {
  int zoom = options_cache_->zoom();
  return QPoint(pos.x() / zoom, pos.y() / zoom);
}

void ImageEditWidget::GetImage(QImage *image) {
  if (NULL == image || image->isNull()) {
    return;
  }

  image_ = *image;
  overlay_image_ = QImage(image_.size(), image_.format());
  overlay_image_.fill(0x0);

  undo_redo_.Do(image_);

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
