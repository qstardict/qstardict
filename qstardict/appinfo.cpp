#include "appinfo.h"

#include <QDir>
#include <QSettings>
#include <QCoreApplication>
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
# include <QDesktopServices>
#else
# include <QStandardPaths>
#endif
#ifdef DEVEL
# include <QDebug>
#endif

namespace QStarDict {

const QString AppInfo::genericDataDir()
{
#ifdef Q_OS_WIN
    // Force Roaming
    wchar_t path[MAX_PATH];
    typedef HRESULT (WINAPI*SHGetFolderPathWFunc)(HWND, int, HANDLE, DWORD, LPTSTR);
    SHGetFolderPathWFunc SHGetFolderPathW = (SHGetFolderPathWFunc) QLibrary::resolve(QLatin1String("Shell32"), "SHGetFolderPathW");
    if (SHGetFolderPathW(NULL, CSIDL_APPDATA, NULL, 0, path) == S_OK) {
        return QDir::fromNativeSeparators(QString::fromWCharArray(path));
    } else {
        return QString();
    }
#else
# if QT_VERSION >= 0x050000
    return QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
# else
#  if defined(Q_OS_UNIX) && !defined(Q_OS_MAC)
    QString dataDir = qgetenv("XDG_DATA_HOME");
    if (dataDir.isEmpty()) {
        dataDir = QDir::homePath() + "/.local/share";
    }
    return dataDir;
#  else
    return QDesktopServices::storageLocation(QDesktopServices::DataLocation);
#  endif
# endif
#endif
}

QString AppInfo::localAppDataDir()
{
    static QString dataDir;
    if (dataDir.isEmpty()) {
        QSettings s;
        dataDir = genericDataDir() + QLatin1Char('/') + s.applicationName();
        QDir d(dataDir);
        if (!d.exists()) {
            d.mkpath(".");
        }
    }
    return dataDir;
}

QString AppInfo::systemAppDataDir()
{
    static QString dataDir;
    if (dataDir.isEmpty()) {
#ifdef Q_OS_WIN
        dataDir = qApp->applicationDirPath();
#else
        dataDir = QString::fromLocal8Bit(QSTARDICT_DATA_DIR);
#endif
    }
    return dataDir;
}

QString AppInfo::systemAppPluginsDir()
{
    static QString libDir;
    if (libDir.isEmpty()) {
#ifdef Q_OS_WIN
        libDir = qApp->applicationDirPath() + QLatin1String("/plugins");
#elif defined(Q_OS_MAC)
        QString binPath = QCoreApplication::applicationDirPath();
        // navigate through mac's bundle tree structore
        QDir d(binPath + "/../lib/"); // TODO why not plugins?
        libDir = d.canonicalPath();
#else
        libDir = QString::fromLocal8Bit(QSTARDICT_PLUGINS_DIR);
#endif
    }
    return libDir;
}

QStringList AppInfo::appPluginsDirs()
{
#if DEVEL
    QDir pluginsDir = QDir(qApp->applicationDirPath());

# if defined(Q_OS_WIN)
    QString dbgSubdir = pluginsDir.dirName();
    if (!(dbgSubdir.toLower() == "debug" || dbgSubdir.toLower() == "release")) {
        dbgSubdir.clear();
    }
# else
    QString dbgSubdir;
# endif

# if defined(Q_OS_WIN)
    if (!dbgSubdir.isEmpty()) // if last part of app dir is debug or release
        pluginsDir.cdUp();
# elif defined(Q_OS_MAC)
    if (pluginsDir.dirName() == "MacOS") {
        pluginsDir.cdUp();
        pluginsDir.cdUp();
        pluginsDir.cdUp();
    }
# endif
    if (!(pluginsDir.cd("../plugins") || pluginsDir.cd("plugins"))) {
        qDebug() << "Can't find plugins dir directory, probably won't find plugins as well";
    }

    QStringList pluginsDirs;
    foreach (const QString &dirName, pluginsDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QDir d(pluginsDir);
        d.cd(dirName);
        if (!dbgSubdir.isEmpty()) {
            d.cd(dbgSubdir);
        }
        pluginsDirs << d.path();
    }
    qDebug() << "Plugins dirs: " << pluginsDirs;
    return pluginsDirs;
#else
    static QString localPluginsDir;
    if (localPluginsDir.isEmpty()) {
        localPluginsDir = localAppDataDir() + QLatin1String("/plugins");
    }
    QDir local(localPluginsDir);
    QDir system(systemAppPluginsDir());
    if (local == system) {
        return QStringList() << localPluginsDir;
    }
    return QStringList() << localPluginsDir << systemAppDataDir();
#endif
}

QString AppInfo::configDir()
{
    static QString confDir;
    if (confDir.isEmpty()) {
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)

# if Q_OS_MAC
        QString ch = QDir::homePath() + QLatin1String("/Library/Caches");
        confDir = ch + QLatin1Char('/') + qApp->applicationName();
# elif defined(Q_OS_LINUX)
        QString ch = QString::fromLocal8Bit(qgetenv("XDG_CACHE_HOME"));
        if (ch.isEmpty())
            ch = QDir::homePath() + QLatin1String("/.config");
        confDir = ch + QLatin1Char('/') + qApp->applicationName();
# else
        confDir = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
# endif

#elif QT_VERSION < QT_VERSION_CHECK(5,5,0)
# ifdef Q_OS_LINUX
        confDir = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) +
                QLatin1Char('/') + qApp->applicationName();
# else
        confDir = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
# endif
#else
        confDir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
#endif
        QDir c(confDir);
        if (!c.exists()) {
            c.mkpath(".");
        }
    }
    return confDir;
}

QString AppInfo::cacheDir()
{
    static QString cd;
    if (cd.isEmpty()) {
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
        cd = QDesktopServices::storageLocation(QDesktopServices::CacheLocation);
#else
        cd = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
#endif
        QDir c(cd);
        if (!c.exists()) {
            c.mkpath(".");
        }
    }
    return cd;
}

} // namespace QStarDict
