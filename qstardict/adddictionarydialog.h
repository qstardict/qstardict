/*****************************************************************************
 * adddictionarydialog.h - QStarDict, a StarDict clone written with using Qt *
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

#ifndef ADDDICTIONARYDIALOG_H
#define ADDDICTIONARYDIALOG_H

#include <QDialog>
#include "ui_adddictionarydialog.h"

#include <QHash>
#include <QList>

/**
 * A dialog that show a list of avialable dictionaries from
 * xdxf.revdanica.com repository and install selected to
 * local dictionaries directory (DictCore::localDictsDir()).
 */
class AddDictionaryDialog: public QDialog, private Ui::AddDictionaryDialog
{
    Q_OBJECT

    public:
        /**
         * Construct AddDictionaryDialog.
         */
        AddDictionaryDialog(QWidget *parent = 0);

    protected:
        void showEvent(QShowEvent*);

    private slots:
        void on_addButton_clicked();
        void on_refreshButton_clicked();
        void on_infoButton_clicked();

    private:
        void refreshDictsList(bool reload = false);

        struct Dict
        {
            QString name;
            QString langFrom;
            QString langTo;
            QString description;
            QString downLink;
            ulong size;
        };
        QList<Dict> dicts;
        QStringList mirrors;
};

#endif // ADDDICTIONARYDIALOG_H

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

