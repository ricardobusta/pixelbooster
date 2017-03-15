#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>

#include "logic/imageproject.h"

class ImageWidget : public QWidget
{
  Q_OBJECT
public:
  explicit ImageWidget(QWidget *parent = 0);

private:
  void paintEvent(QPaintEvent *event);

  ImageProject image;
signals:

public slots:
};

#endif // IMAGEWIDGET_H
