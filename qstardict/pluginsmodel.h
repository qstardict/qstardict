#ifndef QSTARDICT_PLUGINSMODEL_H
#define QSTARDICT_PLUGINSMODEL_H

#include <QStandardItemModel>

namespace QStarDict {

class PluginsModel : public QStandardItemModel
{
    Q_OBJECT
public:
    enum LoadType
    {
        All,
        JustDict,
        ExceptDict
    };

    PluginsModel(QObject *parent = 0);
    void loadPluginsList(LoadType lt);
    QStringList loadedPlugins();
    QString pluginId(int row);

signals:
    void loadedListChanged();

private slots:
    void pluginsItemChanged(QStandardItem *item);
};

} // namespace QStarDict

#endif // QSTARDICT_PLUGINSMODEL_H
