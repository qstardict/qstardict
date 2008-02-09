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

#include <string>
#include <vector>
#include <QFlags>
#include <QStringList>
#include <QVector>

class Libs;

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
         * This set of flags used to control translations.
         */
        enum TranslationFlag
        {
            /**
             * Default parametres.
             */
            None                    = 0x01,
            /**
             * Simple lookup (only exact matched words).
             */
            Simple                  = 0x02,
            /**
             * Translation in HTML format.
             */
            Html                    = 0x04,
            /**
             * Reformat simple text lists to herarchical lists (HTML only).
             */
            Reformat                = 0x08,
            /**
             * Expand dictionary abberviations to full text.
             * For example if dictionary contains translation for "_abr."
             * "abbreviation" then "_abr." in translation will be replaced to
             * "abbreviation".
             */
            ExpandAbbreviations     = 0xf0
        };
        /**
         * The flags used to control translations.
         */
        Q_DECLARE_FLAGS(TranslationFlags, TranslationFlag)

        /**
         * Construct dictionary.
         */
        DictCore(QObject *parent = 0);
        /**
         * Destructor.
         */
        ~DictCore();

        /**
         * Set directories for search dictionaries.
         */
        void setDictDirs(const QStringList &dictDirs)
        { m_dictDirs = dictDirs; }
        /**
         * Return dictionaries search path.
         */
        const QStringList& dictDirs() const
        { return m_dictDirs; }

        /**
         * Set dictionaries list. The translations will be returned in
         * orderedDicts order.
         */
        void setDicts(const QStringList &orderedDicts);
        /**
         * Return list of dictionaries.
         */
        const QStringList& orderedDicts() const
        { return m_orderedDicts; }
        /**
         * Return list of not used but avialable dicts.
         */
        QStringList disabledDicts() const;
        /**
         * Return list of avialable dictionaries in dictDirs.
         */
        QStringList avialableDicts() const;

        /**
         * Find words that looks like str (using fuzzy algoritms).
         */
        QStringList find(const QString &str);
        /**
         * Return true if str has a translation in a dictionaries,
         * otherwise return false.
         */
        bool isTranslatable(const QString &str);
        /**
         * Return translation of str. The flag parameter is used for
         * setting translation format.
         */
        QString translate(const QString &str, TranslationFlags flags = TranslationFlags(None) | Reformat);

        /**
         * Return list of dictionaries placed in dir or dir childs.
         */
        static QStringList findDicts(const QString &dir);

        /**
         * Return default local dir when dictionaries are stored.
         * On UNIX it is $HOME/.stardict/dic.
         */
        static QString localDictsDir();

    private:
        class SearchResult
        {
            public:
                QString dictName;
                QString def;
                QString exp;

                SearchResult()
                { }
                SearchResult(const QString &_dictName, const QString &_def, const QString &_exp)
                        : dictName(_dictName),
                          def(_def),
                          exp(_exp)
                { }
                SearchResult(const char *_dictName, const char *_def, const char *_exp)
                        : dictName(QString::fromUtf8(_dictName)),
                          def(QString::fromUtf8(_def)),
                          exp(QString::fromUtf8(_exp))
                { }
        };
        typedef QVector<SearchResult> SearchResultList;

        void simpleLookup(const std::string &str, SearchResultList &resultList);
        void lookupWithFuzzy(const std::string &str, SearchResultList &resultList);
        void lookupWithRule(const std::string &str, SearchResultList &resultList);
        void lookupData(const std::string &str, SearchResultList &resultList);
        QString translation(const QString &str, const QString &dict);

        Libs *m_sdLibs;
        QStringList m_dictDirs;
        QStringList m_orderedDicts;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(DictCore::TranslationFlags)

}

#endif // DICTCORE_H

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

