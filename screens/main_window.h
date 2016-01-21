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

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QMdiArea;
class QMdiSubWindow;
class QLabel;
class ActionHandler;
class ImageCanvasContainer;
class GlobalOptions;
class ImageEditWidget;
class ColorPaletteWidget;

/*!
 * \brief The MainWindow class
 */
class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

  QMdiArea *mdi_area() const;
  ImageCanvasContainer *current_canvas_container();
  ImageEditWidget *edit_widget();

  ActionHandler *action_handler() const;
  QWidget *main_color_button() const;
  QWidget *alt_color_button() const;
  QLabel *zoom_label() const;

  void SetDegColor(const QImage &image);
  ColorPaletteWidget *color_palette() const;

private:
  Ui::MainWindow *ui;
  ActionHandler *action_handler_;
  ImageCanvasContainer *current_canvas_container_;

  GlobalOptions *options_cache_;

  QRect window_geometry_;
  QRect window_geometry_aux_;
  bool safe_resolution_;

  void ConnectActions();
  void ConnectWidgets();

  void SetToolButtons();

  void SaveSettings();
  void LoadSettings();
  void UpdateWidgetState();

  void changeEvent(QEvent *event);
  void closeEvent(QCloseEvent *event);
  void resizeEvent(QResizeEvent *event);

private slots:
  void CurrentWindowChanged(QMdiSubWindow *w);
  void PostLoadInit();
};

#endif // MAIN_WINDOW_H
