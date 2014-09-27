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

#ifndef NOTECOMMANDS_H
#define NOTECOMMANDS_H

#include <QDebug>
#include <QModelIndex>
#include <QStandardItemModel>
#include <QUndoCommand>
#include <QVariant>

#include <memory>

class QStandardItem;
class Spike;

class NoteCommandAdd : public QUndoCommand {
public:
    NoteCommandAdd() {};
    NoteCommandAdd(QStandardItem* item, QStandardItemModel* spike);
    ~NoteCommandAdd() {};
    virtual void redo() {};
    virtual void undo() { spike_->removeRow(index_.row()); };

private:
  Q_DISABLE_COPY(NoteCommandAdd)
  QModelIndex index_;
  QStandardItemModel* spike_;
};

class NoteCommandDelete : public QUndoCommand {
public:
    NoteCommandDelete() {};
    NoteCommandDelete(const QStandardItem* item, const QModelIndex& index, QStandardItemModel* spike);
    ~NoteCommandDelete() {};
    virtual void redo() {};
    virtual void undo() { spike_->insertRow(index_.row(), item_.release()); };

private:
  Q_DISABLE_COPY(NoteCommandDelete)
  QModelIndex index_;
  QStandardItemModel* spike_;
  std::unique_ptr<QStandardItem> item_;
};

#endif // NOTECOMMANDADD_H
