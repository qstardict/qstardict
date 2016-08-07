/*****************************************************************************
 * settingsdialog.h - QStarDict, a StarDict clone written with using Qt      *
 * Copyright (C) 2007 Alexander Rodin                                        *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 2 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License along   *
 * with this program; if not, write to the Free Software Foundation, Inc.,   *
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.               *
 *****************************************************************************/

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include "ui_settingsdialog.h"

#include "dictcore.h"

class QStandardItemModel;
class QStandardItem;

namespace QStarDict
{
class MainWindow;
class PluginsModel;

/**
 * The settings dialog.
 */
class SettingsDialog: public QDialog, private Ui::SettingsDialog
{
    Q_OBJECT

    public:
        /**
         * Construct new SettingsDialog widget with parent MainWindow.
         */
        SettingsDialog(QWidget *parent = 0);

        void accept();
        void reject();

    private slots:
        void on_dictsMoveUpButton_clicked();
        void on_dictsMoveDownButton_clicked();
        void on_dictsShowInfoButton_clicked();

    private slots:
        void loadDictsList();
        void dictLoadedPluginsChanged();        
        void pluginClicked(const QModelIndex &index);
private:
        QStringList m_oldPlugins;
        QList<DictCore::Dictionary> m_oldDicts;
        QStandardItemModel *m_dictsModel;
        PluginsModel *m_dictPluginsModel;
        PluginsModel *m_miscPluginsModel;
};

}

#endif // SETTINGSDIALOG_H

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

