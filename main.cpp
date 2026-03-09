#include "camlib.h"
#include <QtWidgets/QApplication>
#include <iostream>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    camlib w;
    w.show();
    return a.exec();
}
