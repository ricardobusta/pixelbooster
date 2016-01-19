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

#ifndef COLOR_DIALOG_H
#define COLOR_DIALOG_H

#include <QDialog>

namespace Ui {
class Color_Dialog;
}

class ColorDialog : public QDialog
{
  Q_OBJECT

public:
  explicit ColorDialog(QWidget *parent = 0);
  ~ColorDialog();

  static QColor GetColor(const QColor &initial, QWidget *parent=0, const QString & title = QString());
private:
  Ui::Color_Dialog *ui;
};

#endif // COLOR_DIALOG_H
