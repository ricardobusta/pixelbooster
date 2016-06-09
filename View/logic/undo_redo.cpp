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

#include "undo_redo.h"

#include <QDateTime>

#include "utils/debug.h"

const int kUndoRedoSize = 12;

UndoRedo::UndoRedo() {}

void UndoRedo::Do(const QImage &img) {
  undo.Push(img, QDateTime::currentMSecsSinceEpoch());
  redo.Clear();
}

QImage UndoRedo::Undo(const QImage &current) {
  if (!undo.IsEmpty()) {
    redo.Push(current, QDateTime::currentMSecsSinceEpoch());
  }
  QImage u = undo.Pop();
  return u;
}

qint64 UndoRedo::UndoTimestamp() const {
  return undo.Check();
}

QImage UndoRedo::Redo(const QImage &current) {
  if (!redo.IsEmpty()) {
    undo.Push(current, QDateTime::currentMSecsSinceEpoch());
  }
  QImage r = redo.Pop();
  return r;
}

qint64 UndoRedo::RedoTimestamp() const {
  return redo.Check();
}

UndoRedo::UndoRedoStack::UndoRedoStack() : first(-1),
                                           last(0) {
  data.resize(kUndoRedoSize);
}

bool UndoRedo::UndoRedoStack::IsEmpty() {
  return (first == -1);
}

void UndoRedo::UndoRedoStack::Clear() {
  first = -1;
}

void UndoRedo::UndoRedoStack::Push(const QImage &img, qint64 timestamp) {
  if (first == -1) {
    // Only one element at the stack.
    first = last;
  } else {
    // Multiple elements at the stack. The last element must be deleted if full.
    first++;
    first %= data.length();
    if (first == last) {
      last++;
      last %= data.length();
    }
  }
  data[first].image = img;
  data[first].timestamp = timestamp;
}

qint64 UndoRedo::UndoRedoStack::Check() const {
  return data[first].timestamp;
}

QImage UndoRedo::UndoRedoStack::Pop() {
  if (first == -1) {
    // Empty stack;
    return QImage(0, 0, QImage::Format_Invalid);
  } else if (first == last) {
    first = -1;
    return data[last].image;
  } else {
    int out = first;
    first--;
    first = first < 0 ? data.length() - 1 : first;
    return data[out].image;
  }
}
