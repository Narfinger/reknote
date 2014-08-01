#include "reknote.h"
#include "spikestreemodel.h"

#include <QTreeWidgetItem>

Reknote::Reknote() {
  ui.setupUi(this);
  
  sm_ = new SpikesTreeModel(this);
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
  QStandardItem* i = new QStandardItem("tmp");
  i->setData(QVariant(tmpi));
  sm_->appendRow(i);
  tmpi++;
  
}

void Reknote::activated ( QModelIndex i ) {
  QString tmp = sm_->data(i, Qt::UserRole+1).toString();
  ui.label->setText(tmp);
}


