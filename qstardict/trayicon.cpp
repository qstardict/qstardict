/*****************************************************************************
 * trayicon.cpp - QStarDict, a StarDict clone written with using Qt          *
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

#include "trayicon.h"

#include <QMenu>

#include "application.h"
#include "mainwindow.h"
#include "popupwindow.h"
#include "settingsdialog.h"

namespace QStarDict
{

TrayIcon::TrayIcon(QObject *parent)
    : QSystemTrayIcon(QIcon(":/icons/qstardict.png"), parent)
{
    QMenu *trayMenu = new QMenu(tr("QStarDict"));
    QAction *actionScan = new QAction(tr("Scan"), this);
    actionScan->setCheckable(true);
    actionScan->setChecked(Application::instance()->popupWindow()->isScan());
    connect(actionScan, SIGNAL(toggled(bool)),
            Application::instance()->popupWindow(), SLOT(setScan(bool)));
    connect(Application::instance()->popupWindow(), SIGNAL(scanChanged(bool)),
            actionScan, SLOT(setChecked(bool)));
    trayMenu->addAction(actionScan);
    QAction *actionSettings = new QAction(QIcon(":/icons/configure.png"), tr("Settings"), this);
    connect(actionSettings, SIGNAL(triggered()), SLOT(on_actionSettings_triggered()));
    trayMenu->addAction(actionSettings);
    QAction *actionQuit = new QAction(QIcon(":/icons/application-exit.png"), tr("Quit"), this);
    connect(actionQuit, SIGNAL(triggered()), Application::instance(), SLOT(quit()));
    trayMenu->addAction(actionQuit);
    setContextMenu(trayMenu);
    connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            SLOT(on_activated(QSystemTrayIcon::ActivationReason)));
}

void TrayIcon::on_activated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger)
        Application::instance()->mainWindow()->setVisible(!
                Application::instance()->mainWindow()->isVisible());
}

void TrayIcon::on_actionSettings_triggered()
{
    SettingsDialog dialog(Application::instance()->mainWindow());
    dialog.exec();
}

}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

