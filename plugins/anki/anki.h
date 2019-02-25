/*****************************************************************************
 * anki.h - QStarDict, a StarDict clone written with using Qt                *
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

#ifndef ANKI_H
#define ANKI_H

#include "../baseplugin.h"
#include "../toolbarplugin.h"

class Anki: public QObject, public QStarDict::BasePlugin, public QStarDict::ToolbarPlugin, public QStarDict::ConfigurablePlugin
{
    Q_OBJECT
    Q_INTERFACES(QStarDict::BasePlugin QStarDict::ToolbarPlugin QStarDict::ConfigurablePlugin)
    Q_PLUGIN_METADATA(IID "org.qstardict.AnkiPlugin/1.0" FILE "anki.json")

    public:
        Anki(QObject *parent = 0);
        ~Anki();

        QIcon pluginIcon() const;
        QIcon toolbarIcon() const;
        QString toolbarText() const;
        void execute(QStarDict::DictWidget *dictWidget);
        int execSettingsDialog(QWidget *parent = 0);

        void setConnectUrl(const QString &url)
        { m_connectUrl = url; }
        QString connectUrl() const
        { return m_connectUrl; }

        void setDeckName(const QString &deckName)
        { m_deckName = deckName; }
        QString deckName() const
        { return m_deckName; }

        void setModelName(const QString &modelName)
        { m_modelName = modelName; }
        QString modelName() const
        { return m_modelName; }

        void setAllowDuplicates(bool allowDuplicates)
        { m_allowDuplicates = allowDuplicates; }
        bool allowDuplicates() const
        { return m_allowDuplicates; }

        void saveSettings();

    private:
        QString m_connectUrl;
        QString m_deckName;
        QString m_modelName;
        bool m_allowDuplicates;
};

#endif // ANKI_H

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent
