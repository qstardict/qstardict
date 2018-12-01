/*****************************************************************************
 * dictcore.cpp - QStarDict, a StarDict clone written using Qt               *
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

#include "dictcore.h"

#include <QApplication>
#include <QFileInfoList>
#include <QFileInfo>
#include <QDir>
#include <QRegExp>
#include <QSettings>
#include <QDebug>
#include "../plugins/dictplugin.h"
#include "application.h"
#include "pluginmanager.h"

namespace QStarDict
{

DictCore::DictCore(QObject *parent)
    : QObject(parent)
{
    loadSettings();
}

DictCore::~DictCore()
{
    saveSettings();
}

bool DictCore::isTranslatable(const QString &word)
{
    for (QList<Dictionary>::const_iterator i = m_loadedDicts.begin(); i != m_loadedDicts.end(); ++i)
    {
        auto plugin = Application::instance()->pluginManager()->plugin<DictPlugin>(i->plugin());
        if (!plugin)
            continue;
        if (plugin->isTranslatable(i->name(), word))
            return true;
    }
    return false;
}

QString DictCore::translate(const QString &word)
{
    QString simplifiedWord = word.simplified();
    QString result;
    for (QList<Dictionary>::const_iterator i = m_loadedDicts.begin(); i != m_loadedDicts.end(); ++i)
    {
        auto plugin = Application::instance()->pluginManager()->plugin<DictPlugin>(i->plugin());
        if (!plugin)
            continue;
        if (! plugin->isTranslatable(i->name(), simplifiedWord))
            continue;
        DictPlugin::Translation translation = plugin->translate(i->name(), simplifiedWord);
        result += "<p>\n"
                  "<font class=\"dict_name\">" + translation.dictName() + "</font><br>\n"
                                                                          "<font class=\"title\">" + translation.title() + "</font><br>\n"
                + translation.translation() + "</p>\n";
    }
    return result;
}

QStringList DictCore::findSimilarWords(const QString &word)
{
    QString simplifiedWord = word.simplified();
    QStringList result;
    for (QList<Dictionary>::const_iterator i = m_loadedDicts.begin(); i != m_loadedDicts.end(); ++i)
    {
        auto plugin = Application::instance()->pluginManager()->plugin<DictPlugin>(i->plugin());
        if (!plugin)
            continue;
        if (! plugin->features().testFlag(DictPlugin::Feature::SearchSimilar))
            continue;
        QStringList similar = plugin->findSimilarWords(i->name(), simplifiedWord);
        for (QStringList::const_iterator j = similar.begin(); j != similar.end(); ++j)
            if (! result.contains(*j, Qt::CaseSensitive))
                result << *j;
    }
    return result;
}

QList<DictCore::Dictionary> DictCore::availableDicts() const
{
    QList<Dictionary> result;
    auto pm = Application::instance()->pluginManager();
    foreach (const QString &pluginId, pm->availablePlugins()) {
        auto plugin = pm->plugin<DictPlugin>(pluginId);
        if (plugin) {
            QStringList dicts = plugin->availableDicts();
            for (QStringList::const_iterator j = dicts.begin(); j != dicts.end(); ++j)
                result << Dictionary(pluginId, *j);
        }
    }
    return result;
}

void DictCore::setLoadedDicts(const QList<Dictionary> &loadedDicts)
{
    auto pm = Application::instance()->pluginManager();
    QHash<QString, QStringList> dicts;
    for (QList<Dictionary>::const_iterator i = loadedDicts.begin(); i != loadedDicts.end(); ++i)
        dicts[i->plugin()] << i->name();
    for (QHash<QString, QStringList>::const_iterator i = dicts.begin(); i != dicts.end(); ++i)
    {
        auto plugin = pm->plugin<DictPlugin>(i.key());
        if (!plugin)
            continue;
        plugin->setLoadedDicts(*i);
        dicts[i.key()] = plugin->loadedDicts();
    }
    m_loadedDicts.clear();
    for (QList<Dictionary>::const_iterator i = loadedDicts.begin(); i != loadedDicts.end(); ++i)
        if (dicts.contains(i->plugin()) && dicts[i->plugin()].contains(i->name()))
            m_loadedDicts << *i;
}

void DictCore::saveSettings()
{
    QSettings config;
    QStringList rawDictsList;
    for (QList<Dictionary>::const_iterator i = m_loadedDicts.begin(); i != m_loadedDicts.end(); ++i)
        rawDictsList << i->plugin() << i->name();
    config.setValue("DictCore/loadedDicts", rawDictsList);
}

void DictCore::loadSettings()
{
    QSettings config;
    QStringList rawDictsList = config.value("DictCore/loadedDicts").toStringList();
    if (rawDictsList.isEmpty())
        setLoadedDicts(availableDicts());
    else
    {
        QList<Dictionary> dicts;
        for (QStringList::const_iterator i = rawDictsList.begin(); i != rawDictsList.end(); i += 2)
            dicts << Dictionary(*i, *(i + 1));
        setLoadedDicts(dicts);
    }
}

void DictCore::reloadDicts()
{
    QList<Dictionary> loaded;
    foreach (const QString &pluginId, Application::instance()->pluginManager()->availablePlugins())
    {
        DictPlugin *plugin = Application::instance()->pluginManager()->plugin<DictPlugin>(pluginId);
        if (!plugin) {
            continue;
        }
        plugin->setLoadedDicts(plugin->loadedDicts());
        QStringList loadedNames = plugin->loadedDicts();
        for (QStringList::const_iterator j = loadedNames.begin(); j != loadedNames.end(); ++j)
            loaded << Dictionary(pluginId, *j);
    }
    QList<Dictionary> oldLoaded = m_loadedDicts;
    m_loadedDicts.clear();
    for (QList<Dictionary>::iterator i = oldLoaded.begin(); i != oldLoaded.end(); ++i)
        if (loaded.contains(*i))
            m_loadedDicts << *i;
}

}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

