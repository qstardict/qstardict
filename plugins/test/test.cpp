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
    
}

QString Test::name() const
{
    return "test";
}

QString Test::version() const
{
    return "0.1";
}

QStringList Test::avialableDicts() const
{
    return QStringList("test01") << "test02";
}

QStringList Test::loadedDicts() const
{
    return QStringList("test01");
}

void Test::setLoadedDicts(const QStringList &dicts)
{
    Q_UNUSED(dicts)
}

bool Test::isTranslatable(const QString &dict, const QString &word)
{
    Q_UNUSED(dict)
    Q_UNUSED(word)
}

Test::Translation Test::translate(const QString &dict, const QString &word)
{
    return Translation(word, dict, "test translation");
}

Q_EXPORT_PLUGIN2(test, Test)

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

