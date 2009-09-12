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
#include <QPluginLoader>
#include "../plugins/dictplugin.h"

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
    foreach (QPluginLoader *loader, m_plugins)
    {
        delete loader->instance();
        delete loader;
    }
}

bool DictCore::isTranslatable(const QString &word)
{
    for (QList<Dictionary>::const_iterator i = m_loadedDicts.begin(); i != m_loadedDicts.end(); ++i) 
    {
        if (! m_plugins.contains(i->plugin()))
            continue;
        if (qobject_cast<DictPlugin*>(m_plugins[i->plugin()]->instance())->isTranslatable(i->name(), word))
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
        if (! m_plugins.contains(i->plugin()))
            continue;
        DictPlugin *plugin = qobject_cast<DictPlugin*>(m_plugins[i->plugin()]->instance());
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
        if (! m_plugins.contains(i->plugin()))
            continue;
        DictPlugin *plugin = qobject_cast<DictPlugin*>(m_plugins[i->plugin()]->instance());
        if (! plugin->features().testFlag(DictPlugin::SearchSimilar))
            continue;
        QStringList similar = plugin->findSimilarWords(i->name(), simplifiedWord);
        for (QStringList::const_iterator j = similar.begin(); j != similar.end(); ++j)
            if (! result.contains(*j, Qt::CaseSensitive))
                result << *j;
    }
    return result;
}

QStringList DictCore::availablePlugins() const
{
    QStringList result;
#ifdef Q_WS_X11
    QFileInfoList files = QDir(QSTARDICT_PLUGINS_DIR).entryInfoList(QStringList("lib*.so"),
                  QDir::Files | QDir::NoDotAndDotDot);
    for (QFileInfoList::const_iterator i = files.begin(); i != files.end(); ++i)
        result << i->fileName().mid(3, i->fileName().length() - 6);
#elif defined Q_WS_WIN
    QFileInfoList files = QDir(QSTARDICT_PLUGINS_DIR).entryInfoList(QStringList("*0.dll"),
                  QDir::Files | QDir::NoDotAndDotDot);
    for (QFileInfoList::const_iterator i = files.begin(); i != files.end(); ++i)
        result << i->fileName().left(i->fileName().length() - 5);
#elif defined Q_WS_MAC
    QStringList macFilters;
    // various Qt versions...
    macFilters << "*.dylib" << "*.bundle" << "*.so";
    QString binPath = QCoreApplication::applicationDirPath();
    // navigate through mac's bundle tree structore
    QDir d(binPath + "/../lib/");
    QFileInfoList files = d.entryInfoList(macFilters,
                                          QDir::Files | QDir::NoDotAndDotDot);
    for (QFileInfoList::const_iterator i = files.begin(); i != files.end(); ++i) {
        result << i->fileName();
    }
#else
#error "Function DictCore::availablePlugins() is not implemented on this platform"
#endif
    return result;
}

void DictCore::setLoadedPlugins(const QStringList &loadedPlugins)
{
    for (QHash <QString, QPluginLoader*>::iterator i = m_plugins.begin(); i != m_plugins.end(); ++i)
    {
        delete (*i)->instance();
        delete *i;
    }
    m_plugins.clear();

    for (QStringList::const_iterator i = loadedPlugins.begin(); i != loadedPlugins.end(); ++i)
    {
#ifdef Q_WS_X11
        QString pluginFilename = static_cast<QString>(QSTARDICT_PLUGINS_DIR) + "/" "lib" + *i + ".so";
#elif defined Q_WS_WIN
        QString pluginFilename = static_cast<QString>(QSTARDICT_PLUGINS_DIR) + "/" + *i + "0.dll";
#elif defined Q_WS_MAC
        // here we need to follow mac's bundle tree...
        QString pluginFilename = QDir(QCoreApplication::applicationDirPath()+"/../lib/"+*i).absolutePath();
#else
#error "Function DictCore::setLoadedPlugins(const QStringList &loadedPlugins) is not available on this platform"
#endif
        QPluginLoader *plugin = new QPluginLoader(pluginFilename);
        if (! plugin->load())
        {
            qWarning() << plugin->errorString();
            delete plugin;
            continue;
        }
        m_plugins[*i] = plugin;
    }
}

QList<DictCore::Dictionary> DictCore::availableDicts() const
{
    QList<Dictionary> result;
    for (QHash<QString, QPluginLoader*>::const_iterator i = m_plugins.begin(); i != m_plugins.end(); ++i)
    {
        DictPlugin *plugin = qobject_cast<DictPlugin*>((*i)->instance());
        QStringList dicts = plugin->availableDicts();
        for (QStringList::const_iterator j = dicts.begin(); j != dicts.end(); ++j)
            result << Dictionary(i.key(), *j);
    }
    return result;
}

void DictCore::setLoadedDicts(const QList<Dictionary> &loadedDicts)
{
    QHash<QString, QStringList> dicts;
    for (QList<Dictionary>::const_iterator i = loadedDicts.begin(); i != loadedDicts.end(); ++i)
        dicts[i->plugin()] << i->name();
    for (QHash<QString, QStringList>::const_iterator i = dicts.begin(); i != dicts.end(); ++i)
    {
        if (! m_plugins.contains(i.key()))
            continue;
        DictPlugin *plugin = qobject_cast<DictPlugin*>(m_plugins[i.key()]->instance());
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
    config.setValue("DictCore/loadedPlugins", loadedPlugins());
    QStringList rawDictsList;
    for (QList<Dictionary>::const_iterator i = m_loadedDicts.begin(); i != m_loadedDicts.end(); ++i)
        rawDictsList << i->plugin() << i->name();
    config.setValue("DictCore/loadedDicts", rawDictsList);
}

void DictCore::loadSettings()
{
    QSettings config;
    setLoadedPlugins(config.value("DictCore/loadedPlugins", availablePlugins()).toStringList());
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
    for (QHash<QString, QPluginLoader*>::const_iterator i = m_plugins.begin(); i != m_plugins.end(); ++i)
    {
        DictPlugin *plugin = qobject_cast<DictPlugin*>((*i)->instance());
        plugin->setLoadedDicts(plugin->loadedDicts());
        QStringList loadedNames = plugin->loadedDicts();
        for (QStringList::const_iterator j = loadedNames.begin(); j != loadedNames.end(); ++j)
            loaded << Dictionary(i.key(), *j);
    }
    QList<Dictionary> oldLoaded = m_loadedDicts;
    m_loadedDicts.clear();
    for (QList<Dictionary>::iterator i = oldLoaded.begin(); i != oldLoaded.end(); ++i)
        if (loaded.contains(*i))
            m_loadedDicts << *i;
}

}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

