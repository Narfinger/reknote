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


Spike::Spike(QObject* parent) : QStandardItemModel(parent) {
  connect(this, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(itemChangedSlot(QStandardItem*)));
}

Qt::ItemFlags Spike::flags(const QModelIndex& index) const {
  if (!index.isValid()) return 0;
  return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsUserCheckable;
}

void Spike::save() const {
  qDebug() << "tt";
  QFile f(dir_.absolutePath() + "/spike.xml");
  if (!QDir(dir_.absolutePath()).exists()) {
      QDir().mkdir(dir_.absolutePath());
  }
  f.open(QIODevice::WriteOnly | QIODevice::Truncate);
  QTextStream out(&f);
  QDomDocument d("notes");
    const QList<QStandardItem*> list = findItems("", Qt::MatchContains);
  for(const QStandardItem* it : list) {
    const QDomElement e = constructElement(d, it->index());
    d.appendChild(e);
  }
  out << d.toString();
  out.flush();
  f.close();
}

void Spike::load() {
  QFile f(dir_.absolutePath() + "/spike.xml");
  f.open(QIODevice::ReadOnly);
  QTextStream out(&f);
  QDomDocument d;
  QString error;
  int errorline;
  int errorcolumn;
  d.setContent(&f, false, &error, &errorline, &errorcolumn);
  
  const QDomNodeList notelist = d.elementsByTagName("note");
  for(int i=0; i< notelist.size(); ++i)  {
    const QDomNode n = notelist.at(i);
    insertElement(n);
  }
  
}

void Spike::setName(const QString& name) {
  name_ = name;
  dir_ = QDir(QStandardPaths::standardLocations(QStandardPaths::DataLocation).at(0) + "/" + name);
}


void Spike::setDir(const QString& dir) {
  dir_ = QString(dir) + "/" + name_;
  
}

void Spike::itemChangedSlot(QStandardItem* item) {
  save();
}

const QDomElement Spike::constructElement(QDomDocument& d, const QModelIndex& index) const {
  QDomElement e = d.createElement("note");
  e.setAttribute("checked", data(index, Qt::CheckStateRole).toBool());
 
  QDomText text = d.createTextNode(data(index, Qt::DisplayRole).toString());
  e.appendChild(text);
  return e;
}

void Spike::insertElement ( const QDomNode& n ) {
  const QString checked = n.attributes().namedItem("checked").toAttr().value();
  const bool bchecked = checked=="checked" ? true: false;
  const QString text = n.firstChild().toText().nodeValue();
  QStandardItem* i = new QStandardItem(text);
  i->setData(bchecked, Qt::CheckStateRole);
}
