#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("qstardict");
    app.setApplicationName("qstardict");
    MainWindow window;
    return app.exec();
}

