/*****************************************************************************
 * test.h - QStarDict, a StarDict clone written with using Qt                *
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

#ifndef TEST_H
#define TEST_H

#include <qstardict/dictplugin.h>

#include <QHash>

class Test: public QObject, public QStarDict::DictPlugin
{
    Q_OBJECT
    Q_INTERFACES(QStarDict::DictPlugin)

    public:
        Test(QObject *parent = 0);

        QString name()
        { return "test"; }

        QString version()
        { return "1.0"; }

        Features features() 
        { return Features(SearchSimilar); }

        QStringList avialableDicts();
        QStringList loadedDicts();
        void setLoadedDicts(const QStringList &dicts);

        bool isTranslatable(const QString &dict, const QString &word);
        Translation translate(const QString &dict, const QString &word);
        QStringList findSimilarWords(const QString &dict, const QString &word);

    private:
        QStringList m_loadedDicts;
        QHash<QString, QHash<QString, QString> > m_dicts;
};

#endif // TEST_H

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent

