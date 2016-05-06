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

#include "action_handler.h"

#include "application/pixel_booster.h"
#include "resources/version.h"
#include "screens/about_dialog.h"
#include "screens/help_dialog.h"
#include "screens/main_window.h"
#include "screens/new_image_file_dialog.h"
#include "screens/resize_image_dialog.h"
#include "screens/set_tile_size_dialog.h"
#include "utils/debug.h"
#include "utils/pb_math.h"
#include "widgets/color_dialog.h"
#include "widgets/color_palette_widget.h"
#include "widgets/image_canvas_container.h"
#include "widgets/image_canvas_widget.h"
#include "widgets/image_edit_widget.h"

#include <QAction>
#include <QFileDialog>
#include <QLabel>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QMessageBox>
#include <QPainter>
#include <QSlider>

const QString kTxtSelectMainColor = "Select Main Color";
const QString kTxtSelectAltColor = "Select Secondary Color";

const QString kColorButtonStyle = "background-color: %1; border: 1px solid black;";

const QString kSavedPaletteLocation = "palette.png";

ActionHandler::ActionHandler(QObject *parent)
    : QObject(parent),
      options_cache_(pApp->options()),
      window_cache_(dynamic_cast<MainWindow *>(parent)) {
}

ActionHandler::~ActionHandler() {
}

void ActionHandler::RegisterTool(QAction *tool, int val) {
  tool_action_map_.insert(tool, val);
}

QAction *ActionHandler::GetTool(int val) {
  return tool_action_map_.key(val, nullptr);
}

void ActionHandler::NewFile() const {
  NewImageFileDialog *image_file_dialog = new NewImageFileDialog(window_cache_);
  int result = image_file_dialog->exec();

  // Check if the user canceled the new image creation.
  if (result != QFileDialog::Accepted) {
    delete image_file_dialog;
    return;
  }

  // Gather information from the dialog to create the image.
  QSize size = image_file_dialog->selected_size();
  QImage::Format format = image_file_dialog->selected_format();

  QImage image(size, format);
  image.fill(image_file_dialog->selected_color());
  CreateImageCanvas(image, "");

  delete image_file_dialog;
  return;
}

void ActionHandler::OpenFile() const {
  QStringList file_names = QFileDialog::getOpenFileNames(window_cache_, "Open Files", ".", "Images (*.png *.bmp *.jpg *.jpeg *.pbm *.pgm *.ppm *.tiff *.xbm *.xpm)");

  for (QString file_name : file_names) {
    if (!file_name.isEmpty()) {
      QImage image(file_name);
      if (!image.isNull()) {
        if (image.format() == QImage::Format_Indexed8) {
          int ans = QMessageBox::question(window_cache_,
                                          QString("Unable to open image file."),
                                          QString("The file you tried to open is an 8 bit indexed image format and is not supported in the current version of the software. Do you want to convert it to another image format?"),
                                          QMessageBox::Yes | QMessageBox::No,
                                          QMessageBox::No);
          if (ans == QMessageBox::Yes) {
            image = image.convertToFormat(QImage::Format_ARGB32);
          } else {
            return;
          }
        }
        CreateImageCanvas(image, file_name);
      }
    }
  }
}

void ActionHandler::SaveFile() const {
  ImageCanvasContainer *c = window_cache_->current_canvas_container();
  if (nullptr != c) {
    ImageCanvasWidget *w = c->GetCanvasWidget();
    //if (!w->saved_state()) {
    DEBUG_MSG("Saving image");
    w->Save();
    //}
  }
}

void ActionHandler::SaveAll() const {
  DEBUG_MSG("Should attempt to save all unsaved files");

  for (ImageCanvasWidget *w : *ImageCanvasWidget::open_canvas()) {
    //if (!w->saved_state()) {
    DEBUG_MSG("Saving image");
    w->Save();
    //}
  }
}

void ActionHandler::SaveAs() const {
  ImageCanvasContainer *c = window_cache_->current_canvas_container();
  if (nullptr != c) {
    ImageCanvasWidget *w = c->GetCanvasWidget();
    DEBUG_MSG("Saving image");
    w->SaveAs();
  }
}

bool ActionHandler::CloseWindowHandler() const {
  bool avoid_closing = false;
  for (ImageCanvasWidget *w : *ImageCanvasWidget::open_canvas()) {
    if (!w->saved_state()) {
      avoid_closing = true;
      break;
    }
  }
  if (avoid_closing) {
    int ans = QMessageBox::question(window_cache_, "Unsaved files...", "Do you want to keep the unsaved changes on the images?", "Save All", "Don't Save", "Cancel", 2, 2);
    if (ans == 2) {
      return false;
    } else if (ans == 0) {
      SaveAll();
    }
  }
  return true;
}

void ActionHandler::Undo() const {
  window_cache_->edit_widget()->Undo();
}

void ActionHandler::Redo() const {
  window_cache_->edit_widget()->Redo();
}

void ActionHandler::ToolPressed(QAction *a) const {
  auto it = tool_action_map_.find(a);
  if (it != tool_action_map_.end()) {
    options_cache_->set_tool(static_cast<TOOL_ENUM>(it.value()));
    window_cache_->edit_widget()->ClearSelection();
  }
}

void ActionHandler::ArrowPressed(QAction *a) const {
  DEBUG_MSG(a->objectName());
  QString tool = a->objectName();

  if (tool == "actionRotate_90_CCW") {
    window_cache_->edit_widget()->Rotate(false);
  } else if (tool == "actionRotate_90_CW") {
    window_cache_->edit_widget()->Rotate(true);
  } else if (tool == "actionFlip_Horizontal") {
    window_cache_->edit_widget()->Flip(true, false);
  } else if (tool == "actionFlip_Vertical") {
    window_cache_->edit_widget()->Flip(false, true);
  }
}

void ActionHandler::About() const {
  AboutDialog *about_dialog = new AboutDialog(window_cache_);
  about_dialog->exec();
  delete about_dialog;
}

void ActionHandler::Help() const {
    HelpDialog *help_dialog = new HelpDialog(window_cache_);
    help_dialog->exec();
    delete help_dialog;
}

void ActionHandler::TileSize() const {
  SetTileSizeDialog *tile_dialog = new SetTileSizeDialog(window_cache_);
  tile_dialog->exec();
  delete tile_dialog;
}

void ActionHandler::ToggleTransparency(bool transparency) const {
  options_cache_->set_transparency_enabled(transparency);
}

void ActionHandler::Zoom(int zoom) const {
  int z = clamp(zoom, 1, 32);
  options_cache_->set_zoom(z);
  window_cache_->zoom_label()->setText(QString("x%1").arg(z));
  QSlider *slider = window_cache_->zoom_slider();
  slider->blockSignals(true);
  slider->setValue(z);
  slider->blockSignals(false);
  emit UpdateEditArea();
}

void ActionHandler::OpenMainColorPick() const {
  QColor color = ColorDialog::GetColor(options_cache_->main_color(), window_cache_, kTxtSelectMainColor);
  if (color.isValid()) {
    SetMainColor(color);
  }
}

void ActionHandler::OpenAltColorPick() const {
  QColor color = ColorDialog::GetColor(options_cache_->alt_color(), window_cache_, kTxtSelectAltColor);
  if (color.isValid()) {
    SetAltColor(color);
  }
}

void ActionHandler::TranslatePT_BR() const {
  Translate("pt_br");
}

void ActionHandler::TranslateEN_US() const {
  Translate("en_us");
}

void ActionHandler::SetMainColor(const QColor &color) const {
  options_cache_->set_main_color(color);
  window_cache_->main_color_button()->setStyleSheet(kColorButtonStyle.arg(color.name()));
}

void ActionHandler::SetAltColor(const QColor &color) const {
  options_cache_->set_alt_color(color);
  window_cache_->alt_color_button()->setStyleSheet(kColorButtonStyle.arg(color.name()));
}

void ActionHandler::SwapColors() const {
  QColor color = options_cache_->main_color();
  SetMainColor(options_cache_->alt_color());
  SetAltColor(color);
}

void ActionHandler::SetColorGradient() const {
  QColor main = options_cache_->main_color();
  QColor alt = options_cache_->alt_color();
  QImage new_deg = QImage(10, 3, QImage::Format_ARGB32_Premultiplied);
  QColor black = Qt::black;
  QColor white = Qt::white;
  for (int i = 0; i < 10; i++) {
    float f = float(i) / 9.0f;
    QColor c0 = ColorLerp(main, alt, f);
    QColor c1 = ColorLerp(main, black, f);
    QColor c2 = ColorLerp(main, white, f);
    new_deg.setPixel(i, 0, c0.rgba());
    new_deg.setPixel(i, 1, c1.rgba());
    new_deg.setPixel(i, 2, c2.rgba());
  }
  window_cache_->SetDegColor(new_deg);
}

void ActionHandler::LoadPalette() const {
  QString file_name = QFileDialog::getOpenFileName(window_cache_, "Import palette image file...", ".", "Images (*.png *.bmp *.jpg *.jpeg *.pbm *.pgm *.ppm *.tiff *.xbm *.xpm)");

  if (!file_name.isEmpty()) {
    QImage image(file_name);
    if (!image.isNull()) {
      window_cache_->color_palette()->SetPalette(image);
      image.save(kSavedPaletteLocation);
    }
  }
}

void ActionHandler::SavePalette() const {
  QString output = QFileDialog::getSaveFileName(pApp->main_window(),
                                                tr("Save palette image file as..."), ".", "PNG (*.png);;BMP (*.bmp);;JPG (*.jpg);;JPEG (*.jpeg);;GIF (*.gif);;GIF (*.gif);;PBM (*.pbm);;PGM (*.pgm);;PPM (*.ppm);;TIFF (*.tiff);;XBM (*.xbm);;XPM (*.xpm)");
  if (!output.isEmpty()) {
    bool ok = window_cache_->color_palette()->palette()->save(output);
    if (ok) {
      QMessageBox::information(window_cache_, "Palette Saved.", "Palette file saved with success!");
    }
  }
}

void ActionHandler::DefaultPalette() const {
  QImage image = QImage(":/images/color_palette.png");
  window_cache_->color_palette()->SetPalette(image);
  window_cache_->color_palette()->repaint();
  image.save(kSavedPaletteLocation);
}

void ActionHandler::LoadSavedPalette() const {
  QImage image = QImage(kSavedPaletteLocation);
  if (!image.isNull()) {
    window_cache_->color_palette()->SetPalette(image);
    window_cache_->color_palette()->repaint();
  }
}

void ActionHandler::ToggleShowGrid(bool show) const {
  options_cache_->set_show_grid(show);
  window_cache_->edit_widget()->repaint();
}

void ActionHandler::ToggleShowPixelGrid(bool show) const {
  options_cache_->set_show_pixel_grid(show);
  window_cache_->edit_widget()->repaint();
}

void ActionHandler::ImageSize() const {
  ImageCanvasContainer *c = window_cache_->current_canvas_container();
  if (c == nullptr)
    return;
  QImage old_image = c->GetCanvasWidget()->image();
  QSize size = old_image.size();

  ResizeImageDialog dialog(size, window_cache_);
  int res = dialog.exec();

  if (res == QDialog::Accepted) {
    QSize new_size = dialog.new_size();
    QImage new_image = QImage(new_size, QImage::Format_ARGB32);
    new_image.fill(options_cache_->alt_color());
    QPainter p(&new_image);
    p.drawImage(old_image.rect(), old_image);
    c->GetCanvasWidget()->SetImage(new_image);
  }
}

void ActionHandler::Translate(const QString &language) const {
  options_cache_->set_language(language);
  pApp->Translate(language);
}

void ActionHandler::CreateImageCanvas(const QImage &image, const QString &file_name) const {
  ImageCanvasContainer *canvas_container = new ImageCanvasContainer(image, file_name);
  QMdiArea *mdi = window_cache_->mdi_area();
  QMdiSubWindow *w = mdi->addSubWindow(canvas_container);
  QSize size = image.size() + QSize(50, 50);
  w->resize(size);
  w->show();
}
