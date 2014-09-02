#include <QLabel>
#include <QListView>
#include <QMessageBox>
#include <QSettings>
#include <QTreeWidgetItem>

#include "reknote.h"

#include "gitrepository.h"
#include "spike.h"
#include "spikestreemodel.h"

extern "C" {
#include <git2.h>
}

Reknote::Reknote() {
  ui.setupUi(this);
  
  sm_ = new SpikesTreeModel(this);
  //sm_->setItemPrototype(new SpikeTreeItem());
  ui.spikestreeview->setModel(sm_);
  ui.spikestreeview->expandAll();
  ui.spikestreeview->header()->hide();
  
  QIcon* icon = new QIcon(QIcon::fromTheme("document-save"));
  sbarIcon = new QLabel();
  sbarIcon->setPixmap(icon->pixmap(statusBar()->height()/2));
  statusBar()->addPermanentWidget(sbarIcon);
  sbarText = new QLabel("");
  statusBar()->addPermanentWidget(sbarText);
  
  ui.spikestreeview->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(ui.spikestreeview, &QAbstractItemView::customContextMenuRequested, this, &Reknote::spikestreeContextMenu);
// connect(ui.spikestreeview, &QAbstractItemView::activated, this, &Reknote::activated); //this doesnt work on my ubuntu machien
  connect(ui.spikestreeview, &QAbstractItemView::clicked, this, &Reknote::activated);
  
  connect(ui.actionAddSpike, &QAction::triggered, this, &Reknote::addSpike);
  connect(ui.actionDeleteSpike, &QAction::triggered, this, &Reknote::deleteSelectedSpike);
  connect(ui.actionDeleteNote, &QAction::triggered, this, &Reknote::deleteNote);

  
  //have better ui for when commited and when saving
  connect(sm_, &SpikesTreeModel::commit_waiting, this, &Reknote::commitWaiting);
  connect(sm_, &SpikesTreeModel::commit_done, this, &Reknote::commitFinished);
  sbarIcon->setVisible(false);

  sm_->load();

  //restore index
  QSettings s("Foo", "reknote");
  const int row = s.value("selected-spike-row").toInt();
  const int column = s.value("selected-spike-column").toInt();
  const QModelIndex i = sm_->index(row, column);
  ui.spikestreeview->setCurrentIndex(i);
  activated(i);
}

Reknote::~Reknote() {
  sm_->save();

  const QModelIndex i = ui.spikestreeview->selectionModel()->currentIndex();
  QSettings s("Foo", "reknote");
  s.setValue("selected-spike-row", i.row());
  s.setValue("selected-spike-column", i.column());

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
  ui.spikestreeview->setCurrentIndex(i->index());
}

void Reknote::deleteSelectedSpike() {  
  const QModelIndexList li = ui.spikestreeview->selectionModel()->selectedIndexes();
  if (li.isEmpty()) return;
  const QModelIndex i = li.first();
  if (!i.isValid()) return;  
  const SpikePtr spike = sm_->getPointerFromIndex(i);
  const QString question = QString("Do you want to delete Spike \"%1\",\n with directory \"%2\"").arg(
    spike->name()).arg(spike->dir());
  QMessageBox::StandardButton res = QMessageBox::question(this, "Do you want to delete?", question);
  if (res==QMessageBox::Yes) {
    sm_->removeItemAtIndex(i);
  }
}

void Reknote::deleteNote() {
  const QModelIndexList li = ui.noteView->selectionModel()->selectedIndexes();
  QAbstractItemModel* m = ui.noteView->model();
  for(const QModelIndex& i : li) {
    m->removeRow(i.row());
  }
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
  ui.noteView->setModel(p.data());
}

void Reknote::commitWaiting() {
  sbarIcon->setVisible(false);
  sbarText->setText("saved");
}

void Reknote::commitFinished() {
  sbarIcon->setVisible(true);
  sbarText->setText("");
}

