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

#include <QClipboard>
#include <QMenu>
#include <QSettings>

#include "application.h"
#include "mainwindow.h"
#include "popupwindow.h"
#include "settingsdialog.h"

namespace QStarDict
{


TrayIconDefaultImpl::TrayIconDefaultImpl(QObject *parent) :
    QObject(parent),
    sti(0),
    mw(0)
{

}

TrayIconPlugin::TrayCompat TrayIconDefaultImpl::isDECompatible()
{
    return TrayIconPlugin::CompatFallback;
}

void TrayIconDefaultImpl::initTray()
{
    sti = new QSystemTrayIcon(this);
    connect(sti, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            SLOT(on_activated(QSystemTrayIcon::ActivationReason)));
}

void TrayIconDefaultImpl::setContextMenu(QMenu *menu)
{
    sti->setContextMenu(menu);
}

void TrayIconDefaultImpl::setMainWindow(QWidget *w)
{
    mw = w;
}

void TrayIconDefaultImpl::setScanEnabled(bool enabled)
{
    QIcon icon(enabled ? ":/icons/qstardict.png" : ":/icons/qstardict-disabled.png");
    sti->setIcon(icon);
    sti->setToolTip(tr("QStarDict: scanning is %1").arg(enabled ? tr("enabled") : tr("disabled")));
}

void TrayIconDefaultImpl::setVisible(bool visible)
{
    sti->setVisible(visible);
}

void TrayIconDefaultImpl::on_activated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Trigger:
        // It's quite uncomfortable on OS X to handle show/hide main window
        // in all cases... at least for me (petr)
#ifndef Q_WS_MAC
        mw->setVisible(!mw->isVisible());
#else
        mw->show();
#endif
        break;
    case QSystemTrayIcon::MiddleClick:
        Application::instance()->popupWindow()->showTranslation(Application::clipboard()->text(QClipboard::Selection));
        break;
    default:
        ; // nothing
    }
}


TrayIcon::TrayIcon(QObject *parent)
    : QSystemTrayIcon(parent)
{
    QMenu *trayMenu = new QMenu(tr("QStarDict"));

    _actionMainWindow = new QAction(tr("Show &main window"), this);
    trayMenu->addAction(_actionMainWindow);

    QAction *actionScan = new QAction(QIcon(":/icons/edit-select.png"), tr("&Scan"), this);
    actionScan->setCheckable(true);
    actionScan->setChecked(Application::instance()->popupWindow()->isScan());
    connect(actionScan, SIGNAL(toggled(bool)),
            Application::instance()->popupWindow(), SLOT(setScan(bool)));
    connect(Application::instance()->popupWindow(), SIGNAL(scanChanged(bool)),
            actionScan, SLOT(setChecked(bool)));
    connect(Application::instance()->popupWindow(), SIGNAL(scanChanged(bool)), SLOT(setScanEnabled(bool)));
    trayMenu->addAction(actionScan);

    QAction *actionSettings = new QAction(QIcon(":/icons/configure.png"), tr("&Configure QStarDict"), this);
    connect(actionSettings, SIGNAL(triggered()), SLOT(on_actionSettings_triggered()));
    trayMenu->addAction(actionSettings);

    QAction *actionQuit = new QAction(QIcon(":/icons/application-exit.png"), tr("&Quit"), this);
    connect(actionQuit, SIGNAL(triggered()), Application::instance(), SLOT(quit()));
    trayMenu->addAction(actionQuit);


    QObject *fbTray = 0;
    QObject *tray = 0;
    foreach (const QString &plugin, Application::instance()->dictCore()->loadedPlugins()) {
        QObject *o = Application::instance()->dictCore()->plugin(plugin);
        if (o) {
            TrayIconPlugin *tip = qobject_cast<TrayIconPlugin*>(o);
            if (tip) {
                switch (tip->isDECompatible()) {
                case TrayIconPlugin::CompatFallback:
                    fbTray = o;
                    break;
                case TrayIconPlugin::CompatFull:
                    tray = o;
                    break;
                case TrayIconPlugin::CompatNone:
                default:
                    break;
                }
            }
        }
    }
    if (tray) {
        _trayImpl = tray;
    } else if(fbTray) {
        _trayImpl = fbTray;
    } else {
        _trayImpl = new TrayIconDefaultImpl(this);
    }

    qobject_cast<TrayIconPlugin*>(_trayImpl)->initTray();
    qobject_cast<TrayIconPlugin*>(_trayImpl)->setContextMenu(trayMenu);

    setScanEnabled(Application::instance()->popupWindow()->isScan());

    loadSettings();
}

TrayIcon::~TrayIcon()
{
    saveSettings();
}

void TrayIcon::on_actionSettings_triggered()
{
    SettingsDialog dialog(Application::instance()->mainWindow());
    dialog.exec();
}

void TrayIcon::setScanEnabled(bool enabled)
{
    qobject_cast<TrayIconPlugin*>(_trayImpl)->setScanEnabled(enabled);
}

void TrayIcon::saveSettings()
{
    QSettings config;
    config.setValue("TrayIcon/visible", isVisible());
}

void TrayIcon::setMainWindow(QWidget *w)
{
    qobject_cast<TrayIconPlugin*>(_trayImpl)->setMainWindow(w);
    connect(_actionMainWindow, SIGNAL(triggered()), w, SLOT(show()));
}

void TrayIcon::loadSettings()
{
    QSettings config;
    qobject_cast<TrayIconPlugin*>(_trayImpl)->setVisible(config.value("TrayIcon/visible", true).toBool());
}


}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

