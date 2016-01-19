/***************************************************************************\
*  Pixel::Booster, a simple pixel art image editor.                         *
*  Copyright (C) 2015  Ricardo Bustamante de Queiroz (ricardo@busta.com.br) *
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

#include "color_dialog.h"
#include "ui_color_dialog.h"

#include <QColorDialog>

ColorDialog::ColorDialog(QWidget *parent) : QDialog(parent),
                                            ui(new Ui::Color_Dialog) {
  ui->setupUi(this);
}

ColorDialog::~ColorDialog() {
  delete ui;
}

QColor ColorDialog::GetColor(const QColor &initial, QWidget *parent, const QString &title) {
  // To do:
  if (true) {
    return QColorDialog::getColor(initial, parent, title, QColorDialog::ShowAlphaChannel);
  } else {
    ColorDialog dialog;
    dialog.exec();
    if (dialog.result() == QDialog::Accepted) {
      return QColor(Qt::red);
    }
    return QColor();
  }
}
