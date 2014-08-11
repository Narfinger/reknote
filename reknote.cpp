#include <QTreeWidgetItem>
#include <QListView>

#include "reknote.h"

#include "gitrepository.h"
#include "spike.h"
#include "spikestreemodel.h"

extern "C" {
#include <git2.h>
}

Reknote::Reknote() : repo_(new GitRepository(QStandardPaths::standardLocations(QStandardPaths::DataLocation).at(0))) {
  ui.setupUi(this);
  
  sm_ = new SpikesTreeModel(this);
  //sm_->setItemPrototype(new SpikeTreeItem());
  ui.spikestreeview->setModel(sm_);
  ui.spikestreeview->expandAll();
  ui.spikestreeview->header()->hide();
  ui.spikestreeview->setDragDropMode(QAbstractItemView::InternalMove);
  
  ui.spikestreeview->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(ui.spikestreeview, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(spikestreeContextMenu(const QPoint&)));
  connect(ui.actionAddSpike, SIGNAL(triggered()), this, SLOT(addSpike()));
  connect(ui.actionDeleteSpike, SIGNAL(triggered()), this, SLOT(deleteSelectedSpike()));
  connect(ui.loadbutton, SIGNAL(pressed()), sm_, SLOT(load()));
  connect(ui.savebutton, SIGNAL(pressed()), sm_, SLOT(save()));
  
  connect(ui.spikestreeview, SIGNAL(activated(QModelIndex)), this, SLOT(activated(QModelIndex)));
  
  //have better ui for when commited and when saving
  connect(sm_, &SpikesTreeModel::commit_waiting, [=]() { statusBar()->showMessage("saved but not commited"); });
  connect(sm_, &SpikesTreeModel::commit_done, [=]() { statusBar()->showMessage("commited"); });

  
  
  
  
  
  
  //QIcon* icon = new QIcon(QIcon::fromTheme("document-save"));
  QIcon* icon = new QIcon(QIcon::fromTheme("document-save"));
  QLabel* l = new QLabel();
  l->setPixmap(icon->pixmap(statusBar()->height()/2));
  statusBar()->addPermanentWidget(l,1);
  //l->setVisible(true);
  sm_->load();
}

Reknote::~Reknote() {
  sm_->save();
  delete sm_;
}

void Reknote::addSpike() {
  QStandardItem* i = new QStandardItem("Edit Text");
  SpikePtr s(new Spike());
  sm_->appendRow(i, s);
  ui.spikestreeview->setCurrentIndex(i->index());
  ui.spikestreeview->edit(i->index());
  statusBar()->showMessage("Added", 1*1000);
  
  ui.spikestreeview->selectionModel()->select(i->index(), QItemSelectionModel::Clear);
  activated(i->index()); //i don't know why this is necessary
}

void Reknote::deleteSelectedSpike() {
  const QModelIndexList li = ui.spikestreeview->selectionModel()->selectedIndexes();
  if (li.isEmpty()) return;
  const QModelIndex i = li.first();
  if (!i.isValid()) return;
  sm_->removeItemAtIndex(i);
}

void Reknote::spikestreeContextMenu(const QPoint& point) const {
  QMenu m(ui.spikestreeview);
  m.addAction(ui.actionAddSpike);
  m.addAction(ui.actionDeleteSpike);
  
  ui.actionDeleteSpike->blockSignals(true);	//we need to do something special for delete
  QAction* a = m.exec(ui.spikestreeview->mapToGlobal(point));
  if (a == ui.actionDeleteSpike) {
    QModelIndex i = ui.spikestreeview->indexAt(point);
    if (!i.isValid()) return;
    sm_->removeItemAtIndex(i);
  }
  ui.actionDeleteSpike->blockSignals(false);
}

void Reknote::activated(QModelIndex i) {
  const SpikePtr p = sm_->getPointerFromIndex(i);
  ui.listView->setModel(p.data());
}
