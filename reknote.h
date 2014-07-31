#ifndef reknote_H
#define reknote_H

#include <QMainWindow>

#include "ui_reknote.h"

class Reknote : public QMainWindow
{
Q_OBJECT
private:
  Ui::MainWindow ui;

public:
    Reknote();
    virtual ~Reknote();
};

#endif // reknote_H
