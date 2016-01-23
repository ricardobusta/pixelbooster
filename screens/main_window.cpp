/***************************************************************************\
*  Pixel::Booster, a simple pixel art image editor.                         *
*  Copyright (C) 2015  Ricardo Bustamante de Queiroz (ricardo@busta.com.br) *
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

#include "main_window.h"
#include "ui_main_window.h"

#include <QMdiSubWindow>

#include "application/pixel_booster.h"
#include "logic/action_handler.h"
#include "resources/version.h"
#include "utils/debug.h"
#include "widgets/color_palette_widget.h"
#include "widgets/image_canvas_container.h"

#include <QCloseEvent>
#include <QMenu>
#include <QMessageBox>
#include <QSettings>
#include <QTimer>

const QString kConfigFileName = "config.ini";
const QString kConfigGroupState = "State";
const QString kConfigGroupWindow = "Window";
const QString kConfigWindowState = "WindowState";
const QString kConfigWindowGeometry = "WindowGeometry";
const QString kConfigWindowMaximized = "Maximized";
const bool kConfigWindowMaximizedDefault = false;
const QRect kConfigDefaultWindowGeometry = QRect(50, 50, 800, 600);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      action_handler_(new ActionHandler(this)),
      current_canvas_container_(nullptr),
      options_cache_(pApp->options()) {
  ui->setupUi(this);

  LoadSettings();

  this->setWindowTitle(windowTitle() + " " + kVersionString);

  ui->edit_widget->Clear(options_cache_->tile_selection().size());
  ui->edit_widget->set_scroll_area(ui->scrollArea);

  ConnectActions();
  ConnectWidgets();
  SetToolButtons();

  QTimer::singleShot(0, this, SLOT(PostLoadInit()));
}

void MainWindow::PostLoadInit() {
  UpdateWidgetState();
}

MainWindow::~MainWindow() {
  delete ui;
}

QMdiArea *MainWindow::mdi_area() const {
  return ui->image_mdi_area_;
}

ImageCanvasContainer *MainWindow::current_canvas_container() {
  return current_canvas_container_;
}

ImageEditWidget *MainWindow::edit_widget() {
  return ui->edit_widget;
}

ActionHandler *MainWindow::action_handler() const {
  return action_handler_;
}

QWidget *MainWindow::main_color_button() const {
  return ui->color_main_pushButton;
}

QWidget *MainWindow::alt_color_button() const {
  return ui->color_alt_pushButton;
}

QLabel *MainWindow::zoom_label() const {
  return ui->zoom_label;
}

QSlider *MainWindow::zoom_slider() const {
  return ui->zoom_horizontalSlider;
}

void MainWindow::SetDegColor(const QImage &image) {
  ui->deg_color_widget->SetPalette(image);
  ui->deg_color_widget->update();
}

ColorPaletteWidget *MainWindow::color_palette() const {
  return ui->color_palette_widget;
}

void MainWindow::ConnectActions() {
  // File Actions
  QObject::connect(ui->actionNew, SIGNAL(triggered(bool)), action_handler_, SLOT(NewFile()));
  QObject::connect(ui->actionOpen, SIGNAL(triggered(bool)), action_handler_, SLOT(OpenFile()));
  QObject::connect(ui->actionSave, SIGNAL(triggered(bool)), action_handler_, SLOT(SaveFile()));
  QObject::connect(ui->actionSave_As, SIGNAL(triggered(bool)), action_handler_, SLOT(SaveAs()));
  QObject::connect(ui->actionSave_All, SIGNAL(triggered(bool)), action_handler_, SLOT(SaveAll()));

  // Other Actions
  QObject::connect(ui->actionUndo, SIGNAL(triggered(bool)), action_handler_, SLOT(Undo()));
  QObject::connect(ui->actionRedo, SIGNAL(triggered(bool)), action_handler_, SLOT(Redo()));
  QObject::connect(ui->actionGradient, SIGNAL(triggered(bool)), action_handler_, SLOT(SetColorGradient()));
  QObject::connect(ui->actionSwap_Colors, SIGNAL(triggered(bool)), action_handler_, SLOT(SwapColors()));

  // Interface Actions
  QObject::connect(ui->actionAbout, SIGNAL(triggered(bool)), action_handler_, SLOT(About()));
  QObject::connect(ui->actionTile_Size, SIGNAL(triggered(bool)), action_handler_, SLOT(TileSize()));
  QObject::connect(ui->actionCascade, SIGNAL(triggered(bool)), ui->image_mdi_area_, SLOT(cascadeSubWindows()));
  QObject::connect(ui->actionTile_Subwindows, SIGNAL(triggered(bool)), ui->image_mdi_area_, SLOT(tileSubWindows()));
  QObject::connect(ui->actionExit, SIGNAL(triggered(bool)), this, SLOT(close()));
  QObject::connect(ui->actionTransparency, SIGNAL(triggered(bool)), action_handler_, SLOT(ToggleTransparency(bool)));
  QObject::connect(ui->zoom_horizontalSlider, SIGNAL(valueChanged(int)), action_handler_, SLOT(Zoom(int)));
  QObject::connect(ui->actionLoad_Palette, SIGNAL(triggered(bool)), action_handler_, SLOT(LoadPalette()));
  QObject::connect(ui->actionSave_Palette, SIGNAL(triggered(bool)), action_handler_, SLOT(SavePalette()));
  QObject::connect(ui->actionDefault_Palette, SIGNAL(triggered(bool)), action_handler_, SLOT(DefaultPalette()));
  QObject::connect(ui->actionShow_Grid, SIGNAL(triggered(bool)), action_handler_, SLOT(ToggleShowGrid(bool)));
  QObject::connect(ui->actionShow_Pixel_Grid, SIGNAL(triggered(bool)), action_handler_, SLOT(ToggleShowPixelGrid(bool)));

  // Group Tools
  QActionGroup *tool_action_group = new QActionGroup(this);
  tool_action_group->setExclusive(true);
  tool_action_group->addAction(ui->actionPencil_Tool);
  tool_action_group->addAction(ui->actionFill_Tool);
  tool_action_group->addAction(ui->actionLine_Tool);
  tool_action_group->addAction(ui->actionEllipse_Tool);
  tool_action_group->addAction(ui->actionRectangle_Tool);
  tool_action_group->addAction(ui->actionSelection_Tool);
  tool_action_group->addAction(ui->actionZoom_Tool);
  QMenu *tool_menu = new QMenu(this);
  tool_menu->addActions(tool_action_group->actions());
  QObject::connect(tool_menu, SIGNAL(triggered(QAction *)), action_handler_, SLOT(ToolPressed(QAction *)));
  // Register Tools
  action_handler()->RegisterTool(ui->actionPencil_Tool, TOOL_PENCIL);
  action_handler()->RegisterTool(ui->actionFill_Tool, TOOL_FLOOD_FILL);
  action_handler()->RegisterTool(ui->actionLine_Tool, TOOL_LINE);
  action_handler()->RegisterTool(ui->actionEllipse_Tool, TOOL_ELLIPSE);
  action_handler()->RegisterTool(ui->actionRectangle_Tool, TOOL_RECTANGLE);
  action_handler()->RegisterTool(ui->actionSelection_Tool, TOOL_SELECTION);
  action_handler()->RegisterTool(ui->actionZoom_Tool, TOOL_ZOOM);

  // Install event listeners
  ui->scrollArea->viewport()->installEventFilter(this);

  // Inverse communication
  QObject::connect(action_handler_, SIGNAL(UpdateEditArea()), ui->edit_widget, SLOT(UpdateWidget()));

  // Translation Actions
  QObject::connect(ui->actionPT_BR, SIGNAL(triggered(bool)), action_handler_, SLOT(TranslatePT_BR()));
  QObject::connect(ui->actionEN_US, SIGNAL(triggered(bool)), action_handler_, SLOT(TranslateEN_US()));
}

void MainWindow::ConnectWidgets() {
  QObject::connect(ui->image_mdi_area_, SIGNAL(subWindowActivated(QMdiSubWindow *)), this, SLOT(CurrentWindowChanged(QMdiSubWindow *)));
  QObject::connect(ui->color_main_pushButton, SIGNAL(clicked()), action_handler_, SLOT(OpenMainColorPick()));
  QObject::connect(ui->color_alt_pushButton, SIGNAL(clicked()), action_handler_, SLOT(OpenAltColorPick()));
}

void MainWindow::SetToolButtons() {
  ui->swap_colors_toolButton->setDefaultAction(ui->actionSwap_Colors);
  ui->save_palette_toolButton->setDefaultAction(ui->actionSave_Palette);
  ui->load_palette_toolButton->setDefaultAction(ui->actionLoad_Palette);
  ui->gradient_toolButton->setDefaultAction(ui->actionGradient);
  ui->transparency_toolButton->setDefaultAction(ui->actionTransparency);

  ui->select_toolButton->setDefaultAction(ui->actionSelection_Tool);
  ui->zoom_toolButton->setDefaultAction(ui->actionZoom_Tool);
  ui->text_toolButton->setDefaultAction(ui->actionAdd_Text);
  ui->line_toolButton->setDefaultAction(ui->actionLine_Tool);
  ui->ellipse_toolButton->setDefaultAction(ui->actionEllipse_Tool);
  ui->rectangle_toolButton->setDefaultAction(ui->actionRectangle_Tool);
  ui->fill_toolButton->setDefaultAction(ui->actionFill_Tool);
  ui->pencil_toolButton->setDefaultAction(ui->actionPencil_Tool);
  ui->default_palette_toolButton->setDefaultAction(ui->actionDefault_Palette);

  QMenu *test_menu = new QMenu();
  test_menu->addAction(ui->actionSave);
  test_menu->addAction(ui->actionSave_As);
  test_menu->addAction(ui->actionSave_All);
  ui->actionSave_Menu->setMenu(test_menu);
  test_menu->QObject::connect(ui->actionSave_Menu, SIGNAL(triggered(bool)), ui->actionSave, SLOT(trigger()));
}

void MainWindow::SaveSettings() {
  QSettings settings(kConfigFileName, QSettings::IniFormat);

  settings.beginGroup(kConfigGroupState);
  options_cache_->SaveState(&settings);
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

void MainWindow::LoadSettings() {
  QSettings settings(kConfigFileName, QSettings::IniFormat);

  settings.beginGroup(kConfigGroupState);
  options_cache_->LoadState(&settings);
  settings.endGroup();

  settings.beginGroup(kConfigGroupWindow);
  window_geometry_ = settings.value(kConfigWindowGeometry, kConfigDefaultWindowGeometry).toRect();
  window_geometry_aux_ = window_geometry_;
  setGeometry(window_geometry_);
  restoreState(settings.value(kConfigWindowState).toByteArray());

  if (settings.value(kConfigWindowMaximized, kConfigWindowMaximizedDefault).toBool()) {
    showMaximized();
  }
  settings.endGroup();

  action_handler()->SetColorGradient();

  action_handler()->LoadSavedPalette();
}

void MainWindow::UpdateWidgetState() {
  ui->actionTransparency->setChecked(options_cache_->transparency_enabled());
  action_handler()->GetTool(options_cache_->tool())->setChecked(true);
  action_handler_->SetMainColor(options_cache_->main_color());
  action_handler_->SetAltColor(options_cache_->alt_color());
  action_handler_->Translate(options_cache_->language());
  ui->zoom_horizontalSlider->setValue(options_cache_->zoom());
  ui->zoom_label->setText(QString("x%1").arg(options_cache_->zoom()));
  ui->zoom_label->update();
  ui->zoom_label->repaint();
  ui->actionShow_Grid->setChecked(options_cache_->show_grid());
  ui->actionShow_Pixel_Grid->setChecked(options_cache_->show_pixel_grid());
}

void MainWindow::changeEvent(QEvent *event) {
  if (event->type() == QEvent::LanguageChange) {
    ui->retranslateUi(this);
  }
}

void MainWindow::closeEvent(QCloseEvent *event) {
  bool ans = action_handler()->CloseWindowHandler();
  if (ans) {
    event->accept();
  } else {
    event->ignore();
  }
  SaveSettings();
}

void MainWindow::resizeEvent(QResizeEvent *event) {
  if (windowState() != Qt::WindowMaximized) {
    window_geometry_aux_ = window_geometry_;
    window_geometry_ = geometry();
  } else {
    window_geometry_ = window_geometry_aux_;
  }
}

bool MainWindow::eventFilter(QObject * obj, QEvent * event) {
  if(obj == ui->scrollArea->viewport()){
    if(event->type() == QEvent::Wheel){
      QWheelEvent * wheel_event = static_cast<QWheelEvent*>(event);
      if(wheel_event->modifiers() & Qt::ControlModifier){
        int increment = wheel_event->delta()>0?1:wheel_event->delta()<0?-1:0;
        action_handler()->Zoom(options_cache_->zoom()+increment);
        return true;
      }
    }
  }
  return QMainWindow::eventFilter(obj,event);
}

void MainWindow::CurrentWindowChanged(QMdiSubWindow *w) {
  if (nullptr != current_canvas_container_) {
    current_canvas_container_->RemoveAsActive(ui->edit_widget);
    current_canvas_container_ = nullptr;
  }

  if (nullptr != w) {
    current_canvas_container_ = dynamic_cast<ImageCanvasContainer *>(w->widget());
    if (nullptr != current_canvas_container_) {
      current_canvas_container_->SetAsActive(ui->edit_widget);
    }
  }
}
