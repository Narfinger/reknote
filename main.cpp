#include <QtGui/QApplication>
#include "reknote.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    Reknote foo;
    foo.show();
    return app.exec();
}
