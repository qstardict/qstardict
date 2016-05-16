#include <QHeaderView>

#include "pluginsview.h"

namespace QStarDict {

PluginsView::PluginsView(QWidget *parent) :
    QTableView(parent)
{
    verticalHeader()->hide();
    setColumnWidth(0, 60);
    setColumnWidth(1, 320);
}

} // namespace QStarDict
