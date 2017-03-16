/***************************************************************************\
*  Pixel::Booster, a simple pixel art image editor.                         *
*  Copyright (C) 2017  Ricardo Bustamante de Queiroz (ricardo@busta.com.br) *
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

#include "imagewidget.h"

#include <QDebug>
#include <QPainter>
#include <QMouseEvent>

ImageWidget::ImageWidget(QWidget *parent) : QWidget(parent)
{

}

void ImageWidget::paintEvent(QPaintEvent *event)
{
  qDebug() << "paiting" << selection_;
  QPainter painter(this);

  painter.drawImage(rect(),project.image);

  if(selection_.isValid()){
    painter.setOpacity(0.2f);
    painter.setBrush(Qt::black);
    painter.setPen(Qt::NoPen);

    for(int i=0;i<4;i++){
      painter.drawRect(outerSelection_[i]);
    }
  }
  if(mouseDown_){
    painter.setOpacity(1.0f);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(Qt::black);
    painter.drawRect(QRect(anchor_,currentCursorPos_));
  }
}

void ImageWidget::mousePressEvent(QMouseEvent *e)
{
  mouseDown_ = true;
  anchor_ = e->pos();
}

void ImageWidget::mouseReleaseEvent(QMouseEvent *e)
{
  mouseDown_ = false;
  QRect sel = QRect(anchor_,e->pos()).normalized();
  if(sel.isValid()){
    selection_ = sel;
    outerSelection_[0] = QRect(QPoint(rect().left(),rect().top()),QPoint(rect().right(),selection_.top()-1));
    outerSelection_[1] = QRect(QPoint(selection_.right(),selection_.top()),QPoint(rect().right(),rect().bottom()-1));
    outerSelection_[2] = QRect(QPoint(rect().left(),selection_.top()),QPoint(selection_.left()-1,rect().bottom()-1));
    outerSelection_[3] = QRect(QPoint(selection_.left(),selection_.bottom()),QPoint(selection_.right()-1,rect().bottom()-1));
    qDebug() << "Eita";
  }
  repaint();
}

void ImageWidget::mouseMoveEvent(QMouseEvent *e)
{
  currentCursorPos_ = e->pos();
  repaint();
}

