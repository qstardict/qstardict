/*****************************************************************************
 * stardict.cpp - QStarDict, a StarDict clone written with using Qt          *
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

#include "stardict.h"

#include <map>
#include <utility>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QStack>
#include <glib/gmem.h>
#include <glib/gstrfuncs.h>
#include "lib/lib.h"
#include "lib/file.hpp"

namespace
{
std::string xdxf2html(const char *p);
std::string parse_data(const char *data);
QString html2text(QString html);
QString whereDict(const QString &name, const QString &path);
const int MaxFuzzy = 24;
}

StarDict::StarDict(QObject *parent)
    : QObject(parent)
{
    m_sdLibs = 0;
#ifdef Q_OS_UNIX
    m_dictDirs << "/usr/share/stardict/dic";
    m_dictDirs << QDir::homePath() + "/.stardict/dic";
#else
    m_dictDirs << QCoreApplication::applicationDirPath() + "/dic";
#endif // Q_OS_UNIX
}

StarDict::~StarDict()
{
    delete m_sdLibs;
}

QStringList StarDict::avialableDicts() const
{
    QStringList result;

    for (QStringList::ConstIterator dictDir = m_dictDirs.begin(); dictDir != m_dictDirs.end(); ++dictDir)
        result << findDicts(*dictDir);

    return result;
}

void StarDict::setLoadedDicts(const QStringList &loadedDicts)
{
    delete m_sdLibs; // yes, it's ugly
    m_sdLibs = new Libs;

    for (QStringList::const_iterator dictName = loadedDicts.begin(); dictName != loadedDicts.end(); ++dictName)
        for (QStringList::const_iterator dictDir = m_dictDirs.begin(); dictDir != m_dictDirs.end(); ++dictDir)
        {
            QString dictFile = whereDict(*dictName, *dictDir);
            if (! dictFile.isEmpty())
                m_sdLibs->load_dict(QDir::toNativeSeparators(dictFile).toUtf8().data());
        }
    m_loadedDicts.clear();
    for (int i = 0; i < m_sdLibs.ndicts(); ++i)
        m_loadedDicts[m_sdLibs.dict_name(i)] = i;
}

StarDict::DictInfo dictInfo(const QString &dict)
{
    ::DictInfo nativeInfo;
    if (! nativeInfo.load_from_ifo_file(whereDict(dict), false))
        return DictInfo();
    DictInfo result(name(), dict);
    result.setFullName(QString::fromUtf8(nativeInfo.bookname.c_str()));
    result.setAuthor(QString::fromUtf8(nativeInfo.author.c_str()));
    result.setEmail(QString::fromUtf8(nativeInfo.email.c_str()));
    result.setWebSite(QString::fromUtf8(nativeInfo.webSite.c_str()));
    result.setDescription(QString::fromUtf8(nativeInfo.description.c_str()));
    result.setWordsCount(static_cast<long>(wordcount));
    return result;
}

bool StarDict::isTranslatable(const QString &dict, const QString &word)
{
    if (! m_loadedDicts.contains(dict))
        return false;
    long ind;
    return m_sdLibs->SimpleLookupWord(word.simplified().toUtf8().data(), ind, m_loadedDicts[dict]);
}

Translation StarDict::translate(const QString &dict, const QString &word)
{

}

QStringList StarDict::findSimilarWords(const QString &dict, const QString &word)
{
    
}

void StarDict::execSettingsDialog()
{
    
}

void StarDict::simpleLookup(const std::string &str, SearchResultList &resultList)
{
    
}

void StarDict::lookupWithFuzzy(const std::string &str, SearchResultList &resultList)
{
    
}

void StarDict::lookupWithRule(const std::string &str, SearchResultList &resultList)
{
    
}

void StarDict::lookupData(const std::string &str, SearchResultList &resultList)
{
    
}

QString StarDict::translation(const QString &str, const QString &dict)
{
    
}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

