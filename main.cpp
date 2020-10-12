#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Nvidia Flasher");
    w.show();
    w.init();

    return a.exec();
}
