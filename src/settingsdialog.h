/********************************************************************************
 * settingsdialog.h - QStarDict, a StarDict clone written with using Qt         *
 * Copyright (C) 2007 Alexander Rodin                                           *
 *                                                                              *
 * This program is free software: you can redistribute it and/or modify         *
 * it under the terms of the GNU General Public License as published by         *
 * the Free Software Foundation, either version 3 of the License, or            *
 * (at your option) any later version.                                          *
 *                                                                              *
 * This program is distributed in the hope that it will be useful,              *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of               *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                *
 * GNU General Public License for more details.                                 *
 *                                                                              *
 * You should have received a copy of the GNU General Public License            *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.        *
 ********************************************************************************/

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include "ui_settingsdialog.h"

class MainWindow;

class SettingsDialog: public QDialog, private Ui::SettingsDialog
{
    Q_OBJECT

    public:
        SettingsDialog(MainWindow *parent);

    private slots:
        void apply();

        void moveUpButtonClick();
        void moveDownButtonClick();
        void moveLeftButtonClick();
        void moveRightButtonClick();
        void addDictDirButtonClick();
        void removeDictDirButtonClick();
        void moveUpDictDirButtonClick();
        void moveDownDictDirButtonClick();

    private:
        void updateOrder();

        MainWindow *mainWindow;
};

#endif // SETTINGSDIALOG_H

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent

