#ifndef reknote_H
#define reknote_H

#include <QMainWindow>
#include <QModelIndex>

#include "ui_reknote.h"

class SpikesModel;

class Reknote : public QMainWindow {
Q_OBJECT
private:
  Ui::MainWindow ui;
  SpikesModel* sm_;
  int tmpi;

public:
    Reknote();
    virtual ~Reknote();
    
private slots:
    void tmpAdd();
    void activated(QModelIndex i);
};

#endif // reknote_H
