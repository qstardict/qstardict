/*
The code is adpted from QtNote project
Copyright (C) 2010-2016 Sergey Ili'nykh

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

#include <QPalette>
#include <QFont>

#include "pluginsmodel.h"
#include "dictcore.h"
#include "application.h"
#include "pluginmanager.h"
#include "util.h"

namespace QStarDict {



PluginsModel::PluginsModel(LoadType lt, PluginManager *pmanager) :
    QAbstractTableModel(pmanager),
    pmanager(pmanager)
{
    pluginIds = pmanager->availablePlugins();
    QMutableStringListIterator it(pluginIds);
    while (it.hasNext()) {
        auto &md = pmanager->pluginDesc(it.next())->metadata;
        if (lt == JustDict && !md.features.contains("dict")) {
            it.remove();
        }
        if (lt == ExceptDict && md.features.contains("dict")) {
            it.remove();
        }
    }
    QPixmap pix(":/icons/configure.png");
    settingIcon = QIcon(pix);
    QTransform transform;
    transform.rotate(45);
    pix.transformed(transform, Qt::SmoothTransformation);

    settingIcon.addPixmap(pix.transformed(transform),
                          QIcon::Active);
}

int PluginsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return pluginIds.count();
}

int PluginsModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return 4;
}

QVariant PluginsModel::data(const QModelIndex &index, int role) const
{
    QString pluginId = pluginIds[index.row()];
    if (role == IdRole) {
        return pluginId;
    }
    if (index.column() == 0) {
        switch (role) {
        case Qt::CheckStateRole:
            return pmanager->pluginDesc(pluginId)->isEnabled()?
                        Qt::Checked : Qt::Unchecked;

        case Qt::DisplayRole:
            return pmanager->pluginDesc(pluginId)->metadata.name;

        case Qt::DecorationRole:
        {
            auto &md = pmanager->pluginDesc(pluginId)->metadata;
            QIcon icon = md.icon;
            if (icon.isNull()) {
                if (md.features.contains("dict")) {
                    icon = QIcon(":/icons/qstardict.png");
                } else if (md.features.contains("de")) {
                    icon = QIcon(":/icons/desktop.png");
                } else {
                    icon = QIcon(":/icons/plugin.png");
                }
            }
            return icon;
        }

        case Qt::ToolTipRole:
        {
            auto pd = pmanager->pluginDesc(pluginId);
            QString ret = pd->metadata.description + QLatin1String("<br/><br/>") +
                    tr("<b>Filename:</b> %1").arg(pd->loader->fileName()) + "<br/><br/>" +
                    tr("<b>Status:</b> %1").arg(
                        pd->state & PluginManager::Plugin::Valid?
                            pd->isLoaded()?
                                tr("Loaded") : tr("Not Loaded") :
                            tr("Invalid")
                        );
            if (!pd->metadata.description.isEmpty()) {
                ret += QLatin1String("<br/><br/>");
                ret += Util::htmlEscape(pd->metadata.description).replace('\n', "<br/>");
            }
            return ret;
        }

        case Qt::FontRole:
        {
            QFont f; // application default font. may by not what we expect
            auto pd = pmanager->pluginDesc(pluginId);
            if (pd->isLoaded()) {
                f.setBold(true);
            }
            return f;
        }

        case Qt::ForegroundRole:
        {
            QColor color = qApp->palette().color(QPalette::Foreground); // mey be not what we expect
            auto pd = pmanager->pluginDesc(pluginId);
            if (!(pd->state & PluginManager::Plugin::Valid)) {
                color.setAlpha(128);
            }
            return color;
        }
        }
    } else if (index.column() == 1) { // version
        if (role == Qt::DisplayRole) {
            return pmanager->pluginDesc(pluginId)->metadata.version;
        }
    } else if (index.column() == 2) { // settings button
        // options button
        if (role == Qt::DecorationRole && pmanager->plugin<ConfigurablePlugin>(pluginId)) {
            return settingIcon;
        }
    } else if (index.column() == 3) { // settings button
        // options button
        if (role == Qt::DecorationRole) {
            return QIcon(":/icons/dialog-information.png");
        }
    }
    return QVariant();
}

bool PluginsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.column() == 0 && role == Qt::CheckStateRole) {
        Qt::CheckState cs = (Qt::CheckState)value.value<int>();
        pmanager->setEnabled(pluginIds[index.row()], cs == Qt::Checked);
        emit dataChanged(index, index);
        emit loadedListChanged();
        return true;
    }
    return false;
}

Qt::ItemFlags PluginsModel::flags(const QModelIndex &index) const
{
    if (index.column() == 0) {
        return QAbstractTableModel::flags(index) | Qt::ItemIsUserCheckable;
    }
    return QAbstractTableModel::flags(index);
}

QString PluginsModel::pluginId(int row) const
{
    return pluginIds.at(row);
}


QStringList PluginsModel::loadedPlugins()
{
    QStringList ret;
    foreach (const QString &id, pluginIds) {
        if (pmanager->isLoaded(id)) {
            ret << id;
        }
    }
    return ret;
}

} // namespace QStarDict
