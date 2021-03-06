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
#include <QByteArray>
#include <QDataStream>
#include <QTextStream>

#include "gitcommit.h"
#include "gitrepository.h"
#include "spikestreemodel.h"

const int SpikesTreeModel::modelindexrole = Qt::UserRole + 1;
const int SpikesTreeModel::commmitinterval = 10*1000;

SpikesTreeModel::SpikesTreeModel(QObject* parent) : QStandardItemModel(parent),
  repo_(new GitRepository(QStandardPaths::standardLocations(QStandardPaths::DataLocation).at(0))) {
  connect(this, &SpikesTreeModel::itemChanged, this, &SpikesTreeModel::itemChangedSlot);
  
  //setup commit times
  committimer_.setSingleShot(true);
  connect(&committimer_, &QTimer::timeout, this, &SpikesTreeModel::commit);
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

  loadXml(spikelist, invisibleRootItem());

  out.flush();
  file.close();

  connect(this, &SpikesTreeModel::itemChanged, this, [=]() {this->changed(-1); }); //if we finished loading we can add this signal
}

void SpikesTreeModel::loadXml(const QDomNodeList& list, QStandardItem* parentItem) {
  for(int i=0; i< list.size(); ++i) {
    const QDomNode node(list.at(i));
    QString name = node.attributes().namedItem("name").toAttr().value();
    QStandardItem* it = new QStandardItem(name);
    parentItem->appendRow(it);
    SpikePtr p(new Spike(name));

    s_.append(p);
    const int pos = s_.size() - 1;
    setData(it->index(), s_.size() -1, SpikesTreeModel::modelindexrole);
    connect(p.data(), &Spike::saved, [=]() { this->changed(pos); });

    if( node.hasChildNodes()) {
      QDomNodeList children = node.childNodes();
      loadXml(children, it);
    }
  }
}

void SpikesTreeModel::loadGitCommit(const GitCommitPtr commit) {
  s_.clear();
  const QString spiketree = commit->file("spikestree.xml");

  QDomDocument doc;
  QString error;
  int errorline;
  int errorcolumn;
  doc.setContent(spiketree, false, &error, &errorline, &errorcolumn);
  QDomNodeList spikelist = doc.elementsByTagName("spikes").at(0).childNodes();
  loadXml(spikelist, invisibleRootItem());
  readOnly_ = true;

  //load spikes with correct data
  for (SpikePtr s: s_) {
    s->loadGitCommit(commit);
  }
}

void SpikesTreeModel::save() {
  if (readOnly_) return;
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

void SpikesTreeModel::saveChildrenToXml(QDomDocument& d, QDomElement& elem, QStandardItem* item) const {
  if (readOnly_) return;
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
  if (readOnly_) return;
  i->setData(s_.size(), SpikesTreeModel::modelindexrole);
  s_.push_back(p);
  QStandardItemModel::appendRow(i);
  const int pos = s_.size() -1;
  connect(p.data(), &Spike::itemChanged, [=]() { changed(pos); });
}

void SpikesTreeModel::removeItemAtIndex(const QModelIndex& index) {
  if (readOnly_) return;
  QStandardItem* it = itemFromIndex(index);
  QStandardItem* pit = it->parent();
  if(pit==nullptr) {
    pit = invisibleRootItem();
  }
  QModelIndex p = pit->index();
  int row = it->row();
  const int pos = data(index, SpikesTreeModel::modelindexrole).toInt();
  SpikePtr spike = s_[pos];
  QDir sdir(spike->dir());

  removeRow(row, p);
  sdir.removeRecursively();

  GitIndex rindex(repo_);
  rindex.removeSpike(spike);
  spike.reset(); //delete the model but keep the size the same
  save();
  rindex.add(*this);
  rindex.commit();
  emit commit_done();
}

const SpikePtr SpikesTreeModel::getPointerFromIndex(const QModelIndex& index) const {
  const int r = data(index, SpikesTreeModel::modelindexrole).toInt();
  return s_.at(r);
}

void SpikesTreeModel::itemChangedSlot(QStandardItem* item) {
  if (readOnly_) return;
  const int r = data(item->index(), SpikesTreeModel::modelindexrole).toInt();
  const SpikePtr s = s_.at(r);
  const QString name = data(item->index()).toString();
  s->setRelDirAndName(name);
  save();
}

Qt::ItemFlags SpikesTreeModel::flags(const QModelIndex &index) const {
  if (!index.isValid()) return Qt::ItemIsDropEnabled;
  return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled 
    | Qt::ItemIsDropEnabled | Qt::ItemIsEditable;
  
}

bool SpikesTreeModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) {
  if (readOnly_) return true;
  if (action == Qt::IgnoreAction) return true;
  if (data->hasFormat("text/note")) {
    if (row != -1 || column != -1) return false;  //otherwise the parent is not the correct index

    SpikePtr s = getPointerFromIndex(parent);

    QByteArray b = data->data("text/note");
    QDataStream stream(&b, QIODevice::ReadOnly);
    QList<QStandardItem*> l;
    while (!stream.atEnd()) {
      QStandardItem* item = new QStandardItem();
      stream >> *item;
      l << item;
    }
    s->appendRow(l);
    return true;
  }
  if (action == Qt::MoveAction) return QStandardItemModel::dropMimeData(data, action, row, column, parent);
  return false;
}


void SpikesTreeModel::cleanDone() {
  if (readOnly_) return;
  for(SpikePtr& p : s_) {
    p->cleanDone();
  }
}

void SpikesTreeModel::changed(const int index) {
  if (readOnly_) return;
  emit commit_waiting();
  if(index == -1) {
    commitmodel_ = true;
  } else {
    const SpikePtr p = s_.at(index);
    commit_.insert(p);
  }
  committimer_.setInterval(SpikesTreeModel::commmitinterval);
  committimer_.start();
}

void SpikesTreeModel::commit() {
  if (readOnly_) return;
  qDebug() << "commit" << "commit myself:" << commitmodel_ << "size" << commit_.size();
  GitIndex i(repo_);
  for(const SpikePtr& s : commit_) {
    i.add(s);
    i.removeFiles(s->deletedFiles());
    s->clearDeletedFiles();
  }
  if (commitmodel_) {
    i.add(*this);
  }
  i.commit();
  
  commitmodel_ = false;
  commit_.clear();
  emit commit_done();
}
