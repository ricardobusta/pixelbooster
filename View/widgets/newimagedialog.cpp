#include "newimagedialog.h"
#include "ui_newimagedialog.h"

#include "application.h"

NewImageDialog::NewImageDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::NewImageDialog)
{
  ui->setupUi(this);
}

NewImageDialog::~NewImageDialog()
{
  delete ui;
}

void NewImageDialog::CreateImage()
{
  QString name = ui->name->text();
  int w = ui->width->value();
  int h = ui->height->value();
  pbApp->CreateImage(name,w,h,0xffffffff);
}
