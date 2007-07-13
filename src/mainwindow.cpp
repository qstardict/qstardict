#include "mainwindow.h"

#include <stdio.h>
#include <QApplication>
#include <QCloseEvent>
#include <QDockWidget>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QMenu>
#include <QMessageBox>
#include <QSettings>
#include "dictcore.h"
#include "popupwindow.h"
#include "settingsdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi(this);
    m_dict = new DictCore(this);
    popup = new PopupWindow(m_dict, this);

    menu_View->addAction(wordsListDock->toggleViewAction());

    createTrayIcon();
    createConnections();

    loadSettings();
}

MainWindow::~MainWindow()
{
    saveSettings();
}

void MainWindow::createTrayIcon()
{
    trayIcon = new QSystemTrayIcon(QIcon(":/icons/icon.png"), this);
    trayIcon->show();
    trayMenu = new QMenu(tr("QStarDict"), this);
    trayMenu->addAction(actionScan);
    trayMenu->addAction(actionQuit);
    trayIcon->setContextMenu(trayMenu);
}

void MainWindow::createConnections()
{
    connect(actionAbout, SIGNAL(activated()), SLOT(aboutAction()));
    connect(actionAboutQt, SIGNAL(activated()), qApp, SLOT(aboutQt()));
    connect(actionScan, SIGNAL(toggled(bool)), popup, SLOT(setScan(bool)));
    connect(popup, SIGNAL(scanChanged(bool)), actionScan, SLOT(setChecked(bool)));
    actionScan->setChecked(popup->isScan());
    connect(actionSettings, SIGNAL(activated()), SLOT(settingsAction()));
    connect(actionQuit, SIGNAL(activated()), qApp, SLOT(quit()));
        
    connect(queryButton, SIGNAL(clicked()), SLOT(queryButtonClicked()));
    connect(wordsList, SIGNAL(itemActivated(QListWidgetItem*)),
            SLOT(wordsListItemActivated(QListWidgetItem*)));

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));
}

void MainWindow::loadSettings()
{
    QSettings config;
    restoreGeometry(config.value("MainWindow/geometry", QByteArray()).toByteArray());
    restoreState(config.value("MainWindow/state", QByteArray()).toByteArray());
    setVisible(config.value("MainWindow/visible", true).toBool());
    wordsListDock->setFloating(config.value("MainWindow/wordsListDock/floating", wordsListDock->isFloating()).toBool());
    wordsListDock->setGeometry(config.value("MainWindow/wordsListDock/geometry", wordsListDock->geometry()).toRect());
    m_dict->setDicts(config.value("DictCore/orderedDicts", m_dict->avialableDicts()).toStringList());
}

void MainWindow::saveSettings()
{
    QSettings config;
    config.setValue("MainWindow/geometry", saveGeometry());
    config.setValue("MainWindow/state", saveState());
    config.setValue("MainWindow/visible", isVisible());
    config.setValue("MainWindow/wordsListDock/floating", wordsListDock->isFloating());
    config.setValue("MainWindow/wordsListDock/geometry", wordsListDock->geometry());
    config.setValue("DictCore/orderedDicts", m_dict->orderedDicts());
    config.setValue("DictCore/disabledDicts", m_dict->disabledDicts());
}

void MainWindow::aboutAction()
{
    QMessageBox::about(this,
            tr("About QStarDict"),
            tr("The Qt version of StarDict\n"
               "Copyright (C) 2007, Alex Rodin"));
}

void MainWindow::settingsAction()
{
    SettingsDialog *dialog = new SettingsDialog(this);
    dialog->show();
}

void MainWindow::queryButtonClicked()
{
    if (searchBox->text().isEmpty())
        return;
    wordsList->clear();
    wordsList->addItems(m_dict->find(searchBox->text()));
    translationView->setHtml(m_dict->translate(searchBox->text()));
}

void MainWindow::wordsListItemActivated(QListWidgetItem *item)
{
    searchBox->setText(item->text());
    translationView->setText(m_dict->translate(item->text()));
}

void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger)
        setVisible(! isVisible());
}

