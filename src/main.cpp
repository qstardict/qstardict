/********************************************************************************
 * main.cpp - QStarDict, a StarDict clone written with using Qt                 *
 * Copyright (C) 2007 Alexander Rodin                                           *
 *                                                                              *
 * This program is free software: you can redistribute it and/or modify         *
 * it under the terms of the GNU General Public License as published by         *
 * the Free Software Foundation, either version 3 of the License, or            *
 * (at your option) any later version.                                          *
 *                                                                              *
 * This program is distributed in the hope that it will be useful,              *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of               *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                *
 * GNU General Public License for more details.                                 *
 *                                                                              *
 * You should have received a copy of the GNU General Public License            *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.        *
 ********************************************************************************/

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include "mainwindow.h"

#if !defined(QT_NO_DBUS) && defined Q_OS_UNIX
#include <QDBusConnection>
#include "dbusadaptor.h"
#endif // QT_NO_DBUS

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QTranslator translator;
    translator.load(":/translations/qstardict-" + QLocale::system().name());
    app.installTranslator(&translator);
    app.setOrganizationName("qstardict");
    app.setApplicationName("qstardict");
    MainWindow window;
#if !defined(QT_NO_DBUS) && defined Q_OS_UNIX
    DBusAdaptor dbusAdaptor(&window);
#endif // QT_NO_DBUS

    return app.exec();
}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent

