#include "colorpickwidget.h"
#include "ui_colorpickwidget.h"

ColorPickWidget::ColorPickWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::ColorPickWidget)
{
  ui->setupUi(this);
}

ColorPickWidget::~ColorPickWidget()
{
  delete ui;
}
