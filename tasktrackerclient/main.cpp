#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Task Tracker Client");

    MainWindow w;
    w.show();

    return app.exec();
}
