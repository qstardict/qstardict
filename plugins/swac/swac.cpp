/*****************************************************************************
 * This file is a part of QStarDict, a StarDict clone written with using Qt  *
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

#include "swac.h"

#include <QMessageBox>
#include <QStringList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QVariant>
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include "swac-meta.h"
#endif

Swac::Swac(QObject *parent) : QObject(parent)
{
    db = new QSqlDatabase();
    *db = QSqlDatabase::addDatabase("QSQLITE", QLatin1String("swac"));
    db->setDatabaseName(QDir::homePath() + "/.swac/swac.db");
    db->open();
}

Swac::~Swac()
{
    db->close();
    delete db;
    QSqlDatabase::removeDatabase("swac");
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
QStarDict::PluginMetadata Swac::metadata() const
{
    QStarDict::PluginMetadata md;
    md.id = PLUGIN_ID;
    md.name = QString::fromUtf8(PLUGIN_NAME);
    md.version = PLUGIN_VERSION;
    md.description = PLUGIN_DESCRIPTION;
    md.authors = QString::fromUtf8(PLUGIN_AUTHORS).split(';', QString::SkipEmptyParts);
    md.features = QString::fromLatin1(PLUGIN_FEATURES).split(';', QString::SkipEmptyParts);
    return md;
}
#else
QIcon Swac::pluginIcon() const
{
    return QIcon();
}
#endif

QStringList Swac::availableDicts() const
{
    QStringList result;
    QSqlQuery query = db->exec("SELECT packid FROM packages;");
    while (query.next())
    {
        result << query.value(0).toString();
    }
    return result;
}

void Swac::setLoadedDicts(const QStringList &dicts)
{
    QStringList available = Swac::availableDicts();
    m_loadedDicts.clear();
    for (QStringList::const_iterator i = dicts.begin(); i != dicts.end(); ++i)
    {
        if (available.contains(*i))
            m_loadedDicts << *i;
    }
}

Swac::DictInfo Swac::dictInfo(const QString &dict)
{
    QSqlQuery query = db->exec("SELECT name, format, version, organization, readme FROM packages WHERE packid = \'" + dict + "\' LIMIT 1;");

    if (query.first())
        return DictInfo(query.value(0).toString(), dict, query.value(3).toString(), "<pre>" + query.value(4).toString() + "</pre>");
    else
        return DictInfo("", dict, "", "");
}


QSqlQuery Swac::search(const QString &dict, const QString &word, const QString &fields, const int limit) {
    QSqlQuery query(*db);
    query.prepare(
        "SELECT " + fields + " "
        + "FROM alphaidx" + " "
        + "INNER JOIN sounds ON alphaidx.sounds_idx = sounds.idx "
        + "INNER JOIN packages ON sounds.packages_idx = packages.idx "
        + "WHERE packages.packid = ?1 AND alphaidx.str = ?2 "
        + "LIMIT " + QString::number(limit) +";"
    );
    query.addBindValue(dict);
    query.addBindValue(word);
    query.exec();
    return query;
}



bool Swac::isTranslatable(const QString &dict, const QString &word)
{
    QSqlQuery query = search(dict, word, "SWAC_TEXT", 1);
    return query.first();
}

Swac::Translation Swac::translate(const QString &dict, const QString &word)
{
    QSqlQuery query = search(dict, word, "SWAC_TEXT, packages.path, filename, SWAC_SPEAK_NAME", 128);
    QString article("");
    int i = 0;
    while (query.next())
    {
        if (i > 0)
            article += "<br/>\n";
        article += "<img src=':/icons/sound.png'/> &nbsp; <a href=\"" + query.value(1).toString() + query.value(2).toString() + "\">" + query.value(0).toString() + "</a>";
        i++;
    }

    return Translation(word, dict, article);
}

QStringList Swac::findSimilarWords(const QString &dict, const QString &word)
{
    Q_UNUSED(dict)
    Q_UNUSED(word)
    return QStringList();
}

int Swac::execSettingsDialog(QWidget *parent)
{
    return QMessageBox::information(parent, "SWAC Plugin for QStarDict",
                            "To install new packages, please, use the <b>swac-get</b> command line program.\n"
                            "More information about swac-get is available on <a href='http://shtooka.net/'>Shtooka Project Homepage</A>." );
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(swac, Swac)
#endif

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

