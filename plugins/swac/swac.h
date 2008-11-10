/*****************************************************************************
 * This file is a part of QStarDict, a StarDict clone written using Qt       *
 * swac.cpp - Plugin for words audio collections SWAC                        *
 * Copyright (C) 2008 Nicolas Vion <nico@picapo.net>                         *
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

#ifndef SWAC_H
#define SWAC_H

#include "../dictplugin.h"

#include <QSqlDatabase>
#include <QSqlQuery>

class Swac: public QObject, public QStarDict::DictPlugin
{
    Q_OBJECT
    Q_INTERFACES(QStarDict::DictPlugin)

public:
    Swac(QObject *parent = 0);
    ~Swac();

    QString name() const
    { return "swac"; }
    QString version() const
    { return "0.1"; }
    QString description() const
    { return tr("An experimental plugin for words audio collections (SWAC).<br>For more information about SWAC, please, visit the <a href='http://shtooka.net/'>Shtooka Project Homepage</a>."); }
    QStringList authors() const
    { return QStringList("Nicolas Vion <nico@picapo.net>"); }
    Features features() const
    { return Features(SearchSimilar | SettingsDialog); }

    QStringList availableDicts() const;
    QStringList loadedDicts() const
    { return m_loadedDicts; }
    void setLoadedDicts(const QStringList &dicts);
    DictInfo dictInfo(const QString &dict);

    bool isTranslatable(const QString &dict, const QString &word);
    Translation translate(const QString &dict, const QString &word);
    QStringList findSimilarWords(const QString &dict, const QString &word);
 
    int execSettingsDialog(QWidget *parent);

//    friend class SettingsDialog;

private:
    QSqlQuery search(const QString &dict, const QString &word, const QString &fields, const int limit);

    QSqlDatabase *db;
    QStringList m_loadedDicts;
};

#endif 

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

