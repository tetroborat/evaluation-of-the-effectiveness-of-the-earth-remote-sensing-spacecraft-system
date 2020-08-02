#include "mainwindow.h"
#include <QApplication>
#include "input.h"
#include "manager.h"
#include "listsc.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.showMaximized();
    return a.exec();
}

