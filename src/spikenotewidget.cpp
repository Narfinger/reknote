/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2015  Christian Engels <engels@cs.uni-saarland.de>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
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

#include <QMenu>
#include <QMessageBox>

#include "spikenotewidget.h"

#include "gitrepository.h"
#include "spike.h"
#include "spikestreemodel.h"

SpikeNoteWidget::SpikeNoteWidget(QWidget* parent) : QWidget(parent) {
  ui_.setupUi(this);

  sm_ = new SpikesTreeModel(this);

  ui_.spikestreeview->setModel(sm_);
  ui_.spikestreeview->expandAll();
  ui_.spikestreeview->header()->hide();
  connect(ui_.noteView, &NoteView::stopCommitTimer, sm_, &SpikesTreeModel::stopCommitTimer);
  connect(ui_.spikestreeview, &QAbstractItemView::customContextMenuRequested, this, &SpikeNoteWidget::spikestreeContextMenu);

  ui_.spikestreeview->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(ui_.spikestreeview, &QAbstractItemView::clicked, this, &SpikeNoteWidget::activated);  

  //restore index
  QSettings s("Foo", "reknote");
  const int row = s.value("selected-spike-row").toInt();
  const int column = s.value("selected-spike-column").toInt();
  const QModelIndex i = sm_->index(row, column);
  selectIndex(i);

  sm_->load();
}

SpikeNoteWidget::~SpikeNoteWidget() {
  sm_->save();

  const QModelIndex i = ui_.spikestreeview->selectionModel()->currentIndex();
  QSettings s("Foo", "reknote");
  s.setValue("selected-spike-row", i.row());
  s.setValue("selected-spike-column", i.column());

  delete sm_;
}

void SpikeNoteWidget::addSpike() {
  QStandardItem* i = new QStandardItem("Edit Text");
  SpikePtr s(new Spike());
  sm_->appendRow(i, s);
  ui_.spikestreeview->setCurrentIndex(i->index());
  ui_.spikestreeview->edit(i->index());

  selectIndex(i->index());
}

void SpikeNoteWidget::deleteSelectedSpike() {  
  const QModelIndexList li = ui_.spikestreeview->selectionModel()->selectedIndexes();
  if (li.isEmpty()) return;
  const QModelIndex i = li.first();
  if (!i.isValid()) return;  
  const SpikePtr spike = sm_->getPointerFromIndex(i);
  const QString question = QString("Do you want to delete Spike \"%1\",\n with directory \"%2\"").arg(
    spike->name()).arg(spike->dir());
  QMessageBox::StandardButton res = QMessageBox::question(this, "Do you want to delete?", question);
  if (res==QMessageBox::Yes) {
    sm_->removeItemAtIndex(i);

    const QModelIndex i = ui_.spikestreeview->selectionModel()->selectedIndexes().first();
    selectIndex(i);
  }
}

void SpikeNoteWidget::deleteNote() {
  ui_.noteView->deleteNote();
}

void SpikeNoteWidget::cleanDone() {
  sm_->cleanDone();
}

void SpikeNoteWidget::spikestreeContextMenu(const QPoint& point) const {
  QMenu m(ui_.spikestreeview);
  m.addActions(al_);
  m.exec(ui_.spikestreeview->mapToGlobal(point));	//we don't need to call the action because it is called from main ui
}

void SpikeNoteWidget::setReadOnly(const bool r) {
  ui_.noteView->setDragEnabled(false);
  ui_.spikestreeview->setDragEnabled(r);
  if (r) {
    ui_.noteView->setDragDropMode(QAbstractItemView::NoDragDrop);
    ui_.spikestreeview->setDragDropMode(QAbstractItemView::NoDragDrop);
  } else {
    ui_.noteView->setDragDropMode(QAbstractItemView::DragDrop);
    ui_.spikestreeview->setDragDropMode(QAbstractItemView::DragDrop);
  }
}


void SpikeNoteWidget::selectIndex(const QModelIndex& index) {
  ui_.spikestreeview->setCurrentIndex(index);
  activated(index);
}

void SpikeNoteWidget::activated(QModelIndex i) {
  if (sm_->rowCount()==0) return;
  const SpikePtr p = sm_->getPointerFromIndex(i);
  ui_.noteView->setModel(p.data());
}
