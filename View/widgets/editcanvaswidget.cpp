#include "editcanvaswidget.h"

#include <QPainter>

EditCanvasWidget::EditCanvasWidget(QWidget *parent) : QWidget(parent)
{

}

void EditCanvasWidget::paintEvent(QPaintEvent *e)
{
  QPainter painter(this);

  painter.setPen(Qt::black);
  painter.setBrush(Qt::white);

  painter.drawRect(rect().adjusted(0,0,-1,-1));
}
