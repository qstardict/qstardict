/*****************************************************************************
 * dictplugin.h - QStarDict, a StarDict clone written using Qt               *
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

#ifndef BASEPLUGIN_H
#define BASEPLUGIN_H

#include <QtPlugin>
#include <QStringList>
#include <QDir>
#include <QCoreApplication>
#include <QVariant>
#include <QIcon>

#include "metadata.h"
#include "pluginserver.h"

namespace QStarDict
{

/**
 * This is a base class for all dictionary plugins classes.
 */
class BasePlugin
{
public:
    /**
     * Accessor to QStarDict internals
     */
    PluginServer *qsd; // QStarDict API accessor

    /**
     * Destructor.
     */
    virtual ~BasePlugin() { }

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    /**
     * Return the plugin's metadata.
     */
    virtual PluginMetadata metadata() const = 0;
#else
    virtual QIcon pluginIcon() const = 0;
#endif
};

class ConfigurablePlugin
{
public:
    virtual ~ConfigurablePlugin() {}
    /**
     * Run a settings dialog and return QDialog::DialogCode.
     */
    virtual int execSettingsDialog(QWidget *parent = 0) = 0;
};


} // namespace QStarDict

Q_DECLARE_INTERFACE(QStarDict::BasePlugin, "org.qstardict.BasePlugin/1.0")
Q_DECLARE_INTERFACE(QStarDict::ConfigurablePlugin, "org.qstardict.ConfigurablePlugin/1.0")


#endif // BASEPLUGIN_H

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent
