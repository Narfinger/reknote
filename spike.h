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

#ifndef SPIKE_H
#define SPIKE_H

#include <QDir>
#include <QSharedPointer>
#include <QStandardItemModel>
#include <QString>
#include <QUuid>
#include <QtXml/QtXml>

class Spike : public QStandardItemModel {
    Q_OBJECT
public:
  Spike(QObject* parent = 0): QStandardItemModel(parent) {};
  Qt::ItemFlags flags(const QModelIndex &index) const;
  
  void save() const;
  void load();
  
  void setName(const QString& name) { name_ = name; };
  void setDir(const QString& dir);

private:
  QString name_;
  QUuid id_;
  QDir dir_;
  
  QDomElement saveElement(QDomDocument& d, const QStandardItem* it) const;
  
};

//typedef QSharedPointer<Spike> SpikePtr;
typedef QSharedPointer<Spike> SpikePtr;

#endif // SPIKE_H
