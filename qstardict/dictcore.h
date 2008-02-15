/*****************************************************************************
 * dictcore.h - QStarDict, a StarDict clone written with using Qt            *
 * Copyright (C) 2007 Alexander Rodin                                        *
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

#ifndef DICTCORE_H
#define DICTCORE_H

#include <QObject>

#include <QStringList>
#include <QPair>
#include <QPluginLoader>
#include <QHash>

namespace QStarDict
{

/**
 * The DictCore is a base dictionary class.
 */
class DictCore: public QObject
{
    Q_OBJECT

    public:
        /**
         * Construct dictionary.
         */
        DictCore(QObject *parent = 0);
        /**
         * Destructor.
         */
        ~DictCore();

        /**
         * Returns true if word is exists in dictionaries,
         * otherwise false.
         */
        bool isTranslatable(const QString &word);
        /**
         * Returns translation for word. If word not found, returns
         * "Not found!"
         */
        QString translate(const QString &word);
        /**
         * Returns a list of similar words contained in dictionaries.
         */
        QStringList findSimilarWords(const QString &word);

        /**
         * Returns a list of avialable dictionary plugins.
         */
        QStringList avialablePlugins() const;

        /**
         * Returns a list of loaded dictionary plugins.
         */
        QStringList loadedPlugins() const
        { return QStringList(m_plugins.keys()); }

        /**
         * Sets a loaded plugins.
         * If plugin cannot be loaded it will not be added to
         * loadedPlugins list.
         */
        void setLoadedPlugins(const QStringList &loadedPlugins);

        /**
         * Returns a list of avialable dictionaries.
         * The first item in pair is a plugin name, the second item
         * in pair is a dictionary name.
         */
        QList<QPair<QString, QString> > avialableDicts() const;

        /**
         * Returns a list of loaded dictionaries. 
         * The first item in pair is a plugin name, the second item
         * in pair is a dictionary name.
         */
        const QList<QPair<QString, QString> > &loadedDicts() const
        { return m_loadedDicts; }

        /**
         * Sets a loaded dictionaries.
         * The first item in pair is a plugin name, the second item
         * in pair is a dictionary name.
         * If dictionary cannot be loaded it will not be added to 
         * avialableDicts list.
         */
        void setLoadedDicts(const QList<QPair<QString, QString> > &loadedDicts);

    private:
        /**
         * Save settings.
         */
        void saveSettings();

        /**
         * Load settings.
         */
        void loadSettings();

        QHash<QString, QPluginLoader*> m_plugins;
        QList<QPair<QString, QString> > m_loadedDicts;
};

}

#endif // DICTCORE_H

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

