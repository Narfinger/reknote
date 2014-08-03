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
#include <qt4/QtCore/qshareddata.h>

#include "spikestreemodel.h"

const int SpikesTreeModel::mid = Qt::UserRole + 1;

SpikesTreeModel::SpikesTreeModel(QObject* parent) : QStandardItemModel(parent) {
  QStandardItem* parentItem = this->invisibleRootItem();
  /*for (int i=0; i<4; ++i) {
    QStandardItem* item = new QStandardItem(QString("item %1").arg(i));
    QStandardItem* item2 = new QStandardItem(QString("TESTitem %1").arg(i));
    parentItem->appendRow(item);
    parentItem->appendRow(item2);
    parentItem = item;
  }*/
}

SpikesTreeModel::~SpikesTreeModel() {
}

void SpikesTreeModel::load() {
  QString path = QStandardPaths::standardLocations(QStandardPaths::DataLocation).at(0);
  QFile file(path+"/spikestree.xml");
  if (!file.exists()) return;
  
  file.open(QIODevice::ReadOnly);
  QTextStream out;
  out.setDevice(&file);
  
  QDomDocument doc;
  QString error;
  int errorline;
  int errorcolumn;
  doc.setContent(&file, false, &error, &errorline, &errorcolumn);
  QDomNodeList spikelist = doc.elementsByTagName("spikes").at(0).childNodes();
  
  qDebug() << spikelist.at(0).nodeName();
  loadXml(spikelist, invisibleRootItem());
  
  out.flush();
  file.close();
}

void SpikesTreeModel::loadXml(QDomNodeList& list, QStandardItem* parentItem) {
  for(int i=0; i< list.size(); ++i) {
    const QDomNode node(list.at(i));
    QString text = node.attributes().namedItem("name").toAttr().value();
    QStandardItem* it = new QStandardItem(text);
    parentItem->appendRow(it);
    
    if( node.hasChildNodes()) {
      QDomNodeList children = node.childNodes();
      loadXml(children, it);
    }
  }
}

void SpikesTreeModel::save() {
  QString path = QStandardPaths::standardLocations(QStandardPaths::DataLocation).at(0);
  QDir dir(path);
  if (!dir.exists()) QDir().mkdir(path);
  QFile file(path +"/spikestree.xml");
  file.open(QIODevice::WriteOnly | QIODevice::Truncate);
  QTextStream out(&file);
    
  QDomDocument d("spikes");
  QDomElement e = d.createElement("spikes");
  saveChildrenToXml(d, e, invisibleRootItem());
  d.appendChild(e);
  
  out << d.toString();
  out.flush();  
  file.close();
}

void SpikesTreeModel::saveChildrenToXml(QDomDocument& d, QDomElement& elem, QStandardItem* item) {
  if (item->hasChildren()) {
    for(int i=0; i< item->rowCount(); ++i) {
      QStandardItem* it = item->child(i);
      QDomElement e = d.createElement("spike");
      e.setAttribute("name", it->text());
      elem.appendChild(e);
      saveChildrenToXml(d, e, item->child(i));
    }
  }
}

void SpikesTreeModel::appendRow(QStandardItem* i, SpikePtr p) {
  i->setData(s_.size(), SpikesTreeModel::mid);
  s_.push_back(p);
  QStandardItemModel::appendRow(i);
}

SpikePtr SpikesTreeModel::getPointerFromIndex(const QModelIndex& index) const {
  const int r = data(index, SpikesTreeModel::mid).toInt();
  return s_.at(r);
}

Qt::ItemFlags SpikesTreeModel::flags(const QModelIndex &index) const {
  if (!index.isValid()) return 0;
  return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEditable;
  
}