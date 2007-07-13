#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QTranslator translator;
    translator.load(":/translations/qstardict-" + QLocale::system().name());
    app.installTranslator(&translator);
    app.setOrganizationName("qstardict");
    app.setApplicationName("qstardict");
    MainWindow window;

    return app.exec();
}

