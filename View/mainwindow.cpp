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

#include <QTimer>
#include <QCloseEvent>
#include <QSettings>

#include "constants.h"
#include "dialogs/aboutdialog.h"
#include "dialogs/newimagedialog.h"
#include "widgets/imagewidgetcontainer.h"
#include "logic/imageproject.h"
#include "application.h"
#include "logic/projectmanager.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  //LoadSettings();

  ConnectActions();

  QTimer::singleShot(0, this, SLOT(PostLoadInit()));
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::ConnectActions()
{
  ProjectManager * pm = pbApp->projectManager();
  QObject::connect(ui->actionABOUT,SIGNAL(triggered(bool)),this,SLOT(ShowAboutDialog()));
  QObject::connect(ui->actionNEW_FILE,SIGNAL(triggered(bool)),this,SLOT(ShowNewImageDialog()));
  QObject::connect(ui->actionOPEN_FILE,SIGNAL(triggered(bool)),pm,SLOT(OpenFile()));
}

#define SETTINGS_VALUE(var) (settings.value(var, var##Default))

void MainWindow::LoadSettings()
{
  QSettings settings(kConfigFileName, QSettings::IniFormat);

  //   settings.beginGroup(kConfigGroupState);
  //   options_cache_->LoadState(&settings);
  //   settings.endGroup();

  settings.beginGroup(kConfigGroupWindow);
  window_geometry_ = SETTINGS_VALUE(kConfigWindowGeometry).toRect();
  window_geometry_aux_ = window_geometry_;
  setGeometry(window_geometry_);
  restoreState(settings.value(kConfigWindowState).toByteArray());

  if (SETTINGS_VALUE(kConfigWindowMaximized).toBool()) {
    showMaximized();
  }
  settings.endGroup();

  //action_handler()->SetColorGradient();

  //action_handler()->LoadSavedPalette();
}

void MainWindow::SaveSettings()
{
  QSettings settings(kConfigFileName, QSettings::IniFormat);

  settings.beginGroup(kConfigGroupState);
  //options_cache_->SaveState(&settings);
  settings.endGroup();

  settings.beginGroup(kConfigGroupWindow);
  if (windowState() == Qt::WindowMaximized) {
    settings.setValue(kConfigWindowGeometry, window_geometry_aux_);
  } else {
    settings.setValue(kConfigWindowGeometry, this->geometry());
  }
  settings.setValue(kConfigWindowMaximized, this->isMaximized());
  settings.setValue(kConfigWindowState, this->saveState());
  settings.endGroup();
}

void MainWindow::PostLoadInit()
{

}

bool MainWindow::HandleCloseWindow()
{
  bool avoid_closing = false;
  //  for (ImageCanvasWidget *w : *ImageCanvasWidget::open_canvas()) {
  //    if (!w->saved_state()) {
  //      avoid_closing = true;
  //      break;
  //    }
  //  }
  //  if (avoid_closing) {
  //    int ans = QMessageBox::question(window_cache_, "Unsaved files...", "Do you want to keep the unsaved changes on the images?", "Save All", "Don't Save", "Cancel", 2, 2);
  //    if (ans == 2) {
  //      return false;
  //    } else if (ans == 0) {
  //      SaveAll();
  //    }
  //  }
  return true;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
  bool ans = HandleCloseWindow();
  if (ans) {
    event->accept();
  } else {
    event->ignore();
  }
  SaveSettings();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
  Q_UNUSED(event);
  if (windowState() != Qt::WindowMaximized) {
    window_geometry_aux_ = window_geometry_;
    window_geometry_ = geometry();
  } else {
    window_geometry_ = window_geometry_aux_;
  }
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
  ui->image_tabWidget->addTab(new ImageWidgetContainer(p),p.name);
}
