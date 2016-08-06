#ifndef QSTARDICT_APPINFO_H
#define QSTARDICT_APPINFO_H

#include <QString>

namespace QStarDict {

class AppInfo
{
public:
    static QString localAppDataDir();
    static QString systemAppDataDir();
    static QString systemAppPluginsDir();
    static QStringList appPluginsDirs();

    static QString configDir();
    static QString cacheDir();
private:
    static const QString genericDataDir();
};

} // namespace QStarDict

#endif // QSTARDICT_APPINFO_H
