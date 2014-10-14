/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright 2014  Narfinger.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef NOTEVIEW_H
#define NOTEVIEW_H

#include <QListView>
#include <QMouseEvent>
#include <QUndoStack>
class QStandardItem;
class NoteView : public QListView {
    Q_OBJECT

public:
  NoteView(QWidget* parent = 0);
  ~NoteView();

  void mouseDoubleClickEvent(QMouseEvent* event);
  void noteContextMenu(const QPoint&);
  void deleteNote();

  QUndoStack ustack_;	//tmp public

signals:
  void stopCommitTimer();

public slots:
  /** we kind of cheat with the whole undo command stack, our stuff will not support a redo as getting all needed information
    into the QUndoCommands seems to be difficult and a nightmare of who onws what, so we just clear if you every want to redo
    */
  void undo() { ustack_.undo();};
  void redo() { ustack_.clear(); };

private:
  void openFile(const QModelIndex& index);
  void setColor(const QModelIndex& index);
  void resetColor(const QModelIndex& index);
};

#endif // NOTEVIEW_H
