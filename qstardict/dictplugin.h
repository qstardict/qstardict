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

#include <QObject>

#include <QStringList>

namespace QStarDict
{

/**
 * This is a base class for all dictionary plugins classes.
 */
class DictPlugin: public QObject
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
            SearchSimilar = 0x01
        };
        Q_DECLARE_FLAGS(Features, Feature)

        /**
         * Constructor.
         */
        DictPlugin(QObject *parent = 0)
            : QObject(parent),
              m_features(None)
        { }
        /**
         * Destructor
         */
        virtual ~DictPlugin()
        { }

        /**
         * Returns a features supported by dictionary plugin.
         */
        Features features() const
        { return m_features; }

        /*
         * Returns true if translation exists in dictionary,
         * otherwise return false.
         */
        virtual bool isTranslatable(const QString &word) = 0;
        /**
         * Returns translation for word. If word not found returns
         * empty string.
         */
        virtual QString translate(const QString &word) = 0;
        /**
         * Returns list of similar to "word" words. Works only if
         * SearchSimilar feature is enabled.
         */
        virtual QStringList findSimilarWords(const QString &word)
        { return QStringList(word); }

    protected:
        void setFeatures(Features features)
        { m_features = features; }

    private:
        Features m_features;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(DictPlugin::Features)

}

#endif // DICTPLUGIN_H

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent

