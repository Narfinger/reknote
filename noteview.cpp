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

#include "notecommands.h"
#include "noteview.h"
#include "noteitemdelegate.h"
#include "spike.h"

#include <QAction>
#include <QColorDialog>
#include <QStandardItemModel>
#include <QDesktopServices>
#include <QMenu>
#include <QMimeData>
#include <QtDebug>
#include <QUrl>

NoteView::NoteView(QWidget* parent): QListView(parent) {
  setItemDelegate(new NoteItemDelegate(this));
  setContextMenuPolicy(Qt::CustomContextMenu);
  connect(this, &QAbstractItemView::customContextMenuRequested, this, &NoteView::noteContextMenu);
}

NoteView::~NoteView() {
}

void NoteView::mouseDoubleClickEvent(QMouseEvent* event) {
  QPoint p = event->pos();
  if (indexAt(p).isValid()) {
    QListView::mouseDoubleClickEvent(event);
  } else {
    QStandardItemModel* m = dynamic_cast<QStandardItemModel*>(model());
    if (m==nullptr) {	//no model found
      QListView::mouseDoubleClickEvent(event);  
      return;
    }

    QStandardItem* t = new QStandardItem("enter text");
    t->setCheckable(true);
    m->appendRow(t);
    QModelIndex ni = t->index();
    edit(ni);

    QUndoCommand* addcommand = new NoteCommandAdd(t, m);
    ustack_.push(addcommand);
  }
}

void NoteView::noteContextMenu(const QPoint& p) {
  const QModelIndex i = indexAt(p);
  if (i.isValid()) {
    QMenu* menu = new QMenu();
    if (!i.data(Spike::filepathrole).isNull()) {
      QAction* act = new QAction("Open", menu);
      connect(act, &QAction::triggered, [=]() { openFile(i); });
      menu->addAction(act);
    }

    { //color
      QAction* act = new QAction("Set Color", menu);
      connect(act, &QAction::triggered, [=]() { setColor(i); });
      menu->addAction(act);

      QAction* res = new QAction("Reset Color", menu);
      connect(res, &QAction::triggered, [=]() { resetColor(i); });
      menu->addAction(res);
    }

    menu->exec(this->mapToGlobal(p));
  }
}

void NoteView::deleteNote() {
  const QModelIndexList li = selectionModel()->selectedIndexes();
  QStandardItemModel* m = dynamic_cast<QStandardItemModel*>(model());
  for(const QModelIndex& i : li) {
    const QStandardItem* it = m->itemFromIndex(i);
    QUndoCommand* undo = new NoteCommandDelete(it, i, m);

    m->removeRow(i.row());
    ustack_.push(undo);
  }
}

void NoteView::openFile(const QModelIndex& index) {
  const QString filepath = QStandardPaths::standardLocations(QStandardPaths::DataLocation).at(0) + 
	    "/" + index.data(Spike::filepathrole).toString();
  if (!filepath.isEmpty()) {
    QDesktopServices::openUrl(QUrl::fromLocalFile(filepath));
  }
}

void NoteView::setColor(const QModelIndex& index) {
  QColor defcol = Qt::white;
  const QVariant colvar = model()->data(index, Spike::colorrole);
  if (colvar.isValid())
    defcol.setNamedColor(colvar.toString());

  const QColor col = QColorDialog::getColor(defcol, this, "Select Color for item");
  if (col.isValid()) {
    Spike* spike = dynamic_cast<Spike*>(model());
    if (spike!=nullptr)
      spike->setColor(index, col);
  }
}

void NoteView::resetColor(const QModelIndex& index) {
  Spike* spike = dynamic_cast<Spike*>(model());
  if (spike!=nullptr)
    spike->resetColor(index);
}
