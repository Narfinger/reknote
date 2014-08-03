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

#ifndef SPIKESTREEMODEL_H
#define SPIKESTREEMODEL_H

#include <QtXml/QDomNodeList>
#include <QStandardItemModel>
#include <QStandardPaths>
#include <QtXml/QtXml>

#include "spike.h"

class SpikesTreeModel : public QStandardItemModel
{
    Q_OBJECT

public:  
    SpikesTreeModel(QObject *parent = 0);
    ~SpikesTreeModel();
   
    void loadXml(QDomNodeList& list, QStandardItem* parentItem); 
    void saveChildrenToXml(QDomDocument& d, QDomElement& elem, QStandardItem* item);
    void appendRow(QStandardItem* i, SpikePtr p);
    void removeItemAtIndex(const QModelIndex& index);
    SpikePtr getPointerFromIndex(const QModelIndex& index) const;
    
    
    Qt::ItemFlags flags(const QModelIndex &index) const;
    Qt::DropActions supportedDropActions() const { return Qt::MoveAction; };  
    
    QList<SpikePtr> s_;
    static const int mid;
    
public slots:
  void load();
  void save();
};

#endif // SPIKESTREEMODEL_H
