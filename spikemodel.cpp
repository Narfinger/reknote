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

#include "spikemodel.h"

SpikeModel::SpikeModel(QObject* parent) : QStandardItemModel(parent) {
  QList<QString> l1({"t1", "t2"});
  QList<QString> l2({"tt1", "tt2"});
  list_ << l1 << l2;
  QStandardItem* parentItem = this->invisibleRootItem();
  /*for (int i=0; i<4; ++i) {
    QStandardItem* item = new QStandardItem(QString("item %1").arg(i));
    QStandardItem* item2 = new QStandardItem(QString("TESTitem %1").arg(i));
    parentItem->appendRow(item);
    parentItem->appendRow(item2);
    parentItem = item;
  }*/
}

SpikeModel::~SpikeModel() {
}

void SpikeModel::load() {
  QString path = QStandardPaths::standardLocations(QStandardPaths::DataLocation).at(0);
  QFile xmlfile(path+"/spikes.xml");
}

void SpikeModel::loadXml(QDomNodeList& list, QStandardItem* parentItem) {
  for(int i= 0; i< list.size(); ++i) {
    const QDomNode node(list.at(i));
    QString text = node.attributes().namedItem("name").toText().data();
    QStandardItem* it = new QStandardItem(text);
    parentItem->appendRow(it);
    
    if( node.hasChildNodes()) {
      QDomNodeList children = node.childNodes();
      loadXml(children, it);
    }
  }
}

Qt::ItemFlags SpikeModel::flags(const QModelIndex &index) const {
  if (!index.isValid()) return 0;
  return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEditable;
  
}