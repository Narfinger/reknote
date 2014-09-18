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

#include "noteview.h"
#include "noteitemdelegate.h"
#include "spike.h"

#include <QStandardItemModel>
#include <QDesktopServices>
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
  }
}

void NoteView::noteContextMenu(const QPoint& p) {
  const QModelIndex i = indexAt(p);
  const QStandardItem* it = dynamic_cast<QStandardItemModel*>(model())->itemFromIndex(i);
  if (i.isValid()) {
    if (!i.data(Spike::filepathrole).isNull())
      openFile(it);
  }
}

void NoteView::openFile(const QStandardItem* it) {
  const QString filepath = QStandardPaths::standardLocations(QStandardPaths::DataLocation).at(0) + 
	    "/" + it->data(Spike::filepathrole).toString();
  if (!filepath.isEmpty()) {
    QDesktopServices::openUrl(QUrl::fromLocalFile(filepath));
  }
}
