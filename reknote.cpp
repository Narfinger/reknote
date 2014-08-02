#include <QTreeWidgetItem>
#include <QListView>

#include "reknote.h"
#include "spike.h"
#include "spikestreemodel.h"
#include "spiketreeitem.h"

Reknote::Reknote() {
  ui.setupUi(this);
  
  sm_ = new SpikesTreeModel(this);
  sm_->setItemPrototype(new SpikeTreeItem());
  ui.treeView->setModel(sm_);
  ui.treeView->expandAll();
  ui.treeView->setDragDropMode(QAbstractItemView::InternalMove);
  connect(ui.pushButton, SIGNAL(pressed()), this, SLOT(tmpAdd()));
  connect(ui.treeView, SIGNAL(activated(QModelIndex)), this, SLOT(activated(QModelIndex)));
  
  connect(ui.loadbutton, SIGNAL(pressed()), sm_, SLOT(load()));
  connect(ui.savebutton, SIGNAL(pressed()), sm_, SLOT(save()));
  
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
}

void Reknote::tmpAdd() {
  SpikeTreeItem* i = new SpikeTreeItem("tmp");
  
  //SpikePtr s( new Spike());
  
  Spike* s = new Spike();
  int x = tmpi;
  for(; x< tmpi+5; x++) {
    QString number = QString::number(x);
        
    QStandardItem* t = new QStandardItem(number);
    s->appendRow(t);
  }
  //i->m_ = s;
  i->mm_ = s;
  
  sm_->appendRow(i);
  tmpi++;
  
}

void Reknote::activated ( QModelIndex i ) {
  SpikeTreeItem* sti = static_cast<SpikeTreeItem*>(sm_->itemFromIndex(i));
  qDebug() << sti->rowCount() << sti->columnCount();
  /*SpikePtr p = sti->m_;
  Spike* s = p.data();
  */
  Spike* s = sti->mm_;
  
  ui.listView->setModel(s);
}
