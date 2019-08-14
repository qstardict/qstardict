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

#include <optional>
#include <functional>
#include <QJsonObject>
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

        void setAllowDuplicates(bool allowDuplicates)
        { m_allowDuplicates = allowDuplicates; }
        bool allowDuplicates() const
        { return m_allowDuplicates; }

        void setBasicCard(bool basicCard)
        { m_basicCard = basicCard; }
        bool basicCard() const
        { return m_basicCard; }

        void setBasicCardDeckName(std::optional<QString> basicCardDeckName)
        { m_basicCardDeckName = basicCardDeckName; }
        std::optional<QString> basicCardDeckName() const
        { return m_basicCardDeckName; }

        void setReversedBasicCard(bool reversedBasicCard)
        { m_reversedBasicCard = reversedBasicCard; }
        bool reversedBasicCard() const
        { return m_reversedBasicCard; }

        void setReversedBasicCardDeckName(std::optional<QString> reversedBasicCardDeckName)
        { m_reversedBasicCardDeckName = reversedBasicCardDeckName; }
        std::optional<QString> reversedBasicCardDeckName() const
        { return m_reversedBasicCardDeckName; }

        void setTypeInCard(bool TypeInCard)
        { m_typeInCard = TypeInCard; }
        bool typeInCard() const
        { return m_typeInCard; }

        void setTypeInCardDeckName(std::optional<QString> typeInCardDeckName)
        { m_typeInCardDeckName = typeInCardDeckName; }
        std::optional<QString> typeInCardDeckName() const
        { return m_typeInCardDeckName; }

        void setReversedTypeInCard(bool reversedTypeInCard)
        { m_reversedTypeInCard = reversedTypeInCard; }
        bool reversedTypeInCard() const
        { return m_reversedTypeInCard; }

        void setReversedTypeInCardDeckName(std::optional<QString> reversedTypeInCardDeckName)
        { m_reversedTypeInCardDeckName = reversedTypeInCardDeckName; }
        std::optional<QString> reversedTypeInCardDeckName() const
        { return m_reversedTypeInCardDeckName; }

        void saveSettings();

    private:
        struct Card {
            QString modelName;
            QString deckName;
            QString front;
            QString back;
        };

        QString m_connectUrl;
        QString m_deckName;
        bool m_allowDuplicates;

        bool m_basicCard;
        std::optional<QString> m_basicCardDeckName;
        bool m_reversedBasicCard;
        std::optional<QString> m_reversedBasicCardDeckName;
        bool m_typeInCard;
        std::optional<QString> m_typeInCardDeckName;
        bool m_reversedTypeInCard;
        std::optional<QString> m_reversedTypeInCardDeckName;

        bool m_showError;
        
        void createDeck(const QString &deckName, std::function<void(bool)> callback);
        void createCard(const Card &card, std::function<void(bool)> callback);
        void sendRequest(const QJsonObject &requestObject, std::function<void(bool)> callback);
        void showError(QWidget *parent);
};

#endif // ANKI_H

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent
