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

namespace QStarDict
{

/**
 * This is a base class for all dictionary plugins classes.
 */
class BasePlugin
{
public:

    /**
     * Destructor.
     */
    virtual ~BasePlugin() { }

    /**
     * Return the plugin name.
     */
    virtual QString name() const = 0;

    /**
     * Return the plugin version.
     */
    virtual QString version() const = 0;

    /**
     * Return the plugin description.
     */
    virtual QString description() const = 0;

    /**
     * Return the plugin authors.
     */
    virtual QStringList authors() const = 0;

protected:
    /**
     * Return a directory that contains plugin's data.
     */
    QString workPath() const
    {
        QString path = QDir::homePath() + "/.config/qstardict/pluginsdata/" + name();

        if (! QDir::root().exists(path))
            QDir::root().mkpath(path);
        return path;
    }
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
