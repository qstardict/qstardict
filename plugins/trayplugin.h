/*****************************************************************************
 * dictplugin.h - QStarDict, a StarDict clone written using Qt               *
 * Copyright (C) 2016 Sergey Ilinykh                                         *
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

#ifndef TRAYPLUGIN_H
#define TRAYPLUGIN_H

#include <QtPlugin>
#include <QStringList>
#include <QDir>
#include <QCoreApplication>
#include <QVariant>

#include "baseplugin.h"

#include <QMenu>

namespace QStarDict
{

/**
 * This is a base class for all tray plugins classes.
 */
class TrayIconPlugin
{
public:
    enum class TrayCompat
    {
        None,
        Fallback,
        Full
    };

    enum class Feature
    {
        None = 0x0,
        ClipoardTranslate = 0x1
    };
    Q_DECLARE_FLAGS(Features, Feature)

    virtual ~TrayIconPlugin() {}

    virtual TrayCompat isDECompatible() = 0;
    virtual void initTray() = 0;
    virtual void uninitTray() = 0;
    virtual Features features() const = 0;
    virtual void setContextMenu(QMenu *menu) = 0;
    virtual void setMainWindow(QWidget *w) = 0;
    virtual void setScanEnabled(bool enabled) = 0;
    virtual void setVisible(bool visible) = 0;
    virtual bool isVisible() const = 0;

    /* Possible signals:
     * void translateClipboard();
     */
};


} // namespace QStarDict

Q_DECLARE_OPERATORS_FOR_FLAGS(QStarDict::TrayIconPlugin::Features)
Q_DECLARE_INTERFACE(QStarDict::TrayIconPlugin, "org.qstardict.TrayIconPlugin/1.1")

#endif // TRAYPLUGIN_H

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent
