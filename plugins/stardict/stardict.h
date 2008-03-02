/*****************************************************************************
 * stardict.h - QStarDict, a StarDict clone written with using Qt            *
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

#ifndef STARDICT_H
#define STARDICT_H

#include <qstardict/dictplugin.h>

#include <string>
#include <QVector>
#include <QHash>
#include "lib.h"

class StarDict: public QObject, public QStarDict::DictPlugin
{
    Q_OBJECT
    Q_INTERFACES(QStarDict::DictPlugin)
    
    public:
        StarDict(QObject *parent = 0);
        ~StarDict();

        QString name() const
        { return "stardict"; }
        QString version() const
        { return "0.1"; }
        QString description() const
        { return tr("The StarDict plugin"); }
        Features features() const
        { return Features(SearchSimilar | SettingsDialog); }

        QStringList avialableDicts() const;
        QStringList loadedDicts() const
        { return m_loadedDicts.keys(); }
        void setLoadedDicts(const QStringList &loadedDicts);
        DictInfo dictInfo(const QString &dict);

        bool isTranslatable(const QString &dict, const QString &word);
        Translation translate(const QString &dict, const QString &word);
        virtual QStringList findSimilarWords(const QString &dict, const QString &word);

        void execSettingsDialog();

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
        QHash<QString, int> m_loadedDicts;
};

#endif // STARDICT_H

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent

