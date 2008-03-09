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
#include <QDir>
#include <QCoreApplication>
#include "dictinfo.h"

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
                 * Constructs a translation from data.
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
         * Destructor.
         */
        virtual ~DictPlugin() { }

        /**
         * Returns a plugin name.
         */
        virtual QString name() const = 0;

        /**
         * Returns a plugin version.
         */
        virtual QString version() const = 0;

        /**
         * Returns a plugin description.
         */
        virtual QString description() const = 0;

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

        /**
         * Returns true if translation exists in dictionary,
         * otherwise returns false.
         */
        virtual bool isTranslatable(const QString &dict, const QString &word) = 0;
        /**
         * Returns translation for word from dictionary. If word not found
         * returns empty string.
         */
        virtual Translation translate(const QString &dict, const QString &word) = 0;
        /**
         * Returns a list of similar to "word" words from all loaded dictionaries.
         * Works only if SearchSimilar feature is enabled.
         */
        virtual QStringList findSimilarWords(const QString &word)
        { return QStringList(word); }

        /**
         * Returns an information about dictionary. The dictionary may be not loaded
         * but avialable.
         */
        virtual DictInfo dictInfo(const QString &dict) = 0;

        /**
         * Run a settings dialog.
         */
        virtual void execSettingsDialog(QWidget *parent = 0)
        { Q_UNUSED(parent) }

    protected:
        /**
         * Returns a directory that contains plugin's data.
         */
        QString workPath() const
        {
#ifdef Q_OS_UNIX
            QString path = QDir::homePath() + "/.qstardict/pluginsdata/" + name();
#elif define Q_OS_WIN32
            QString path = QDir::QCoreApplication::applicationDirPath() + "\\pluginsdata\\" + name();
#else
#error "This function is not implemented on this platform"
#endif
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

