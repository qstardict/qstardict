/*****************************************************************************
 * dbusadaptor.h - QStarDict, a StarDict clone written with using Qt         *
 * Copyright (C) 2007 Alexander Rodin                                        *
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

#ifndef DBUSADAPTOR_H
#define DBUSADAPTOR_H

#include <QDBusAbstractAdaptor>

namespace QStarDict
{

class MainWindow;

/**
 * The DBusAdaptor class represents and QStarDict D-Bus interface.
 */
class DBusAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.qstardict.dbus")
    /**
     * The main window visible property.
     */
    Q_PROPERTY(int mainWindowVisible READ mainWindowVisible WRITE setMainWindowVisible)

    public:
        /**
         * Construct a DBusAdaptor.
         */
        DBusAdaptor(MainWindow *mainWindow);

        /**
         * Return true if main window is visible, otherwise return
         * false.
         */
        bool mainWindowVisible() const;
        /**
         * Set visible state of main window.
         */
        void setMainWindowVisible(bool visible);

    public slots:
        /**
         * Show main window with translation of text.
         */
        void showTranslation(const QString &text);
        /**
         * Show popup window with translation of text.
         */
        void showPopup(const QString &text);
        /**
         * Return a translation of text in plain text format.
         */
        QString translate(const QString &text);
        /**
         * Return a translation of text in HTML format.
         */
        QString translateHtml(const QString &text);

    private:
        MainWindow *m_mainWindow;
};

}

#endif // DBUSADAPTOR_H

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

