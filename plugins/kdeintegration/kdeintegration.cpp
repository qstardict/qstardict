/*
Ported from QtNote project
Copyright (C) 2016 Sergey Ili'nykh

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

Contacts:
E-Mail: rion4ik@gmail.com XMPP: rion@jabber.ru
*/

#include <KStatusNotifierItem>
#include <KWindowSystem>
#ifndef USE_KDE5
#include <KAction>
#include <KMenu>
#else
#include <QAction>
#include <KGlobalAccel>
#endif
#include <KNotification>
#include <QWidget>
#include <QtPlugin>
#include <QAction>
#include <QMenu>

#include "kdeintegration.h"
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include "kdeintegration-meta.h"
#endif


namespace QStarDict {

static const QLatin1String pluginId("kde_de");


class KDEIntegration::Private
{
public:
    KStatusNotifierItem *sni;
};

//------------------------------------------------------------
// KDEIntegration
//------------------------------------------------------------
KDEIntegration::KDEIntegration(QObject *parent) :
    QObject(parent),
    d(new Private)
{
    d->sni = 0;
}

KDEIntegration::~KDEIntegration()
{
    uninitTray();
    delete d;
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
QStarDict::PluginMetadata KDEIntegration::metadata() const
{
    QStarDict::PluginMetadata md;
    md.id = PLUGIN_ID;
    md.name = QString::fromUtf8(PLUGIN_NAME);
    md.authors = QString::fromUtf8(PLUGIN_AUTHORS).split(';', QString::SkipEmptyParts);
    md.features = QString::fromLatin1(PLUGIN_FEATURES).split(';', QString::SkipEmptyParts);
    md.icon = QIcon(":/icons/kde-logo");
    return md;
}
#else
QIcon KDEIntegration::pluginIcon() const
{
    return QIcon(":/icons/kde-logo");
}
#endif

QStarDict::TrayIconPlugin::TrayCompat KDEIntegration::isDECompatible()
{
    QString session = qgetenv("XDG_CURRENT_DESKTOP");
    QString dataDirs = qgetenv("XDG_DATAD_DIRS");
    if (session == "KDE" || session.contains("kde") || session.contains("plasma"))
        return QStarDict::TrayIconPlugin::CompatFull;
    else
        return QStarDict::TrayIconPlugin::CompatNone;
}

void KDEIntegration::initTray()
{
    if (d->sni)
        return;
    d->sni = new KStatusNotifierItem("qstardict", this);
    d->sni->setIconByName("qstardict"); // TODO review dev mode when no icon in /usr/share
    d->sni->setStatus(KStatusNotifierItem::Active);
    d->sni->setTitle("QStarDict");

    connect(d->sni, SIGNAL(secondaryActivateRequested(QPoint)), SIGNAL(translateClipboard()));
}

void KDEIntegration::uninitTray()
{
    if (d->sni) {
        d->sni->setParent(0);
        delete d->sni;
        d->sni = 0;
    }
}

TrayIconPlugin::Features KDEIntegration::features() const
{
    return ClipoardTranslate;
}

void KDEIntegration::setContextMenu(QMenu *menu)
{
#ifdef USE_KDE5
    d->sni->setContextMenu(menu);
#else
    KMenu *kmenu = new KMenu(menu);
    foreach (QAction *a, menu->actions()) {
        KAction *ka = new KAction(KIcon(a->icon()), a->text(), a);
        if (!ka->isSeparator()) {
            connect(ka, SIGNAL(triggered(bool)), a, SIGNAL(triggered(bool)));
        }
        kmenu->addAction(ka);
    }
    d->sni->setContextMenu(kmenu);
#endif
}

void KDEIntegration::setMainWindow(QWidget *w)
{
    d->sni->setAssociatedWidget(w);
}

void KDEIntegration::setScanEnabled(bool enabled)
{
    // TODO rewrite to system icons
    QIcon icon(enabled ? ":/icons/qstardict.png" : ":/icons/qstardict-disabled.png");
    d->sni->setIconByPixmap(icon);
    d->sni->setToolTip(icon, "QStarDict", tr("scanning is %1").arg(enabled ? tr("enabled") : tr("disabled")));
}

void KDEIntegration::setVisible(bool visible)
{
    d->sni->setStatus(visible? KStatusNotifierItem::Active : KStatusNotifierItem::Passive);
}

bool KDEIntegration::isVisible() const
{
    return d->sni->status() != KStatusNotifierItem::Passive;
}


/*
void KDEIntegration::activateWidget(QWidget *w)
{
    KWindowSystem::forceActiveWindow(w->winId(), 0); // just activateWindow doesn't work when started from tray
}
*/


} // namespace QStarDict

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(kdeintegration, QStarDict::KDEIntegration)
#endif
