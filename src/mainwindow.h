/*****************************************************************************
 * mainwindow.h - QStarDict, a StarDict clone written with using Qt          *
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

        QString translate(const QString &text) const;
	QString translateHtml(const QString &text) const;

    protected:
        void closeEvent(QCloseEvent *event);

    private slots:
        void aboutAction();
        void settingsAction();
        void saveToFileAction();

        void queryButtonClicked();
        void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
        void wordsListItemActivated(QListWidgetItem *item);
        void wordTranslated(const QString &word);

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
