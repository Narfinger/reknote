#include "reknote.h"
#include "spikemodel.h"

#include <QTreeWidgetItem>

Reknote::Reknote() {
  ui.setupUi(this);
  
  sm_ = new SpikeModel(this);
  ui.treeView->setModel(sm_);
  ui.treeView->expandAll();
  ui.treeView->setDragDropMode(QAbstractItemView::InternalMove);
  connect(ui.pushButton, SIGNAL(pressed()), this, SLOT(tmpAdd()));
  
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

void Reknote::tmpAdd()
{
  QStandardItem* i = new QStandardItem("tmp");
  sm_->appendRow(i);
  

}

