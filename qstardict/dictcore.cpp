/*****************************************************************************
 * dictcore.cpp - QStarDict, a StarDict clone written with using Qt          *
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

#include <QFileInfoList>
#include <QDir>
#include <QRegExp>

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
        delete loader;
}

bool DictCore::isTranslatable(const QString &word)
{
    Q_UNUSED(word)
}

QString DictCore::translate(const QString &word)
{
    Q_UNUSED(word)
}

QStringList DictCore::findSimilarWords(const QString &word)
{
    Q_UNUSED(word)
}

QStringList DictCore::avilablePlugins() const
{
    QStringList result;
#ifdef Q_OS_UNIX
    QFileInfoList files = QDir(QSTARDICT_PLUGINS_DIR).entryInfoList(QStringList("*.so"),
                  QDir::Files | QDir::NoDotAndDotDot);
    for (QFileInfoList::const_iterator i = files.begin(); i != files.end(); ++i)
        result << i->fileName().remove(QRegExp("^lib")).remove(QRegExp("\\.so"));
#else
#error "Function DictCore::avialablePlugins() is not implemented on this platform"
#endif
    return result;
}

void DictCore::setLoadedPlugins(const QStringList &loadedPlugins)
{
    QHash <QString, QPluginLoader*> oldPlugins = m_plugins;
    m_plugins.clear();
    for (QStringList::const_iterator i = loadedPlugins.begin(); i != loadedPlugins.end(); ++i)
    {
        if (oldPlugins.contains(*i))
        {
            m_plugins[*i] = oldPlugins[*i];
            oldPlugins.remove(*i);
            continue;
        }
#ifdef Q_OS_UNIX
        QString pluginFilename = QSTARDICT_PLUGINS_DIR "/" "lib" + *i + ".so";
#else
#error "Function DictCore::setLoadedPlugins(const QStringList &loadedPlugins) is not avialable on this platform"
#endif
        QPluginLoader *plugin = new QPluginLoader(pluginFilename);
        if (! plugin->load())
        {
            delete plugin;
            continue;
        }
        m_plugins[*i] = plugin;
    }
    foreach (QPluginLoader *i, oldPlugins)
        delete i;
}

QList<QPair<QString, QString> > DictCore::avialableDicts() const
{
    QList<QPair<QString, QString> > result;
}

void DictCore::setLoadedDicts(const QList<QPair<QString, QString> > &loadedDicts)
{
    
}

void DictCore::saveSettings()
{
    
}

void DictCore::loadSettings()
{
    
}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc
