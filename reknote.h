#ifndef reknote_H
#define reknote_H

#include <QMainWindow>
#include <QModelIndex>

#include "ui_reknote.h"

class SpikesTreeModel;

class Reknote : public QMainWindow {
Q_OBJECT
private:
  Ui::MainWindow ui;
  SpikesTreeModel* sm_;

public:
    Reknote();
    virtual ~Reknote();

private slots:
  void addSpike();
  void deleteSelectedSpike(); 
  void spikestreeContextMenu(const QPoint&) const;
    
  void activated(QModelIndex i);
};

#endif // reknote_H
