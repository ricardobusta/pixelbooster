#include "resize_image_dialog.h"
#include "ui_resize_image_dialog.h"

ResizeImageDialog::ResizeImageDialog(const QSize &original_size, QWidget *parent)
    : QDialog(parent),
      ui(new Ui::Resize_Image_Dialog) {
  ui->setupUi(this);
  ui->width_spinBox->setValue(original_size.width());
  ui->height_spinBox->setValue(original_size.height());

  QObject::connect(this, SIGNAL(accepted()), this, SLOT(UpdateSize()));
}

ResizeImageDialog::~ResizeImageDialog() {
  delete ui;
}

QSize ResizeImageDialog::new_size() const {
  return new_size_;
}

void ResizeImageDialog::UpdateSize() {
  new_size_ = QSize(ui->width_spinBox->value(),ui->height_spinBox->value());
}
