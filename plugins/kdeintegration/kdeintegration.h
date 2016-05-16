/*
Ported from QtNote project
Copyright (C) 2016 Sergey Ili'nykh

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

#ifndef KDEINTEGRATION_H
#define KDEINTEGRATION_H

#include <QObject>

#include "../trayplugin.h"

class QMenu;

namespace QStarDict {

class KDEIntegration : public QObject, public QStarDict::BasePlugin, public QStarDict::TrayIconPlugin
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "org.qstardict.KDEPlugin/1.0" FILE "kdeintegration.json")
#endif
    Q_INTERFACES(QStarDict::BasePlugin QStarDict::TrayIconPlugin)
public:
    explicit KDEIntegration(QObject *parent = 0);
	~KDEIntegration();

	QString name() const
    { return "kdeintegration"; }
    QString version() const
    { return "0.01"; }
    QString description() const
    { return tr("Makes QStarDict fill more native on KDE"); }
    QStringList authors() const
    { return QStringList("Sergey Il'inykh <rion4ik@gmail.com>"); }

	TrayCompat isDECompatible();
	void initTray();
	void setContextMenu(QMenu *menu);
	void setMainWindow(QWidget *w);
	void setScanEnabled(bool enabled);
	void setVisible(bool visible);

signals:

public slots:

private:
	class Private;
	friend class Private;
	Private *d;
};

} // namespace QtNote

#endif // KDEINTEGRATION_H
