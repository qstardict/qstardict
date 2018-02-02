/*
The code is adpted from QtNote project
Copyright (C) 2010-2016 Sergey Ili'nykh

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

Contacts:
E-Mail: rion4ik@gmail.com XMPP: rion@jabber.ru
*/

/*
Algo for plugins search.
------------------------
Plugins priority:
1) Directories for plugin search have priority
   To choose between plugins with same ID.
   AppInfo::appPluginsDirs() return directories in priority order
   (most prioretized first)
   If there are two plugins with the same id in some directory,
   the one with more recent modification time will be chosen.
2) Plugins have load priority
   To decide which plugin to load first.

We have few different states of plugins
1) Discovered plugins:
   All files which look like suitable plugins.
   In this case for example we can have 2 or more copies of the same
   plugin and only one of them should become available.
   All of this data will be cached.

2) Available plugins
   Plugins ready to be loaded. All of them have unique ID.
   All not suitable dups by prioriry or other means are removed.
   This list of plugins could be available to other code as
   priority sorted list.

3) Enabled plugins
   Subset of available plugins which are supposed to be loaded.

4) Loaded plugins
   Successfully loaded plugins


Compilation mode:
   AppInfo::appPluginsDirs() can return different set of directories
depending on compilation options
1) DEVEL
   This could be set as qmake param. It's developer's mode.
   In this mode plugins search works differently. Only just compiled
   plugins' directories added to search, so only they will be loaded.

2) Normal mode
   In normal mode we have two directories for search.
   One in ~/local/share/QStarDict/plugins and another one is
   /usr/lib/QStarDict/plugins.
   The directories could have different paths dependeing on
   user and system settings.

Algo:
1) Iterate over all plugins' directories and find all suitable plugins.
2) Cache this data in order they were found
3) Iterate over cache and make a list of "available" plugins.
   In other words make do not add tot the list plugins with already
   known ID from previous iterations.
4) Sort "available" list according to priority set by user.
5) Save the list to settings for future use.
6) Iterate over sorted "available" list check which plugin is enabled
   and load it.
7) emit "pluginLoaded" signal for each loaded plugin.

Business rules:
1) Each plugin should know its priority as was discovered.
2) Plugins are not expected to heavily iteract with application
   on load, since we can load a plugin just to update metadata.
*/

#include <QSettings>
#include <QStringList>
#include <QDir>
#include <QPluginLoader>
#include <QSet>
#include <QApplication>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
# include <QJsonArray>
#endif
#include <QDebug>

#include "appinfo.h"
#include "pluginmanager.h"
#include "../plugins/pluginserver.h"

namespace QStarDict {

class PluginServerImpl : public QObject, public PluginServer
{
    Q_OBJECT
    Q_INTERFACES(QStarDict::PluginServer)

    PluginManager *pm;
public:
    PluginServerImpl(PluginManager *parent) :
        QObject(parent),
        pm(parent)
    {

    }

    const PluginMetadata &metadata(const QString &pluginId) const
    {
        return pm->pluginDesc(pluginId)->metadata;
    }

    /**
     * Return a directory that contains plugin's data.
     */
    QString configDir(const QString &pluginId) const
    {
        QString path = AppInfo::configDir() + QLatin1String("/pluginsdata/") + metadata(pluginId).name;

        if (! QDir::root().exists(path))
            QDir::root().mkpath(path);
        return path;
    }

    QString idForPlugin(QObject *instance) const
    {
        auto data = pm->findPluginInstance(instance);
        if (data) {
            return data->metadata.id;
        }
        return QString();
    }
};

class PluginsIterator
{
    QDir currentDir;
    QStringList pluginsDirs;
    QStringList plugins;
    QStringList::const_iterator pluginsDirsIt;
    QStringList::const_iterator pluginsIt;

public:
    PluginsIterator()
    {
        pluginsDirs = AppInfo::appPluginsDirs();
        pluginsDirsIt = pluginsDirs.constBegin();
        findDirWithPlugins();
    }

    void next()
    {
        if (pluginsIt != plugins.constEnd()) {
            pluginsIt++;
        }
        if (pluginsIt == plugins.constEnd() && pluginsDirsIt != pluginsDirs.constEnd()) {
            pluginsDirsIt++;
            findDirWithPlugins();
        }
    }

    inline bool isFinished() const { return pluginsIt == plugins.constEnd() &&
                pluginsDirsIt == pluginsDirs.constEnd(); }

    inline QString fileName() const
    {
        if (pluginsIt != plugins.constEnd()) {
            return QFileInfo(currentDir.filePath(*pluginsIt)).canonicalFilePath();
        }
        return QString();
    }

private:
    void findDirWithPlugins()
    {
        while (pluginsDirsIt != pluginsDirs.constEnd()) {
            currentDir = QDir(*pluginsDirsIt);
            if (currentDir.isReadable()) {
                plugins.clear();
                foreach(const QString &file, currentDir.entryList(QDir::Files, QDir::Time)) {
                    if (QLibrary::isLibrary(file)) {
                        plugins.append(file);
                    }
                }
                if (!plugins.isEmpty()) {
                    pluginsIt = plugins.constBegin();
                    return;
                }
            }
            pluginsDirsIt++;
        }
        pluginsIt = plugins.constEnd();
    }
};

PluginManager::PluginManager()
{
    pluginServer = new PluginServerImpl(this);
    QDir(iconsCacheDir()).mkpath(QLatin1String("."));
    updateMetadata();
}

PluginManager::~PluginManager()
{
    /*
    QHashIterator<QString, Plugin::Ptr> it(plugins);
    while (it.hasNext()) {
        auto item = it.next();
        if (item.value()->isLoaded()) {
            item.value()->unload();
        }
    }
    */
}

void PluginManager::loadPlugins()
{
    QSettings s;
    QStringList prioritizedList = s.value("plugins-priority").toStringList();
    QMutableStringListIterator it(prioritizedList);
    while (it.hasNext()) {
        QString id = it.next();
        if (id.isEmpty() || !plugins.contains(id)) {
            it.remove();
        }
    }
    // now prioritizedList comprises only known plugin

    // and any not previously prioritized
    prioritizedList += (plugins.keys().toSet() - prioritizedList.toSet()).toList();
    s.setValue("plugins-priority", prioritizedList);

    /*
     * now we have fully prioritized list.
     */
    QStringListIterator it2(prioritizedList);
    while (it2.hasNext()) {
        Plugin::Ptr pd = plugins[it2.next()];

        if (!pd->isEnabled() || pd->isLoaded()) {
            continue;
        }

        pd->load();
    }
}

QString PluginManager::iconsCacheDir()
{
    return AppInfo::cacheDir() + QLatin1String("/plugin-icons");
}

void PluginManager::setEnabled(const QString &pluginId, bool enabled)
{
    QSettings s;
    auto pd = plugins.value(pluginId);
    if (!pd) {
        return;
    }
    pd->setEnabled(enabled);
    s.beginGroup("plugins");
    s.beginGroup(pluginId);
    s.setValue("state", (int)pd->state);

    if (enabled) {
        if (!pd->isLoaded()) {
            if (pd->load() != LE_NoError) {
                emit pluginLoaded(pd->metadata.id);
            }
        }
    } else if (pd->isLoaded()) {
        if (!pd->unload()) {
            qWarning("Failed to unload %s: %s", qPrintable(pd->loader->fileName()), qPrintable(pd->loader->errorString()));
        }
    }
}

QStringList PluginManager::availablePlugins() const
{
    return QSettings().value("plugins-priority").toStringList();
}

QStringList PluginManager::loadedPlugins() const
{
    QStringList ret;

    foreach (const QString &pluginId, plugins.keys()) {
        auto ptr = plugins.value(pluginId);
        if (ptr->isLoaded()) {
            ret.append(pluginId);
        }
    }

    return ret;
}

void PluginManager::setLoadedPlugins(const QStringList &pluginIds)
{
    QSet<QString> ps = QSet<QString>::fromList(pluginIds);
    QMutableHashIterator<QString, Plugin::Ptr> i(plugins);

    while(i.hasNext()) {
        auto p = i.next();
        setEnabled(p.key(), ps.contains(p.key()));
    }
}

PluginManager::Plugin::Ptr PluginManager::findPluginInstance(QObject *instance) const
{
    foreach (const QString &pluginId, plugins.keys()) {
        auto ptr = plugins.value(pluginId);
        if (ptr->isLoaded() && ptr->loader->instance() == instance) {
            return ptr;
        }
    }
    return PluginManager::Plugin::Ptr();
}

void PluginManager::updateMetadata()
{
    QSettings s;
    QSettings metaCache(AppInfo::cacheDir() + QLatin1String("/pluginsmeta.ini"), QSettings::IniFormat);
    //decltype(plugins) tmpPlugins;
    QHash<QString, Plugin::Ptr> file2data;

    // read metadata cache
    int size = metaCache.beginReadArray("list");
    for (int i = 0; i < size; ++i) {
        metaCache.setArrayIndex(i);
        QString fileName = metaCache.value("filename", QString()).toString();
        if (fileName.isEmpty()) {
            break;
        }
        Plugin::Ptr pd(new Plugin);
        //tmpPlugins[pluginId] = pd;
        file2data[fileName] = pd;
        pd->state = metaCache.value("state").toUInt();
        pd->state &= ~Plugin::Exist; // reset it until we sure it exists
        pd->modifyTime = QDateTime::fromTime_t(metaCache.value("lastModify").toUInt()); // if 0 then we have staled cache. it's ok
        pd->metadata.id = metaCache.value("id").toString();
        pd->metadata.name = metaCache.value("name").toString();
        pd->metadata.description = metaCache.value("description").toString();
        pd->metadata.authors = metaCache.value("authors").toStringList();
        pd->metadata.features = metaCache.value("features").toStringList();
        pd->metadata.version = metaCache.value("version").toUInt();
        pd->metadata.extra = metaCache.value("extra").toMap();
        pd->metadata.icon = QIcon(iconsCacheDir() + '/' + pd->metadata.id + QLatin1String(".png"));
        //pd->metadata.extra = s.value("extra").();
    }
    metaCache.endArray();

    lastError = LE_NoError;
    metaCache.beginWriteArray("list");
    int cacheIndex = 0;
    PluginsIterator it;
    while (!it.isFinished()) {
        metaCache.setArrayIndex(cacheIndex);
        QString fileName = it.fileName();
        it.next();
        Plugin::Ptr pd = file2data.value(fileName);
        bool isnew = pd.isNull();
        if (isnew) {
            pd = Plugin::Ptr(new Plugin);
            pd->state |= Plugin::Exist;
        }
        pd->pluginServer = pluginServer;
        pd->loader = new QPluginLoader(fileName);

        if (isnew || (!pd->isLoaded() &&
                            pd->modifyTime != QFileInfo(pd->loader->fileName()).lastModified())) { // have to update metadata cache

            pd->modifyTime = QFileInfo(fileName).lastModified();
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
            lastError = pd->load();
            if (lastError != LE_NoError) { // new cache
                pd->state &= ~(Plugin::Valid | Plugin::Enabled);
                continue; // we were unable to do anything with this "plugin"
            }

            auto qnp = pd->castInstance<BasePlugin>();
            PluginMetadata md = qnp->metadata(); // FIXME it's qt4 only
            if (md.id.isEmpty() || md.name.isEmpty()) {
                pd->unload();
                qDebug("QStarDict plugin %s did not set metadata id or name. ignore it", qPrintable(fileName));
                lastError = LE_Metadata;
                pd->state &= ~(Plugin::Valid | Plugin::Enabled); // mark invalid and disable
                continue;
            }

            pd->metadata = md;
            pd->cacheIcon();
#else
            auto js = pd->loader->metaData().value(QLatin1String("MetaData")).toObject();
            QString id = js.value(QLatin1String("id")).toString();
            QString name = js.value(QLatin1String("name")).toString();
            if (id.isEmpty() || name.isEmpty()) {
                pd->unload();
                qDebug("QStarDict plugin %s did not set metadata id or name. ignore it", qPrintable(fileName));
                lastError = LE_Metadata;
                pd->state &= ~(Plugin::Valid | Plugin::Enabled); // mark invalid and disable
                continue;
            }

            pd->metadata.id = id;
            pd->metadata.name = name;
            pd->metadata.description = js.value(QLatin1String("description")).toString();
            pd->metadata.authors = js.value(QLatin1String("authors")).toString().split(';');
            pd->metadata.features = js.value(QLatin1String("features")).toString().split(';');
            pd->metadata.version = js.value(QLatin1String("version")).toString();
            // extra?

#endif
            if (isnew) {
                pd->setEnabled(pd->metadata.features.contains("defenable"));
            }

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
            if (!pd->isEnabled()) {
                pd->unload();
            }
#endif

            pd->state |= Plugin::Valid;

            metaCache.setValue("id", pd->metadata.id);
            metaCache.setValue("state", (int)pd->state);
            metaCache.setValue("filename", pd->loader->fileName());
            metaCache.setValue("lastModify", pd->modifyTime.toTime_t());
            metaCache.setValue("name", pd->metadata.name);
            metaCache.setValue("description", pd->metadata.description);
            metaCache.setValue("authors", pd->metadata.authors);
            metaCache.setValue("features", pd->metadata.features);
            metaCache.setValue("version", pd->metadata.version);
            metaCache.setValue("extra", pd->metadata.extra);
            cacheIndex++;
        }

        plugins.insert(pd->metadata.id, pd);
    }
    metaCache.endArray();
}

void PluginManager::Plugin::cacheIcon()
{
    if (!metadata.icon.isNull()) {
        QFileInfo fi(PluginManager::iconsCacheDir() + '/' + metadata.id + QLatin1String(".png"));
        if (!fi.exists() || fi.lastModified() < modifyTime) {
            metadata.icon.pixmap(16, 16).save(fi.filePath());
        }
    }
}

PluginManager::LoadError PluginManager::Plugin::load()
{
    if (isLoaded()) {
        return LE_NoError;
    }
#ifdef DEVEL
    qDebug("Loading plugin: %s", qPrintable(loader->fileName()));
#endif
    QObject *plugin = loader->instance();
    if (!plugin) {
        qDebug("failed to load %s : %s", qPrintable(loader->fileName()), qPrintable(loader->errorString()));
        return LE_NotPlugin;
    }
    BasePlugin *qnp = qobject_cast<BasePlugin *>(plugin);
    if (!qnp) {
        loader->unload();
        qDebug("not QStarDict plugin %s. ignore it", qPrintable(loader->fileName()));
        return LE_Abi;
    }
    qnp->qsd = pluginServer;
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    metadata.icon = qnp->pluginIcon();
    cacheIcon();
#endif

    return LE_NoError;
}

bool PluginManager::Plugin::unload()
{
    QString fileName = loader->fileName();
    if (loader->unload()) {
        delete loader;
        // probably Qt bug but "instance" method doesn't work after unload. So recreate loader.
        loader = new QPluginLoader(fileName);
        return true;
    }

    qWarning("Failed to unload plugin: %s", qPrintable(metadata.name));
    return false;
}

} // namespace QtNote

#include "pluginmanager.moc"
