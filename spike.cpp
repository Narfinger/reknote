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
#include <QFileIconProvider>
#include <QMimeData>
#include <QMimeDatabase>
#include <QTextStream>

#include "spike.h"

const int Spike::maxdirname = 10;
const int Spike::filepathrole = Qt::UserRole + 1;

Spike::Spike(QObject* parent) : QStandardItemModel(parent), dir_(QDir::root()) {
  setupSignals();
}


Spike::Spike(const QString& reldirandname, QObject* parent) : QStandardItemModel(parent), dir_(QDir::root()) {
  if (reldirandname.isEmpty()) {
    qDebug() << "Warning, empty reldirandname, aborting everything";
    return;
  }
  setRelDirAndName(reldirandname);
  load();
  setupSignals(); //add signal if we finished loading or are not loading anything
}

Qt::ItemFlags Spike::flags(const QModelIndex& index) const {
  if (!index.isValid()) return Qt::ItemIsDropEnabled;
  return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsUserCheckable 
    |Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
}

void Spike::save() const {
  qDebug() << "saved";
  QFile f(dir_.absolutePath() + "/spike.xml");
  if (!QDir(dir_.absolutePath()).exists()) {
      QDir().mkdir(dir_.absolutePath());
  }
  f.open(QIODevice::WriteOnly | QIODevice::Truncate);
  QTextStream out(&f);
  QDomDocument d("notes");
  const QList<QStandardItem*> list = findItems("", Qt::MatchContains);
  QDomNode root = d.createElement("notes");
  d.appendChild(root);
  for(const QStandardItem* it : list) {
    const QDomElement e = constructElement(d, it->index());
    root.appendChild(e);
  }
  out << d.toString();
  out.flush();
  f.close();
  emit saved();
}

void Spike::load() {
  QFile f(dir_.absolutePath() + "/spike.xml");
  qDebug() << "Loaindg spike";
  f.open(QIODevice::ReadOnly);
  QTextStream out(&f);
  QDomDocument d;
  QString error;
  int errorline;
  int errorcolumn;
  d.setContent(&f, false, &error, &errorline, &errorcolumn);
  if (!error.isEmpty()) { qDebug() << "error loading file" << dir_.absolutePath(); return; }
  
  const QDomNodeList notelist = d.elementsByTagName("note");
  for(int i=0; i< notelist.size(); ++i)  {
    const QDomNode n = notelist.at(i);
    insertElement(n);
  }
}

void Spike::setName(const QString& name) {
  name_ = name;
}

void Spike::setRelativeDir(QString dir) {
  dir.truncate(Spike::maxdirname);
  const QString newdirname = QStandardPaths::standardLocations(QStandardPaths::DataLocation).at(0) + "/" + dir;
  if (dir_.isRoot())
    dir_.setPath(newdirname); 
  else {
    dir_.rename(dir_.absolutePath(), newdirname);
  }
}

bool Spike::removeRows(int position, int rows, const QModelIndex& parent) {
  QStandardItem* it = itemFromIndex(parent);
  for (int i = position + rows - 1; i >= position; i--) {
    QModelIndex child = index(i, 0, parent);
    const QString filepath = child.data(filepathrole).toString();
    if (!filepath.isEmpty()) {
      QFile f(filepath);
      f.remove();
      deletedfiles_.append(filepath);
    }
  }
  bool s = QStandardItemModel::removeRows(position, rows, parent);
  save();
  return s;
}

bool Spike::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) {
  if (action == Qt::IgnoreAction) return true;
  if (action == Qt::MoveAction) return QStandardItemModel::dropMimeData(data, action, row, column ,parent);

  if (data->hasFormat("text/uri-list")) {
    const QString d(data->data("text/uri-list"));
    const QUrl u(d);
    QFile f(u.toLocalFile());
    const QFileInfo fi(f);
    const QString newfilepath = dir_.path() + "/"+ fi.fileName();
    const QFileInfo nfi(newfilepath);
    if (nfi.exists()) return false;
    const bool res = f.copy(newfilepath);
    if (!res) return false;

    //const QString itemstring = QString("<a href='file://%1'>%2</a>").arg(newfilepath).arg(fi.fileName());
    QStandardItem* it = new QStandardItem("Edit Text");
    it->setData(newfilepath, filepathrole);
    it->setIcon(iconFromFilepath(newfilepath));
    it->setCheckable(true);
    appendRow(it);
    return true;
  }
  return false;
}

void Spike::setupSignals() {
  connect(this, &Spike::itemChanged,  this, &Spike::save);
  connect(this, &Spike::rowsInserted, this, &Spike::save);
  //connect(this, &Spike::rowsRemoved,  this, &Spike::save);
  //connect(this, &Spike::rowsAboutToBeRemoved, this, &Spike::removed);
}

const QDomElement Spike::constructElement(QDomDocument& d, const QModelIndex& index) const {
  QDomElement e = d.createElement("note");
  e.setAttribute("checked", data(index, Qt::CheckStateRole).toBool());
  e.setAttribute("file", data(index, filepathrole).toString());

  QDomText text = d.createTextNode(data(index, Qt::DisplayRole).toString());
  e.appendChild(text);
  return e;
}

void Spike::insertElement(const QDomNode& n) {
  const QString checked = n.attributes().namedItem("checked").toAttr().value();
  const Qt::CheckState bchecked = checked=="1" ? Qt::Checked: Qt::Unchecked;

  const QString filepath = n.attributes().namedItem("file").toAttr().value();

  const QString text = n.firstChild().toText().nodeValue();
  QStandardItem* i = new QStandardItem(text);
  i->setCheckState(bchecked);
  i->setData(filepath, filepathrole);
  if(!filepath.isEmpty()) {
    i->setIcon(iconFromFilepath(filepath));
  }

  appendRow(i);
}

QIcon Spike::iconFromFilepath(const QString& fp) const {
  const QMimeDatabase db;
  const QMimeType mt = db.mimeTypeForFile(fp);
  qDebug() << "called for" << fp << db.mimeTypeForFile(fp).iconName() << mt.genericIconName();
  return QIcon::fromTheme(mt.genericIconName());
  //return QIcon::fromTheme(db.mimeTypeForFile(fp).iconName());
}
