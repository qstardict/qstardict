#include "pluginsmodel.h"
#include "dictcore.h"
#include "application.h"
#include "pluginmanager.h"

namespace QStarDict {

PluginsModel::PluginsModel(QObject *parent) :
    QStandardItemModel(parent)
{
    setHorizontalHeaderLabels(QStringList() << tr("Enabled") << tr("Name"));

    connect(this, SIGNAL(itemChanged(QStandardItem*)),
            SLOT(pluginsItemChanged(QStandardItem*)));
}

void PluginsModel::loadPluginsList(LoadType lt)
{
    setRowCount(0);
    PluginManager *pm = Application::instance()->pluginManager();
    QStringList plugins = pm->availablePlugins();
    int row = 0;
    for (int i = 0; i < plugins.size(); ++i)
    {
        const PluginMetadata &md = pm->pluginDesc(plugins[i])->metadata;

        if (lt == JustDict && !md.features.contains("dict")) {
            continue;
        }

        if (lt == ExceptDict && md.features.contains("dict")) {
            continue;
        }

        QStandardItem *item = new QStandardItem();
        item->setData(md.id);
        item->setCheckable(true);
        item->setCheckState(pm->isLoaded(plugins[i]) ? Qt::Checked : Qt::Unchecked);
        setItem(row, 0, item);
        setItem(row, 1, new QStandardItem(plugins[i]));
        row++;
    }
}

void PluginsModel::pluginsItemChanged(QStandardItem *item)
{
    if (item->isCheckable())
        Application::instance()->pluginManager()->setEnabled(item->data().toString(), item->checkState() == Qt::Checked);
}

QStringList PluginsModel::loadedPlugins()
{
    QStringList ret;
    int rows = rowCount();

    for (int i = 0; i < rows; ++i)
        if (item(i, 0)->checkState() == Qt::Checked)
            ret << item(i, 1)->data().toString();

    return ret;
}

QString PluginsModel::pluginId(int row)
{
    return item(row, 1)->data().toString();
}

} // namespace QStarDict
