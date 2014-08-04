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


Qt::ItemFlags Spike::flags ( const QModelIndex& index ) const {
  if (!index.isValid()) return 0;
  return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsUserCheckable;
}

void Spike::save() const {
  //if (!dir_.exists()) dir_.mkdir(dir_.dirName());
  
  const QList<QStandardItem*> list = findItems("", Qt::MatchContains);
  
  int i = 0;
  const int length = 2; //FIXME this is obviously wrong
  for(const QStandardItem* it : list) {
    const QString fname = QString("%1/%2-node.xml").arg(dir_.absolutePath()).arg(i, length, 'i', 0, '0');
    QFile f(fname);
    f.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream out(&f);
    QDomDocument d("note");
    d.appendChild(saveElement(d, it));    
    
    i++;
  }

}

void Spike::load() {

}

void Spike::setDir(const QString& dir) {
  dir_ = QString(dir) + "/" + name_;
  
}


QDomElement Spike::saveElement(QDomDocument& d, const QStandardItem* it ) const {
  QDomElement e = d.createElement("note");
  e.setAttribute("checked", it->data(Qt::CheckStateRole).toBool());
 
  QDomText text = d.createTextNode(it->data(Qt::DisplayRole).toString());
  e.appendChild(text);
  return e;
}

