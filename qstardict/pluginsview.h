#ifndef QSTARDICT_PLUGINSVIEW_H
#define QSTARDICT_PLUGINSVIEW_H

#include <QTableView>

namespace QStarDict {

class PluginsView : public QTableView
{
    Q_OBJECT
public:
    PluginsView(QWidget *parent = 0);
};

} // namespace QStarDict

#endif // QSTARDICT_PLUGINSVIEW_H
