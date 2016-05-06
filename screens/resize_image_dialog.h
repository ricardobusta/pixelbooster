#ifndef RESIZE_IMAGE_DIALOG_H
#define RESIZE_IMAGE_DIALOG_H

#include <QDialog>

namespace Ui {
class Resize_Image_Dialog;
}

class ResizeImageDialog : public QDialog
{
  Q_OBJECT

public:
  explicit ResizeImageDialog(const QSize &original_size, QWidget *parent = 0);
  ~ResizeImageDialog();

  QSize new_size() const;
private:
  Ui::Resize_Image_Dialog *ui;

  QSize new_size_;

private slots:
  void UpdateSize();
};

#endif // RESIZE_IMAGE_DIALOG_H
