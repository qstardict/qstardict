/*****************************************************************************
 * dictplugin.h - QStarDict, a StarDict clone written using Qt               *
 * Copyright (C) 2016 Sergey Il'inykh                                        *
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

/*
 * This interface is intended to be used from plugin
 * to access QStarDict internals
 */


#ifndef PLUGINSERVER_H
#define PLUGINSERVER_H

#include "metadata.h"

namespace QStarDict {

class PluginServer
{
public:
    virtual ~PluginServer() {}

    /**
     * Returns plugin's metadata. name/descriptions/authors/etc.
     */
    virtual const PluginMetadata &metadata(const QString &pluginId) const = 0;

    /**
     * Return a directory that contains plugin's cofiguration data.
     */
    virtual QString configDir(const QString &pluginId) const = 0;

    /**
     * Returns plugin's id. where instance is instance of plugin
     */
    virtual QString idForPlugin(QObject *instance) const = 0;
};

} // namepace QStarDict

Q_DECLARE_INTERFACE(QStarDict::PluginServer, "org.qstardict.PluginServer/1.0")

#endif // PLUGINSERVER_H

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent
