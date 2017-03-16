#ifndef IMAGEWIDGETCONTAINER_H
#define IMAGEWIDGETCONTAINER_H

#include <QScrollArea>

class ImageProject;

namespace Ui {
class ImageWidgetContainer;
}

class ImageWidgetContainer : public QScrollArea
{
  Q_OBJECT

public:
  explicit ImageWidgetContainer(const ImageProject &p, QWidget *parent = 0);
  ~ImageWidgetContainer();

private:
  Ui::ImageWidgetContainer *ui;
};

#endif // IMAGEWIDGETCONTAINER_H
