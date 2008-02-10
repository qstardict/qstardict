/*****************************************************************************
 * application.h - QStarDict, a StarDict clone written with using Qt         *
 * Copyright (C) 2008 Alexander Rodin                                        *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 2 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License along   *
 * with this program; if not, write to the Free Software Foundation, Inc.,   *
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.               *
 *****************************************************************************/

#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>

namespace QStarDict
{
class DictCore;
class MainWindow;
class TrayIcon;
#ifdef QSTARDICT_WITH_DBUS
class DBusAdaptor;
#endif

/**
 * Main application class.
 */
class Application: public QApplication
{
    Q_OBJECT

    public:
        /**
         * Construct new QStarDict application.
         */
        Application(int &argc, char **argv);

        /**
         * Enter the main event loop and wait until exit().
         */
        int exec();

        /**
         * Returns a pointer to the application's QStarDict::Application instance.
         */
        static Application *instance()
        { return qobject_cast<Application*>(QCoreApplication::instance()); }

        /**
         * Returns a pointer to the DictCore instance.
         */
        DictCore *dictCore()
        { return m_dictCore; }
        /**
         * Returns a pointer to the application's main window.
         */
        MainWindow *mainWindow()
        { return m_mainWindow; }

        /**
         * Returns a pointer to the tray icon.
         */
        TrayIcon *trayIcon()
        { return m_trayIcon; }
#ifdef QSTARDICT_WITH_DBUS
        /**
         * Returns a pointer to the DBusAdaptor instance.
         */
        DBusAdaptor *dbusAdaptor()
        { return m_dbusAdaptor; }
#endif
    private:
        DictCore *m_dictCore;
        MainWindow *m_mainWindow;
        TrayIcon *m_trayIcon;
#ifdef QSTARDICT_WITH_DBUS
        DBusAdaptor *m_dbusAdaptor;
#endif
};

}

#endif // APPLICATION_H

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent

