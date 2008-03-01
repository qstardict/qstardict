/*****************************************************************************
 * dictplugin.h - QStarDict, a StarDict clone written with using Qt          *
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

#ifndef DICTPLUGIN_H
#define DICTPLUGIN_H

#include <QtPlugin>
#include <QStringList>

namespace QStarDict
{

/**
 * This is a base class for all dictionary plugins classes.
 */
class DictPlugin
{
    public:
        /**
         * This enum describe a features of dictionary plugin.
         */
        enum Feature
        {
            /**
             * No features.
             */
            None          = 0x00,
            /**
             * Dictionary plugin can search for similar words using
             * fuzzy algoritms.
             */
            SearchSimilar = 0x01,
            /**
             * Dictionary plugin has a settings dialog.
             */
            SettingsDialog = 0x02,
        };
        Q_DECLARE_FLAGS(Features, Feature)

        /**
         * This class represents a translation.
         */
        class Translation
        {
            public:
                /**
                 * Constructs an empty translation.
                 */
                Translation()
                { }

                /**
                 * Constructs a translation from strings.
                 */
                Translation(const QString &title,
                        const QString &dictName,
                        const QString &translation)
                    : m_title(title),
                      m_dictName(dictName),
                      m_translation(translation)
                { }

                /**
                 * Returns a translation title.
                 */
                const QString &title() const
                { return m_title; }

                /**
                 * Returns a dictionary name.
                 */
                const QString &dictName() const
                { return m_dictName; }

                /**
                 * Returns a translation.
                 */
                const QString &translation() const
                { return m_translation; }

                /**
                 * Sets a translation title.
                 */
                void setTitle(const QString &title)
                { m_title = title; }

                /**
                 * Sets a dictionary name.
                 */
                void setDictName(const QString &dictName)
                { m_dictName = dictName; }

                /**
                 * Sets a translation.
                 */
                void setTranslation(const QString &translation)
                { m_translation = translation; }

            private:
                QString m_title;
                QString m_dictName;
                QString m_translation;
        };

        /**
         * Destructor
         */
        virtual ~DictPlugin() { }

        /*
         * Returns a plugin name.
         */
        virtual QString name() const = 0;

        /**
         * Returns a plugin version.
         */
        virtual QString version() const = 0;

        /**
         * Returns a features supported by dictionary plugin.
         */
        virtual Features features() const
        { return Features(None); }

        /**
         * Returns a list of avialable dictionaries.
         */
        virtual QStringList avialableDicts() const = 0;

        /**
         * Returns a list of loaded dictionaries.
         */
        virtual QStringList loadedDicts() const = 0;

        /**
         * Set a list of loaded dictionaries.
         */
        virtual void setLoadedDicts(const QStringList &loadedDicts) = 0;

        /*
         * Returns true if translation exists in dictionary,
         * otherwise return false.
         */
        virtual bool isTranslatable(const QString &dict, const QString &word) = 0;
        /**
         * Returns translation for word from dictionary. If word not found
         * returns empty string.
         */
        virtual Translation translate(const QString &dict, const QString &word) = 0;
        /**
         * Returns a list of similar to "word" words form dictionary. Works only if
         * SearchSimilar feature is enabled.
         */
        virtual QStringList findSimilarWords(const QString &dict, const QString &word)
        { Q_UNUSED(dict) return QStringList(word); }

        /**
         * Run a settings dialog.
         */
        virtual void execSettingsDialog()
        { };
};

Q_DECLARE_OPERATORS_FOR_FLAGS(DictPlugin::Features)

}

Q_DECLARE_INTERFACE(QStarDict::DictPlugin, "org.qstardict.DictPlugin/1.0")

#endif // DICTPLUGIN_H

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent

