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

#include <QSet>
#include <QStandardItemModel>
#include <QStandardPaths>
#include <QTimer>
#include <QtXml/QtXml>
#include <QtXml/QDomNodeList>

#include "spike.h"

class GitRepository;

class SpikesTreeModel : public QStandardItemModel
{
    Q_OBJECT

public:  
    SpikesTreeModel(QObject *parent = nullptr);
    ~SpikesTreeModel();

    void loadXml(QDomNodeList& list, QStandardItem* parentItem); 
    void saveChildrenToXml(QDomDocument& d, QDomElement& elem, QStandardItem* item) const;
    GitRepositoryPtr getGitRepositoryPtr() { return repo_; };

    void appendRow(QStandardItem* i, SpikePtr p);
    void removeItemAtIndex(const QModelIndex& index);
    const SpikePtr getPointerFromIndex(const QModelIndex& index) const;

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    Qt::DropActions supportedDropActions() const override { return Qt::MoveAction; };  
    QStringList mimeTypes() const override  { return QStringList("text/note"); };
    bool dropMimeData(const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent) override;
    
    static const int modelindexrole;
    
public slots:
  void itemChangedSlot(QStandardItem* item);

  void load();
  void save();
  void cleanDone();
  void stopCommitTimer() { committimer_.stop(); };
  
signals:
  void commit_waiting();
  void commit_done();
  
  
private:
  QList<SpikePtr> s_;
  GitRepositoryPtr repo_;
  QSet<SpikePtr> commit_;
  bool commitmodel_ = false;
  QTimer committimer_;
  static const int commmitinterval;
  
  void changed(const int index);
  void commit();
};

#endif // SPIKESTREEMODEL_H
