/*****************************************************************************
 * web.h - QStarDict, a StarDict clone written with using Qt                 *
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

#ifndef WEB_H
#define WEB_H

#include "../dictplugin.h"

#include <QHash>

class Web: public QObject, public QStarDict::BasePlugin, public QStarDict::DictPlugin, public QStarDict::ConfigurablePlugin
{
    Q_OBJECT
    Q_INTERFACES(QStarDict::BasePlugin QStarDict::DictPlugin QStarDict::ConfigurablePlugin)
    Q_PLUGIN_METADATA(IID "org.qstardict.WebPlugin/1.0" FILE "web.json")

    public:
        Web(QObject *parent = 0);

        QIcon pluginIcon() const;
        Features features() const
        { return 0; }

        QStringList availableDicts() const;
        QStringList loadedDicts() const
        { return m_loadedDicts.keys(); }
        void setLoadedDicts(const QStringList &dicts);
        DictInfo dictInfo(const QString &dict);

        bool isTranslatable(const QString &dict, const QString &word);
        Translation translate(const QString &dict, const QString &word);

        int execSettingsDialog(QWidget *parent);

        friend class SettingsDialog;
    private:
        struct QueryStruct
        {
            QString query;
            QByteArray codec;
        };
        QHash<QString, QueryStruct> m_loadedDicts;
};

#endif // WEB_H

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent

