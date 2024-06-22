

#include <QApplication>
#include <QTextCodec>
#include <QTranslator>

#include "mainwindow.h"
#include "settings.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Task Tracker Client");

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    Settings settings("settings.ini");

    QTranslator translator;
    translator.load(settings.language());
    app.installTranslator(&translator);

    MainWindow w(&settings);
    w.show();

    return app.exec();
}
