/*****************************************************************************
 * This file is a part of QStarDict, a StarDict clone written using Qt       *
 * multitran.cpp - Plugin for multitran-data (multitran.sf.net)              *
 * Copyright (C) 2008 Nick Shaforostoff                                      *
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

#ifndef MULTITRAN_H
#define MULTITRAN_H

#include "../dictplugin.h"

#include <string>
#include <QVector>
#include <QHash>

class Multitran: public QObject, public QStarDict::DictPlugin
{
    Q_OBJECT
    Q_INTERFACES(QStarDict::DictPlugin)

    public:
        Multitran(QObject *parent = 0);
        ~Multitran();

        QString name() const
        { return "multitran"; }
        QString version() const
        { return "0.1"; }
        QString description() const
        { return "The Multitran plugin"; }
        QStringList authors() const
        { return QStringList()
            << tr("Stanislav Ievlev <inger@altlinux.org>")
            << tr("Nick Shaforostoff <shaforostoff@kde.ru>"); }
        Features features() const
        { return Features(/*SearchSimilar | SettingsDialog*/); }

        QStringList availableDicts() const;
        QStringList loadedDicts() const {return QStringList("Multitran");}//{ return m_loadedDicts.keys(); }
        void setLoadedDicts(const QStringList &loadedDicts);
        DictInfo dictInfo(const QString &dict);

        bool isTranslatable(const QString &dict, const QString &word);
        Translation translate(const QString &dict, const QString &word);
        virtual QStringList findSimilarWords(const QString &dict, const QString &word);

        int execSettingsDialog(QWidget *parent);

        //friend class SettingsDialog;

    private:
        QStringList m_dictDirs;
        QHash<QString, int> m_loadedDicts;
};

#endif // MULTITRAN_H

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent

