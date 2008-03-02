/*****************************************************************************
 * trayicon.h - QStarDict, a StarDict clone written with using Qt            *
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

#ifndef TRAYICON_H
#define TRAYICON_H

#include <QSystemTrayIcon>

namespace QStarDict
{

class TrayIcon: public QSystemTrayIcon
{
    Q_OBJECT

    public:
        TrayIcon(QObject *parent = 0);

    private slots:
        void on_activated(QSystemTrayIcon::ActivationReason reason);
        void on_actionSettings_triggered();
};

}

#endif // TRAYICON_H

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent

