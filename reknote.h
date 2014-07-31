#ifndef reknote_H
#define reknote_H

#include <QMainWindow>

#include "ui_reknote.h"

class SpikeModel;

class Reknote : public QMainWindow {
Q_OBJECT
private:
  Ui::MainWindow ui;
  SpikeModel* sm_;

public:
    Reknote();
    virtual ~Reknote();
    
private slots:
    void tmpAdd();
};

#endif // reknote_H
