/*****************************************************************************
 * test.cpp - QStarDict, a StarDict clone written with using Qt              *
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

#include "test.h"

Test::Test(QObject *parent)
    : QObject(parent)
{
    m_dicts["Test 01"]["Linux"] = "Kernel of GNU/Linux operating system";
    m_dicts["Test 01"]["Linus"] = "Creator of Linux kernel";
    m_dicts["Test 02"]["cat"] = "A lazy and fluffy pet";
    m_dicts["Test 02"]["Linux"] = "A cat's name";
}

QStringList Test::avialableDicts() const
{
    return QStringList("Test 01") << "Test 02";
}

QStringList Test::loadedDicts() const
{
    return m_loadedDicts;
}

void Test::setLoadedDicts(const QStringList &dicts)
{
    m_loadedDicts.clear();
    foreach (QString str, dicts)
        if (m_dicts.contains(str))
            m_loadedDicts << str;
}

bool Test::isTranslatable(const QString &dict, const QString &word)
{
    if (! m_loadedDicts.contains(dict))
        return false;
    return m_dicts[dict].contains(word);
}

QStarDict::DictPlugin::Translation Test::translate(const QString &dict, const QString &word)
{
    if (! isTranslatable(dict, word))
        return Translation();
    return Translation(word, dict, m_dicts[dict][word]);
}

QStringList Test::findSimilarWords(const QString &dict, const QString &word)
{
    Q_UNUSED(word)
    if (! m_loadedDicts.contains(dict))
        return QStringList();
    return m_dicts[dict].keys();
}

Q_EXPORT_PLUGIN2(Test, Test)

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

