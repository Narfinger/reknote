#ifndef reknote_H
#define reknote_H

#include <QMainWindow>
#include <QModelIndex>

#include "ui_reknote.h"

class QLabel;
class SpikesTreeModel;
class GitRepository;

class Reknote : public QMainWindow {
Q_OBJECT
private:
  Ui::MainWindow ui;
  SpikesTreeModel* sm_;

public:
  Reknote();
  virtual ~Reknote();

private:
  QLabel* sbarText;
  QLabel* sbarIcon;
  
private slots:    
  void addSpike();
  void deleteSelectedSpike(); 
  void deleteNote();
  void spikestreeContextMenu(const QPoint&) const;
  void activated(QModelIndex i);
  void commitWaiting();
  void commitFinished();
};

#endif // reknote_H
