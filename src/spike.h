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
#include <QStringList>
#include <QUuid>
#include <QtXml/QtXml>

class GitCommit;

class Spike : public QStandardItemModel {
    Q_OBJECT
public:
  static const int filepathrole;
  static const int colorrole;

  Spike(QObject* parent = nullptr);
  Spike(const QString& reldirandname, QObject* parent = nullptr);	//loads the spike with reldirandname set
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  Qt::DropActions supportedDragActions() const override { return Qt::MoveAction; };
  Qt::DropActions supportedDropActions() const override { return Qt::MoveAction; };

  void load();
  void loadGitCommit(const QSharedPointer<GitCommit> commit);

  const QString name() const 			{ return name_; };
  void setName(const QString& name);
  const QString dir() const 			{ return dir_.path(); };
  const QString dirName() const 		{ return dir_.dirName(); };
  void setRelativeDir(QString dir);
  void setRelDirAndName(const QString& n) 	{ setRelativeDir(n); setName(n); };
  const QStringList& deletedFiles() const 	{ return deletedfiles_; };
  void clearDeletedFiles() 			{ deletedfiles_.clear(); };
  void setColor(const QModelIndex& index, const QColor& color) { setData(index, color, colorrole); };
  void resetColor(const QModelIndex& index);

  bool removeRows(int position, int rows, const QModelIndex& parent = QModelIndex()) override;
  QStringList mimeTypes() const override  { return QStringList("application/x-qabstractitemmodeldatalist") << "text/note"; };
  bool dropMimeData(const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent) override;
  QMimeData* mimeData(const QModelIndexList& indexes) const override;
  void cleanDone();
  int getTodo() const;

signals:
  void saved() const;
  void deletedFile(QString) const;

public slots:
  void save() const;

private:
  static const int maxdirname;
  
  QString name_;
  QUuid id_;
  QDir dir_;
  QStringList deletedfiles_;
  
  void setupSignals();
  const QDomElement constructElement(QDomDocument& d, const QModelIndex& index) const;
  void insertElement(const QDomNode& n);
  QIcon iconFromFilepath(const QString& fp) const;
};

typedef QSharedPointer<Spike> SpikePtr;

#endif // SPIKE_H
