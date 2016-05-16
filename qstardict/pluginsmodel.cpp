#include "pluginsmodel.h"
#include "dictcore.h"
#include "application.h"

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
    DictCore *dict = Application::instance()->dictCore();
    QStringList plugins = dict->availablePlugins();
    QStringList loaded = dict->loadedPlugins();
    int row = 0;
    for (int i = 0; i < plugins.size(); ++i)
    {
        QObject *o = dict->plugin(plugins[i]);

        if (lt == JustDict && !qobject_cast<DictPlugin*>(o))
            continue;

        if (lt == ExceptDict && qobject_cast<DictPlugin*>(o))
            continue;

        QStandardItem *item = new QStandardItem();
        item->setCheckable(true);
        item->setCheckState(loaded.contains(plugins[i]) ? Qt::Checked : Qt::Unchecked);
        setItem(row, 0, item);
        setItem(row, 1, new QStandardItem(plugins[i]));
        row++;
    }
}

void PluginsModel::pluginsItemChanged(QStandardItem *item)
{
    if (item->isCheckable())
        emit loadedListChanged();
}

QStringList PluginsModel::loadedPlugins()
{
    QStringList ret;
    int rows = rowCount();

    for (int i = 0; i < rows; ++i)
        if (item(i, 0)->checkState() == Qt::Checked)
            ret << item(i, 1)->text();

    return ret;
}

} // namespace QStarDict
