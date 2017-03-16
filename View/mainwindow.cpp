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

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "widgets/aboutdialog.h"
#include "widgets/newimagedialog.h"
#include "widgets/imagewidgetcontainer.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  ConnectActions();
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::ConnectActions()
{
  QObject::connect(ui->actionABOUT,SIGNAL(triggered(bool)),this,SLOT(ShowAboutDialog()));
  QObject::connect(ui->actionNEW_FILE,SIGNAL(triggered(bool)),this,SLOT(ShowNewImageDialog()));
}

void MainWindow::ShowAboutDialog()
{
  AboutDialog *d = new AboutDialog(this);

  d->exec();

  delete d;
}

void MainWindow::ShowNewImageDialog()
{
  NewImageDialog *d = new NewImageDialog(this);

  if(d->exec() == QDialog::Accepted){
    d->CreateImage();
  }

  delete d;
}

void MainWindow::AddProjectTab(const ImageProject &p)
{
  ui->image_tabWidget->addTab(new ImageWidgetContainer(),"New");
}
