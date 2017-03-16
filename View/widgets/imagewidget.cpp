#include "imagewidget.h"

#include <QPainter>

ImageWidget::ImageWidget(QWidget *parent) : QWidget(parent)
{

}

void ImageWidget::paintEvent(QPaintEvent *event)
{
  QPainter painter(this);

  painter.drawImage(rect(),project.image);
}

