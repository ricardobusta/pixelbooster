#ifndef COLORPICKWIDGET_H
#define COLORPICKWIDGET_H

#include <QWidget>

namespace Ui {
class ColorPickWidget;
}

class ColorPickWidget : public QWidget
{
  Q_OBJECT

public:
  explicit ColorPickWidget(QWidget *parent = 0);
  ~ColorPickWidget();

private:
  Ui::ColorPickWidget *ui;
};

#endif // COLORPICKWIDGET_H
