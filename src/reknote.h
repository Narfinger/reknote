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


public:
  Reknote();
  virtual ~Reknote();

private:
  QLabel* sbarText;
  QLabel* sbarIcon;
  
private slots:
  void addSpike()  		{ statusBar()->showMessage("Added", 1*1000); ui.spikenotewidget->addSpike(); }
  void deleteSelectedSpike()  	{ ui.spikenotewidget->deleteSelectedSpike(); }
  void deleteNote()  		{ ui.spikenotewidget->deleteNote(); }
  void showHistory();
  //void spikestreeContextMenu(const QPoint&) const;
  void commitWaiting();
  void commitFinished();
  void cleanDone() 		{ ui.spikenotewidget->cleanDone(); }
};

#endif // reknote_H
