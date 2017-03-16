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

#include "projectmanager.h"

#include <QFileDialog>
#include <QApplication>
#include <QMessageBox>

#include "logic/imageproject.h"
#include "mainwindow.h"

ProjectManager::ProjectManager(QObject *parent) : QObject(parent)
{

}

void ProjectManager::OpenFile()
{
  QStringList file_names = QFileDialog::getOpenFileNames(qApp->activeWindow(), tr("Open Files"), ".", tr("Images")+" (*.png *.bmp *.jpg *.jpeg *.pbm *.pgm *.ppm *.tiff *.xbm *.xpm)");

  for (QString file_name : file_names) {
    if (!file_name.isEmpty()) {
      QImage image(file_name);
      if (!image.isNull()) {
        if (image.format() == QImage::Format_Indexed8) {
          int ans = QMessageBox::question(qApp->activeWindow(),
                                          QString(tr("UNABLE_TO_OPEN_FILE_NAME")),
                                          QString(tr("THE_FILE_YOU_TRIED_TO_OPEN_IS_IN_INVALID_FORMAT")),
                                          QMessageBox::Yes | QMessageBox::No,
                                          QMessageBox::No);
          if (ans == QMessageBox::Yes) {
            image = image.convertToFormat(QImage::Format_ARGB32);
          } else {
            return;
          }
        }
        ImageProject pro = ImageProject();
        pro.image = image;
        pro.name = file_name;
        windowRef->AddProjectTab(pro);
      }
    }
  }
}

void ProjectManager::CreateImage(const QString &name, int w, int h, const QColor &bg)
{
  QImage image = QImage(w,h,QImage::Format_ARGB32);
  image.fill(bg);

  ImageProject pro = ImageProject();
  pro.image = image;
  pro.name = name;

  windowRef->AddProjectTab(pro);
}
