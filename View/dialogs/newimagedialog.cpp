/***************************************************************************\
*  Pixel::Booster, a simple pixel art image editor.                         *
*  Copyright (C) 2017  Ricardo Bustamante de Queiroz (ricardo@busta.com.br) *
*  Visit the Official Homepage: pixel.busta.com.br                          *
*                                                                           *
*  This program is free software: you can redistribute it and/or modify     *
*  it under the terms of the GNU General Public License as published by     *
*  the Free Software Foundation, either version 3 of the License, or        *
*  (at your option) any later version.                                      *
*                                                                           *
*  This program is distributed in the hope that it will be useful,          *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
*  GNU General Public License for more details.                             *
*                                                                           *
*  You should have received a copy of the GNU General Public License        *
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.    *
\***************************************************************************/

#include "newimagedialog.h"
#include "ui_newimagedialog.h"

#include "application.h"
#include "logic/projectmanager.h"

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
  pbApp->projectManager()->CreateImage(name,w,h,0xffffffff);
}
