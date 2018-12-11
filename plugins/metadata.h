/*****************************************************************************
 * dictplugin.h - QStarDict, a StarDict clone written using Qt               *
 * Copyright (C) 2016 Ilinykh Sergey                                        *
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

#ifndef QSD_METADATA_H
#define QSD_METADATA_H

#include <QString>
#include <QStringList>
#include <QVariantMap>
#include <QIcon>

namespace QStarDict
{

// IMPORTANT: If you change this structure,
// don't forget to change BasePlugin interface version

struct PluginMetadata
{
    QString id; // not-human name. to identify
    QString name; // human name. to display it
    QString version; // plugin's version
    QString description; // short discription to show in plugins list or tooltip
    QStringList authors; // list of authors preferably in format "Name <e-mail>"
    QStringList features; // comma-separated list of supported features qstardict knows about
                          // "dict" - dictionay plugin,
                          // "defenable" - enable this plugin by default if not cached
                          // "de" - desktop environment plugin
                          // "toolbar" - toolbar plugin
    QIcon icon; // plugin's icon. it's cached as soon as we enable plugin once
    QVariantMap extra; // for any not described here use
};


} // namespace QStarDict

#endif // QSD_METADATA_H
