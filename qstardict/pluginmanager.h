#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QObject>
#include <QDateTime>
#include <QSharedPointer>
#include <QLibrary>
#include <QPluginLoader>

#include "../plugins/baseplugin.h"

namespace QStarDict {

class PluginServerImpl;

class PluginManager : public QObject
{
    Q_OBJECT
public:

    enum LoadError {
        LE_NoError = 0,
        LE_NotPlugin,
        LE_Abi,
        LE_Metadata,
    };

    enum PluginFeature {
        FirstFeature     = 0x1,
        RegularPlugin   = FirstFeature,
        DEIntegration   = 0x2,
        TrayIcon        = 0x4,
        GlobalShortcuts = 0x8,
        Notifications   = 010,
        LastFeature  = 020
    };
    Q_DECLARE_FLAGS(PluginFeatures, PluginFeature)

    class Plugin
    {
    public:
        typedef QSharedPointer<Plugin> Ptr;

        enum State : quint8 {
            Exist = 1, /* used during plugins search */
            Valid = 2,
            Enabled = 4
        };

        Plugin() : loader(0), pluginServer(0) {}
        QPluginLoader  *loader;
        PluginServerImpl *pluginServer;
        uint            state;
        QDateTime      modifyTime; // modificatio time of plugin library (outdated metadata check)
        PluginMetadata metadata;

        inline bool isEnabled() const { return state & Enabled; }
        inline void setEnabled(bool enabled) {
            if (enabled) state |= Enabled;
            else state &= ~Enabled;
        }
        inline bool isLoaded() const { return loader? loader->isLoaded() : false; }
        LoadError load();
        bool unload();
        void cacheIcon();

        template<class T>
        inline T* castInstance() {
            QObject *o = isLoaded()? loader->instance() : 0;
            return o? qobject_cast<T*>(o) : 0;
        }
    };

    explicit PluginManager();
    ~PluginManager();

    inline QObject *plugin(const QString &pluginId) const {
        auto pd = plugins.value(pluginId);
        return pd->isLoaded()? pd->loader->instance() : 0;
    }

    template<class T>
    inline T *plugin(const QString &pluginId) const {
        return plugins.value(pluginId)->castInstance<T>();
    }

    void loadPlugins();
    inline bool isEnabled(const QString &pluginId) const { auto pd = plugins.value(pluginId); return pd? pd->isEnabled() : false; }
    inline bool isLoaded(const QString &pluginId) const { auto pd = plugins.value(pluginId); return pd? pd->isLoaded() : false; }
    void setEnabled(const QString &pluginId, bool enabled);
    inline int pluginsCount() const { return plugins.size(); }

    QStringList availablePlugins() const;
    QStringList loadedPlugins() const;
    void setLoadedPlugins(const QStringList &pluginIds);

    Plugin::Ptr pluginDesc(const QString &pluginId) const { return plugins.value(pluginId); }
    Plugin::Ptr findPluginInstance(QObject *instance) const;

signals:
    void pluginLoaded(const QString &);

public slots:

private:
    LoadError lastError;
    PluginServerImpl *pluginServer;
    QHash<QString, Plugin::Ptr> plugins;

    void updateMetadata();
    static QString iconsCacheDir();
};

}


#endif // PLUGINMANAGER_H
