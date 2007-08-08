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
#include "mainwindow.h"

#ifdef Q_OS_UNIX
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <unistd.h>
#endif

#ifdef QSTARDICT_WITH_TRANSLATIONS
#include <QLocale>
#include <QTranslator>
#endif // QSTARDICT_WITH_TRANSLATIONS

#ifdef QSTARDICT_WITH_DBUS
#include <QDBusConnection>
#include "dbusadaptor.h"
#endif // QSTARDICT_WITH_DBUS

int main(int argc, char *argv[])
{
#ifdef Q_OS_UNIX
    QFile lockFile(QDir::tempPath() + "/qstardict.lock");
    lockFile.open(QIODevice::ReadOnly);
    QTextStream lockStream(&lockFile);
    QString oldPid = lockStream.readLine();
    if (oldPid.length() && QDir("/proc/" + oldPid).exists())
    {
        qWarning("qstardict: Already running");
        return 0;
    }
    lockFile.close();
    lockFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
    lockStream << getpid() << endl;
    lockFile.close();
#endif

    QApplication app(argc, argv);
#ifdef QSTARDICT_WITH_TRANSLATIONS
    QTranslator translator;
    translator.load(":/translations/qstardict-" + QLocale::system().name());
    app.installTranslator(&translator);
#endif // QSTARDICT_WITH_TRANSLATIONS
    app.setOrganizationName("qstardict");
    app.setApplicationName("qstardict");
    MainWindow window;
#ifdef QSTARDICT_WITH_DBUS
    DBusAdaptor dbusAdaptor(&window);
#endif // QSTARDICT_WITH_DBUS

    return app.exec();
}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

