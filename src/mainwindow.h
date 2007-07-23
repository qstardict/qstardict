/********************************************************************************
 * mainwindow.h - QStarDict, a StarDict clone written with using Qt             *
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"

#include <QSystemTrayIcon>

class QMenu;
class QCloseEvent;

class DictCore;
class PopupWindow;
class SettingsDialog;

class MainWindow: public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = 0);
        ~MainWindow();

        PopupWindow* popupWindow()
        { return popup; }

    public slots:
        void showTranslation(const QString &text);

    protected:
        void closeEvent(QCloseEvent *event);

    private slots:
        void aboutAction();
        void settingsAction();
        void saveToFileAction();

        void queryButtonClicked();
        void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
        void wordsListItemActivated(QListWidgetItem *item);

    private:
        friend class SettingsDialog;

        void createTrayIcon();
        void createConnections();
        void loadSettings();
        void saveSettings();

        DictCore *m_dict;
        QSystemTrayIcon *trayIcon;
        QMenu *trayMenu;
        PopupWindow *popup;
};

#endif // MAINWINDOW_H

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

