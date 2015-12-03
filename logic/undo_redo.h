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

#ifndef UNDO_REDO_H
#define UNDO_REDO_H

#include <QImage>

class UndoRedo {
public:
  UndoRedo();
//  ~UndoRedo();

  void Do(const QImage &img);
  QImage Undo();
  qint64 UndoTimestamp() const;
  QImage Redo();
  qint64 RedoTimestamp() const;

private:
  class UndoRedoStack{
  private:
    class UndoRedoState{
    public:
      QImage image;
      qint64 timestamp;
    };

  public:
    UndoRedoStack();
    QVector<UndoRedoState> data;
    int first;
    int last;

    void Push(const QImage &img, qint64 timestamp);
    qint64 Check() const;
    QImage Pop();
  };

  UndoRedoStack undo;
  UndoRedoStack redo;
};

#endif // UNDO_REDO_H
