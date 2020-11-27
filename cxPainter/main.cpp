
#include <QApplication>
#include "cxpainter.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    cxPainter w;
    w.show();

    return a.exec();
}
