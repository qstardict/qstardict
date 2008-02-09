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

class MainWindow;
#ifdef QSTARDICT_WITH_DBUS
class DBusAdaptor;
#endif

class Application: public QApplication
{
    Q_OBJECT

    public:
        Application(int &argc, char **argv);

        int exec();

        static Application *instance()
        { return qobject_cast<Application*>(QCoreApplication::instance()); }

        MainWindow *mainWindow()
        { return m_mainWindow; }
#ifdef QSTARDICT_WITH_DBUS
        DBusAdaptor *dbusAdaptor()
        { return m_dbusAdaptor; }
#endif
    private:
        MainWindow *m_mainWindow;
#ifdef QSTARDICT_WITH_DBUS
        DBusAdaptor *m_dbusAdaptor;
#endif
};

}

#endif // APPLICATION_H

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent

