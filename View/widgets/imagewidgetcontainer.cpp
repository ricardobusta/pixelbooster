#include "imagewidgetcontainer.h"
#include "ui_imagewidgetcontainer.h"

ImageWidgetContainer::ImageWidgetContainer(QWidget *parent) :
  QScrollArea(parent),
  ui(new Ui::ImageWidgetContainer)
{
  ui->setupUi(this);
}

ImageWidgetContainer::~ImageWidgetContainer()
{
  delete ui;
}
