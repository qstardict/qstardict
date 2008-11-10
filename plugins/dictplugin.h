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

#ifndef DICTPLUGIN_H
#define DICTPLUGIN_H

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
class DictPlugin
{
    public:
        /**
         * This enum describes a features of dictionary plugin.
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
         * This class represents information about dictionary.
         */
        class DictInfo
        {
            public:
                /**
                 * Construct empty DictInfo object.
                 */
                DictInfo()
                    : m_wordsCount(-1L)
                { }
                /**
                 * Construct DictInfo object from data.
                 * @param plugin A plugin name
                 * @param name A dictionary name
                 * @param author A dictionary author
                 * @param desription A dictionary description
                 * @param wordsCount A count of words that available in dictionary
                 */
                DictInfo(const QString &plugin,
                         const QString &name,
                         const QString &author = QString(),
                         const QString &description = QString(),
                         long wordsCount = -1L)
                    : m_plugin(plugin),
                      m_name(name),
                      m_author(author),
                      m_description(description),
                      m_wordsCount(wordsCount)
                { }

                const QString &plugin() const
                { return m_plugin; }
                const QString &name() const
                { return m_name; }
                const QString &author() const
                { return m_author; }
                const QString &description() const
                { return m_description; }
                long wordsCount() const
                { return m_wordsCount; }

                void setPlugin(const QString &plugin)
                { m_plugin = plugin; }
                void setName(const QString &name)
                { m_name = name; }
                void setAuthor(const QString &author)
                { m_author = author; }
                void setDescription(const QString &description)
                { m_description = description; }
                void setWordsCount(long wordsCount)
                { m_wordsCount = wordsCount; }

            private:
                QString m_plugin;
                QString m_name;
                QString m_author;
                QString m_description;
                long m_wordsCount;
        };

        /**
         * This class represent a translation.
         */
        class Translation
        {
            public:
                /**
                 * Construct an empty translation.
                 */
                Translation()
                { }

                /**
                 * Construct a translation from data.
                 * @param title A translation title
                 * @param dictName A full dictionary name
                 * @param translation A translation
                 */
                Translation(const QString &title,
                        const QString &dictName,
                        const QString &translation)
                    : m_title(title),
                      m_dictName(dictName),
                      m_translation(translation)
                { }

                /**
                 * Return the translation title.
                 */
                const QString &title() const
                { return m_title; }

                /**
                 * Return the dictionary name.
                 */
                const QString &dictName() const
                { return m_dictName; }

                /**
                 * Return the translation.
                 */
                const QString &translation() const
                { return m_translation; }

                /**
                 * Set a translation title.
                 */
                void setTitle(const QString &title)
                { m_title = title; }

                /**
                 * Set a dictionary name.
                 */
                void setDictName(const QString &dictName)
                { m_dictName = dictName; }

                /**
                 * Set a translation.
                 */
                void setTranslation(const QString &translation)
                { m_translation = translation; }

            private:
                QString m_title;
                QString m_dictName;
                QString m_translation;
        };

        /**
         * Destructor.
         */
        virtual ~DictPlugin() { }

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

        /**
         * Return a features supported by dictionary plugin.
         */
        virtual Features features() const
        { return Features(None); }

        /**
         * Return a list of available dictionaries.
         */
        virtual QStringList availableDicts() const = 0;

        /**
         * Return a list of loaded dictionaries.
         */
        virtual QStringList loadedDicts() const = 0;

        /**
         * Set a list of loaded dictionaries.
         */
        virtual void setLoadedDicts(const QStringList &loadedDicts) = 0;

        /**
         * Return true if translation exists in dictionary,
         * otherwise returns false.
         */
        virtual bool isTranslatable(const QString &dict, const QString &word) = 0;
        /**
         * Return translation for word from dictionary. If word not found
         * returns empty string.
         */
        virtual Translation translate(const QString &dict, const QString &word) = 0;
        /**
         * Return a list of similar to "word" words from all loaded dictionaries.
         * Works only if SearchSimilar feature is enabled.
         */
        virtual QStringList findSimilarWords(const QString &dict, const QString &word)
        { Q_UNUSED(dict); return QStringList(word); }
        
        /**
         * Return a required resource. Scheme of URLs:
         *   plugin://plugin_name/...
         */
        virtual QVariant resource(int type, const QUrl &name)
        { Q_UNUSED(type) Q_UNUSED(name) return QVariant(); }

        /**
         * Return an information about dictionary. The dictionary may be not loaded
         * but available.
         */
        virtual DictInfo dictInfo(const QString &dict) = 0;

        /**
         * Run a settings dialog and return QDialog::DialogCode.
         */
        virtual int execSettingsDialog(QWidget *parent = 0)
        { Q_UNUSED(parent); return 0; }

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

Q_DECLARE_OPERATORS_FOR_FLAGS(DictPlugin::Features)

}

Q_DECLARE_INTERFACE(QStarDict::DictPlugin, "org.qstardict.DictPlugin/1.0")

#endif // DICTPLUGIN_H

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent

