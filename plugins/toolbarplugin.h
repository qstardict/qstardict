/*****************************************************************************
 * dictplugin.h - QStarDict, a StarDict clone written using Qt               *
 * Copyright (C) 2018 Alexander Rodin                                        *
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

#ifndef TOOLBARPLUGIN_H
#define TOOLBARPLUGIN_H

#include <QtPlugin>
#include "../qstardict/dictwidget.h"

namespace QStarDict {
class ToolbarPlugin {
    public:
        virtual ~ToolbarPlugin() {}

        virtual QIcon toolbarIcon() const = 0;
        virtual QString toolbarText() const = 0;
        virtual void execute(DictWidget *dictWidget) = 0;
};

} // namespace QStarDict

Q_DECLARE_INTERFACE(QStarDict::ToolbarPlugin, "org.qstardict.ToolbarPlugin/1.0")

#endif // TOOLBARPLUGIN_H
