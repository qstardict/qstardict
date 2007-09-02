/********************************************************************************
 * dbusadaptor.h - QStarDict, a StarDict clone written with using Qt            *
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

#ifndef DBUSADAPTOR_H
#define DBUSADAPTOR_H

#include <QDBusAbstractAdaptor>

class MainWindow;

class DBusAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.qstardict.dbus")
    Q_PROPERTY(int mainWindowVisible READ mainWindowVisible WRITE setMainWindowVisible)

    public:
        DBusAdaptor(MainWindow *mainWindow);

        bool mainWindowVisible() const;
        void setMainWindowVisible(bool visible);

    public slots:
        void showTranslation(const QString &text);
        void showPopup(const QString &text);
	QString translate(const QString &text);
	QString translateHtml(const QString &text);

    private:
        MainWindow *m_mainWindow;
};

#endif // DBUSADAPTOR_H

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc
