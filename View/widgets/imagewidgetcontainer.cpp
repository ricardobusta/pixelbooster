#include "imagewidgetcontainer.h"
#include "ui_imagewidgetcontainer.h"

ImageWidgetContainer::ImageWidgetContainer(const ImageProject &p, QWidget *parent) :
  QScrollArea(parent),
  ui(new Ui::ImageWidgetContainer)
{
  ui->setupUi(this);

  ui->image_widget->project = p;
  ui->image_widget->setFixedSize(p.image.width(),p.image.height());
}

ImageWidgetContainer::~ImageWidgetContainer()
{
  delete ui;
}
