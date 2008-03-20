/*****************************************************************************
 * settingsdialog.h - QStarDict, a StarDict clone written with using Qt      *
 * Copyright (C) 2008 Alexander Rodin                                        *
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

#include "web.h"

class SettingsDialog: public QDialog, public Ui::SettingsDialog
{
    Q_OBJECT
    public:
        SettingsDialog(Web *plugin, QWidget *parent = 0);

        void accept();

    private slots:
        void on_editDictButton_clicked();
        void on_addDictButton_clicked();
        void on_removeDictButton_clicked();

    private:
        void refresh();

        struct Dict
        {
            Dict(const QString &a = QString(),
                const QString &d = QString(),
                const QString &q = QString(),
                const QByteArray &c = QByteArray())
                : author(a),
                  description(d),
                  query(q),
                  charset(c)
            { }
            QString author;
            QString description;
            QString query;
            QByteArray charset;
        };
        QHash<QString, Dict> m_oldDicts;
        QHash<QString, Dict> m_dicts;
        Web *m_plugin;
};

#endif // SETTINGSDIALOG_H

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent

