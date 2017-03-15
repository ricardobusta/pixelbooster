#ifndef EDITCANVASWIDGET_H
#define EDITCANVASWIDGET_H

#include <QWidget>

class EditCanvasWidget : public QWidget
{
  Q_OBJECT
public:
  explicit EditCanvasWidget(QWidget *parent = 0);

private:
  void paintEvent(QPaintEvent *e);

signals:

public slots:
};

#endif // EDITCANVASWIDGET_H
