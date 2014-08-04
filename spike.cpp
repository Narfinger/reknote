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

#include <QDir>
#include <QTextStream>

#include "spike.h"


Qt::ItemFlags Spike::flags(const QModelIndex& index) const {
  if (!index.isValid()) return 0;
  return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsUserCheckable;
}

void Spike::save(const QModelIndex& index, const QString& filename) const {
  QFile f(filename);
  f.open(QIODevice::WriteOnly | QIODevice::Truncate);
  QTextStream out(&f);
  QDomDocument d("note");
  d.appendChild(constructElement(d, index));
}

void Spike::saveAll() const {
  //if (!dir_.exists()) dir_.mkdir(dir_.dirName());
  const QList<QStandardItem*> list = findItems("", Qt::MatchContains);
  int i = 0;
  for(const QStandardItem* it : list) {
    const QString fname = QString("%1/%2-node.xml").arg(dir_.absolutePath()).arg(i);
    save(it->index(), fname);
    i++;
  }
}


void Spike::load() {

}

void Spike::setDir(const QString& dir) {
  dir_ = QString(dir) + "/" + name_;
  
}

const QDomElement Spike::constructElement(QDomDocument& d, const QModelIndex& index) const {
  QDomElement e = d.createElement("note");
  e.setAttribute("checked", data(index, Qt::CheckStateRole).toBool());
 
  QDomText text = d.createTextNode(data(index, Qt::DisplayRole).toString());
  e.appendChild(text);
  return e;
}
