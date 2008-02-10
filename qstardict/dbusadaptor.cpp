/*****************************************************************************
 * dbusadaptor.cpp - QStarDict, a StarDict clone written with using Qt       *
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

#include "dbusadaptor.h"

#include <QDBusConnection>
#include "application.h"
#include "mainwindow.h"
#include "popupwindow.h"

namespace QStarDict
{

DBusAdaptor::DBusAdaptor(MainWindow *mainWindow)
    : QDBusAbstractAdaptor(mainWindow), m_mainWindow(mainWindow)
{
    QDBusConnection connection = QDBusConnection::sessionBus();
    connection.registerService("org.qstardict.dbus");
    connection.registerObject("/qstardict", mainWindow);
}

bool DBusAdaptor::mainWindowVisible() const
{
    return m_mainWindow->isVisible();
}

void DBusAdaptor::setMainWindowVisible(bool visible)
{
    m_mainWindow->setVisible(visible);
}

void DBusAdaptor::showTranslation(const QString &text)
{
    m_mainWindow->showTranslation(text);
}

void DBusAdaptor::showPopup(const QString &text)
{
    Application::instance()->popupWindow()->showTranslation(text);
}

QString DBusAdaptor::translate(const QString &text)
{
    return Application::instance()->dictCore()->translate(text);
}

QString DBusAdaptor::translateHtml(const QString &text)
{
    return Application::instance()->dictCore()->translate(text);
}

}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

