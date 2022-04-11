#include "go.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Go w;
    w.show();

    return a.exec();
}
