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
class DictCore: public QObject
{
    Q_OBJECT

    public:
        enum TranslationFlag
        {
            None                    = 0x01,
            Simple                  = 0x02,
            Html                    = 0x04,
            Reformat                = 0x08,
            ExpandAbbreviations     = 0xf0
        };
        Q_DECLARE_FLAGS(TranslationFlags, TranslationFlag)

        DictCore(QObject *parent = 0);
        ~DictCore();

        void setDictDirs(const QStringList &dictDirs)
        { m_dictDirs = dictDirs; }
        const QStringList& dictDirs() const
        { return m_dictDirs; }

        void setDicts(const QStringList &orderedDicts);
        const QStringList& orderedDicts() const
        { return m_orderedDicts; }
        QStringList disabledDicts() const;
        QStringList avialableDicts() const;

        QStringList find(const QString &str);
        bool isTranslatable(const QString &str);
        QString translate(const QString &str, TranslationFlags flags = TranslationFlags(None) | Reformat);

        static QStringList findDicts(const QString &dir);

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
