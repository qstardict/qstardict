#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"

#include <QSystemTrayIcon>
class QMenu;
class DictCore;
class PopupWindow;
class SettingsDialog;

class MainWindow: public QMainWindow, private Ui::MainWindow
{   
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private slots:
        void aboutAction();
        void settingsAction();
        
        void queryButtonClicked();
        void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
        void wordsListItemActivated(QListWidgetItem *item);

        friend class SettingsDialog;
    private:
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

