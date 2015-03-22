#include <QLabel>
#include <QListView>

#include <QSettings>
#include <QTreeWidgetItem>
#include <QUndoView>


#include "historydialog.h"
#include "reknote.h"

#include "gitrepository.h"
#include "spike.h"
#include "spikestreemodel.h"

extern "C" {
#include <git2.h>
}

Reknote::Reknote() {
  ui.setupUi(this);

  QIcon* icon = new QIcon(QIcon::fromTheme("document-save"));
  sbarIcon = new QLabel();
  sbarIcon->setPixmap(icon->pixmap(statusBar()->height()/2));
  statusBar()->addPermanentWidget(sbarIcon);
  sbarText = new QLabel("");
  statusBar()->addPermanentWidget(sbarText);

  //connect(ui.actionUndo, &QAction::triggered, ui.noteView, &NoteView::undo);

  connect(ui.actionAddSpike, &QAction::triggered,    ui.spikenotewidget, &SpikeNoteWidget::addSpike);
  connect(ui.actionDeleteSpike, &QAction::triggered, ui.spikenotewidget, &SpikeNoteWidget::deleteSelectedSpike);
  connect(ui.actionDeleteNote, &QAction::triggered,  ui.spikenotewidget, &SpikeNoteWidget::deleteNote);
  connect(ui.actionCleanDone, &QAction::triggered,   ui.spikenotewidget, &SpikeNoteWidget::cleanDone);

  connect(ui.actionHistory, &QAction::triggered, this, &Reknote::showHistory);

  //have better ui for when commited and when savingkolk
  connect(ui.spikenotewidget->spikestreemodel(), &SpikesTreeModel::commit_waiting, this, &Reknote::commitWaiting);
  connect(ui.spikenotewidget->spikestreemodel(), &SpikesTreeModel::commit_done, this, &Reknote::commitFinished);
  sbarIcon->setVisible(false);
}

Reknote::~Reknote() {
}

void Reknote::showHistory() {
  /*const QModelIndexList li = ui.spikestreeview->selectionModel()->selectedIndexes();
  if (li.isEmpty()) return;*/
  //const QModelIndex i = li.first();
  //if (!i.isValid()) return;
  HistoryDialog d(ui.spikenotewidget->spikestreemodel()->getGitRepositoryPtr(), this);
  int result = d.exec();
}


void Reknote::commitWaiting() {
  sbarIcon->setVisible(false);
  sbarText->setText("saved");
}

void Reknote::commitFinished() {
  sbarIcon->setVisible(true);
  sbarText->setText("");
}
