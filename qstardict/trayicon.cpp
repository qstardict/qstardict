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
#include <QTimer>

#include "application.h"
#include "mainwindow.h"
#include "popupwindow.h"
#include "settingsdialog.h"

namespace QStarDict
{


TrayIconDefaultImpl::TrayIconDefaultImpl(QObject *parent) :
    QObject(parent),
    sti(0),
    associatedWidget(0),
    actionMainWindow(0)
{
}

TrayIconDefaultImpl::~TrayIconDefaultImpl()
{
    delete sti->contextMenu();
}

TrayIconPlugin::TrayCompat TrayIconDefaultImpl::isDECompatible()
{
    return TrayIconPlugin::CompatFallback;
}

void TrayIconDefaultImpl::initTray()
{
    sti = new QSystemTrayIcon(this);
    actionMainWindow = new QAction(tr("Show &main window"), sti);

    connect(sti, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            SLOT(on_activated(QSystemTrayIcon::ActivationReason)));
}

void TrayIconDefaultImpl::uninitTray()
{
    if (sti) {
        delete sti->contextMenu();
        sti->setParent(0);
        delete sti;
        sti = 0;
    }
}

TrayIconPlugin::Features TrayIconDefaultImpl::features() const
{
    return ClipoardTranslate;
}

void TrayIconDefaultImpl::setContextMenu(QMenu *menu)
{
    menu->insertAction(menu->actions()[0], actionMainWindow);

    QAction *actionQuit = new QAction(QIcon(":/icons/application-exit.png"), tr("&Quit"), menu);
    connect(actionQuit, SIGNAL(triggered()), Application::instance(), SLOT(quit()));
    menu->addAction(actionQuit);

    sti->setContextMenu(menu);
}

void TrayIconDefaultImpl::setMainWindow(QWidget *w)
{
    associatedWidget = w;
    connect(actionMainWindow, SIGNAL(triggered(bool)), w, SLOT(show()));
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

bool TrayIconDefaultImpl::isVisible() const
{
    return sti->isVisible();
}

void TrayIconDefaultImpl::on_activated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Trigger:
        // It's quite uncomfortable on OS X to handle show/hide main window
        // in all cases... at least for me (petr)
#ifndef Q_WS_MAC
        associatedWidget->setVisible(!associatedWidget->isVisible());
#else
        mw->show();
#endif
        break;
    case QSystemTrayIcon::MiddleClick:
        emit translateClipboard();
        break;
    default:
        ; // nothing
    }
}


TrayIcon::TrayIcon(QObject *parent) :
    QObject(parent),
    _mainWindow(0),
    _trayImpl(0)
{
    _initTrayTimer = new QTimer(this);
    _initTrayTimer->setSingleShot(true);
    _initTrayTimer->setInterval(100);
    connect(_initTrayTimer, SIGNAL(timeout()), SLOT(reinit()));

    _defaultTrayImpl = new TrayIconDefaultImpl(this);

    reinit();
}

TrayIcon::~TrayIcon()
{
    saveSettings();
}

void TrayIcon::reinit()
{
    if (_trayImpl) {
        qobject_cast<TrayIconPlugin*>(_trayImpl)->uninitTray();
    }

    _trayCandidat.clear();
    _trayFallbackCandidat.clear();
    foreach (const QString &plugin, Application::instance()->dictCore()->loadedPlugins()) {
        QObject *o = Application::instance()->dictCore()->plugin(plugin);
        if (o) {
            TrayIconPlugin *tip = qobject_cast<TrayIconPlugin*>(o);
            if (tip) {
                connect(o, SIGNAL(destroyed(QObject*)), SLOT(on_trayImplDestroyed(QObject*)), Qt::UniqueConnection);
                switch (tip->isDECompatible()) {
                case TrayIconPlugin::CompatFull:
                    _trayCandidat.append(o);
                    break;
                case TrayIconPlugin::CompatFallback:
                    _trayFallbackCandidat.append(o);
                    break;
                case TrayIconPlugin::CompatNone:
                default:
                    break;
                }
            }
        }
    }
    _trayFallbackCandidat.append(_defaultTrayImpl);

    QMenu *trayMenu = new QMenu(tr("QStarDict"));

    QAction *actionScan = new QAction(QIcon(":/icons/edit-select.png"), tr("&Scan"), trayMenu);
    actionScan->setCheckable(true);
    actionScan->setChecked(Application::instance()->popupWindow()->isScan());
    connect(actionScan, SIGNAL(toggled(bool)),
            Application::instance()->popupWindow(), SLOT(setScan(bool)));
    connect(Application::instance()->popupWindow(), SIGNAL(scanChanged(bool)),
            actionScan, SLOT(setChecked(bool)));
    connect(Application::instance()->popupWindow(), SIGNAL(scanChanged(bool)), SLOT(setScanEnabled(bool)));
    trayMenu->addAction(actionScan);

    QAction *actionSettings = new QAction(QIcon(":/icons/configure.png"), tr("&Configure QStarDict"), trayMenu);
    connect(actionSettings, SIGNAL(triggered()), SLOT(on_actionSettings_triggered()));
    trayMenu->addAction(actionSettings);

    if (_trayCandidat.count()) {
        _trayImpl = _trayCandidat[0];
    } else {
        _trayImpl = _trayFallbackCandidat[0];
    }

    TrayIconPlugin *tip = qobject_cast<TrayIconPlugin*>(_trayImpl);
    tip->initTray();
    tip->setContextMenu(trayMenu);

    if (_mainWindow) {
        tip->setMainWindow(_mainWindow);
    }

    setScanEnabled(Application::instance()->popupWindow()->isScan());
    if (tip->features() & TrayIconPlugin::ClipoardTranslate) {
        connect(_trayImpl, SIGNAL(translateClipboard()), SLOT(translateClipboard()));
    }

    loadSettings();
}

void TrayIcon::on_trayImplDestroyed(QObject *o)
{
    _trayCandidat.removeOne(o);
    _trayFallbackCandidat.removeOne(o);
    if (o == _trayImpl) {
        _trayImpl = 0;
        _initTrayTimer->start();
    }
}

void TrayIcon::translateClipboard()
{
    Application::instance()->popupWindow()->showTranslation(Application::clipboard()->text(QClipboard::Selection));
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
    _mainWindow = w;
    qobject_cast<TrayIconPlugin*>(_trayImpl)->setMainWindow(w);
}

void TrayIcon::setVisible(bool visible)
{
    qobject_cast<TrayIconPlugin*>(_trayImpl)->setVisible(visible);
}

bool TrayIcon::isVisible() const
{
    return qobject_cast<TrayIconPlugin*>(_trayImpl)->isVisible();
}

void TrayIcon::loadSettings()
{
    QSettings config;
    qobject_cast<TrayIconPlugin*>(_trayImpl)->setVisible(config.value("TrayIcon/visible", true).toBool());
}


}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

