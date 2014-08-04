#include <QTreeWidgetItem>
#include <QListView>

#include "reknote.h"
#include "spike.h"
#include "spikestreemodel.h"

Reknote::Reknote() {
  ui.setupUi(this);
  
  sm_ = new SpikesTreeModel(this);
  //sm_->setItemPrototype(new SpikeTreeItem());
  ui.treeView->setModel(sm_);
  ui.treeView->expandAll();
  ui.treeView->setDragDropMode(QAbstractItemView::InternalMove);
  connect(ui.pushButton, SIGNAL(pressed()), this, SLOT(tmpAdd()));
  connect(ui.treeView, SIGNAL(activated(QModelIndex)), this, SLOT(activated(QModelIndex)));
  
  connect(ui.loadbutton, SIGNAL(pressed()), sm_, SLOT(load()));
  connect(ui.savebutton, SIGNAL(pressed()), sm_, SLOT(save()));
  connect(ui.deleteb, SIGNAL(pressed()), this, SLOT(tmpDelete()));
  
  tmpi = 0;
  /*  QLabel* l = new QLabel( this );
    l->setText( "Hello World!" );
    setCentralWidget( l );
    QAction* a = new QAction(this);
    a->setText( "Quit" );
    connect(a, SIGNAL(triggered()), SLOT(close()) );
    menuBar()->addMenu( "File" )->addAction( a );
    */
}

Reknote::~Reknote() {
  delete sm_;
}

void Reknote::tmpAdd() {
  QStandardItem* i = new QStandardItem("tmp");
  
  //SpikePtr s( new Spike());
  
  SpikePtr s(new Spike());
  int x = tmpi;
  for(; x< tmpi+5; x++) {
    QString number = QString::number(x);
        
    QStandardItem* t = new QStandardItem(number);
    t->setCheckable(true);
    s->appendRow(t);
  }
  sm_->appendRow(i, s);
  tmpi++;
  
}

void Reknote::tmpDelete() {
  QModelIndex i = ui.treeView->selectionModel()->selectedIndexes().first();
  sm_->removeItemAtIndex(i);
}

void Reknote::activated ( QModelIndex i ) {
  const SpikePtr p = sm_->getPointerFromIndex(i);
  p->saveAll();
  ui.listView->setModel(p.data());
}
