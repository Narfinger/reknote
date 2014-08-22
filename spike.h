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
  Spike(QObject* parent = 0);
  Qt::ItemFlags flags(const QModelIndex &index) const;
  Qt::DropActions supportedDragActions() const { return Qt::MoveAction; };
  Qt::DropActions supportedDropActions() const { return Qt::MoveAction | Qt::CopyAction; };
  
  void save() const;
  void load();
  
  const QString name() const { return name_; };
  void setName(const QString& name);
  const QString dir() const { return dir_.absolutePath(); };
  const QString dirName() const { return dir_.dirName(); };
  void setRelativeDir(QString dir);
  void setRelDirAndName(const QString& n) { setRelativeDir(n); setName(n); };
  QStringList mimeTypes() const  { return QStringList("text/plain"); };
  bool dropMimeData(const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent);
  
private slots:
  void itemChangedSlot(QStandardItem* item);

private:
  static const int maxdirname;
  
  QString name_;
  QUuid id_;
  QDir dir_;
  
  const QDomElement constructElement(QDomDocument& d, const QModelIndex& index) const;
  void insertElement(const QDomNode& n);
  
};

typedef QSharedPointer<Spike> SpikePtr;

#endif // SPIKE_H
