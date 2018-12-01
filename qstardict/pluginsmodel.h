#ifndef QSTARDICT_PLUGINSMODEL_H
#define QSTARDICT_PLUGINSMODEL_H

#include <QAbstractTableModel>
#include <QStringList>
#include <QIcon>

namespace QStarDict {

class PluginManager;

class PluginsModel : public QAbstractTableModel
{
    Q_OBJECT

    PluginManager *pmanager;
    QStringList pluginIds; // by priority
    QIcon settingIcon;

public:
    enum class LoadType
    {
        All,
        JustDict,
        ExceptDict
    };

    enum Roles {
        IdRole = Qt::UserRole
    };

    PluginsModel(LoadType lt, PluginManager *pmanager);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;

    QStringList loadedPlugins();
    QString pluginId(int row) const;

signals:
    void loadedListChanged();
};

} // namespace QStarDict

#endif // QSTARDICT_PLUGINSMODEL_H
