/*****************************************************************************
 * stardict.h - QStarDict, a StarDict clone written using Qt                 *
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

#include "lib.h"
#include "../dictplugin.h"

#include <string>
#include <QVector>
#include <QHash>


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
        { return "The StarDict plugin"; }
        QStringList authors() const
        { return QStringList()
            << "Hu Zheng <huzheng_001@163.com>"
            << "Opera Wang <wangvisual@sohu.com>"
            << "Alexander Rodin <rodin.alexander@gmail.com>"; }
        Features features() const
        { return Features(SearchSimilar | SettingsDialog); }

        QStringList availableDicts() const;
        QStringList loadedDicts() const
        { return m_loadedDicts.keys(); }
        void setLoadedDicts(const QStringList &loadedDicts);
        DictInfo dictInfo(const QString &dict);

        bool isTranslatable(const QString &dict, const QString &word);
        Translation translate(const QString &dict, const QString &word);
        virtual QStringList findSimilarWords(const QString &dict, const QString &word);

        int execSettingsDialog(QWidget *parent);

        friend class SettingsDialog;

    private:
        QString parseData(const char *data, int dictIndex = -1,
                bool htmlSpaces = false, bool reformatLists = false, bool expandAbbreviations = false);

        Libs *m_sdLibs;
        QStringList m_dictDirs;
        QHash<QString, int> m_loadedDicts;
        bool m_reformatLists;
        bool m_expandAbbreviations;
};

#endif // STARDICT_H

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent

