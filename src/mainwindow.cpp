/*****************************************************************************
 * mainwindow.cpp - QStarDict, a StarDict clone written with using Qt        *
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

#include "mainwindow.h"

#include <QApplication>
#include <QCloseEvent>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QMenu>
#include <QMessageBox>
#include <QSettings>
#include <QTextStream>
#include "dictcore.h"
#include "popupwindow.h"
#include "settingsdialog.h"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent)
{
    setupUi(this);
    m_dict = new DictCore(this);
    translationView->setDict(m_dict);
    popup = new PopupWindow(m_dict, this);

    menu_Options->insertAction(menu_Options->actions().first(), wordsListDock->toggleViewAction());

    createTrayIcon();
    createConnections();

    loadSettings();
}

MainWindow::~MainWindow()
{
    saveSettings();
}

void MainWindow::showTranslation(const QString &text)
{
    searchBox->setText(text);
    queryButtonClicked();
}

void MainWindow::createTrayIcon()
{
    trayIcon = new QSystemTrayIcon(QIcon(":/icons/qstardict.png"), this);
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
    connect(actionSaveToFile, SIGNAL(activated()), SLOT(saveToFileAction()));
    connect(actionScan, SIGNAL(toggled(bool)), popup, SLOT(setScan(bool)));
    connect(popup, SIGNAL(scanChanged(bool)), actionScan, SLOT(setChecked(bool)));
    actionScan->setChecked(popup->isScan());
    connect(actionSettings, SIGNAL(activated()), SLOT(settingsAction()));
    connect(actionQuit, SIGNAL(activated()), qApp, SLOT(quit()));

    connect(queryButton, SIGNAL(clicked()), SLOT(queryButtonClicked()));
    connect(wordsList, SIGNAL(itemActivated(QListWidgetItem*)),
            SLOT(wordsListItemActivated(QListWidgetItem*)));
    connect(wordsList, SIGNAL(itemClicked(QListWidgetItem*)),
            SLOT(wordsListItemActivated(QListWidgetItem*)));

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));
    connect(translationView, SIGNAL(wordTranslated(const QString&)),
            SLOT(wordTranslated(const QString&)));
}

void MainWindow::loadSettings()
{
    QSettings config;
    restoreGeometry(config.value("MainWindow/geometry", QByteArray()).toByteArray());
    restoreState(config.value("MainWindow/state", QByteArray()).toByteArray());
    setVisible(config.value("MainWindow/visible", true).toBool());
    wordsListDock->setFloating(config.value("MainWindow/wordsListDock/floating", wordsListDock->isFloating()).toBool());
    wordsListDock->setGeometry(config.value("MainWindow/wordsListDock/geometry", wordsListDock->geometry()).toRect());
    m_dict->setDictDirs(config.value("DictCore/dictDirs", m_dict->dictDirs()).toStringList());
    m_dict->setDicts(config.value("DictCore/orderedDicts", m_dict->avialableDicts()).toStringList());
    translationView->setTranslationFlags(QFlag(config.value("DictWidget/translationFlags",
                    static_cast<int>(translationView->translationFlags())).toInt()));
    setInstantSearch(config.value("MainWindow/instantSearch", false).toBool());
}

void MainWindow::saveSettings()
{
    QSettings config;
    config.setValue("MainWindow/geometry", saveGeometry());
    config.setValue("MainWindow/state", saveState());
    config.setValue("MainWindow/visible", isVisible());
    config.setValue("MainWindow/wordsListDock/floating", wordsListDock->isFloating());
    config.setValue("MainWindow/wordsListDock/geometry", wordsListDock->geometry());
    config.setValue("DictCore/dictDirs", m_dict->dictDirs());
    config.setValue("DictCore/orderedDicts", m_dict->orderedDicts());
    config.setValue("DictCore/disabledDicts", m_dict->disabledDicts());
    config.setValue("DictWidget/translationFlags", static_cast<int>(translationView->translationFlags()));
    config.setValue("MainWindow/instantSearch", m_instantSearch);
}

void MainWindow::aboutAction()
{
    QMessageBox::about(this,
            tr("About QStarDict"),
            tr("<b>QStarDict ") + QSTARDICT_VERSION + tr("</b> - Qt version of StarDict<br>") +
            tr("Copyright (C) 2007 Alexander Rodin "
               "<a href=\"http://qstardict.ylsoftware.com\">http://qstardict.ylsoftware.com</a>"));
}

void MainWindow::settingsAction()
{
    SettingsDialog dialog(this);
    dialog.exec();
}

void MainWindow::saveToFileAction()
{
    queryButtonClicked();
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save translation"),
                       QDir::homePath() + "/" + searchBox->text() + ".txt");
    if (! fileName.isEmpty())
    {
        QFile outputFile(fileName);
        if (! outputFile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMessageBox::warning(this, tr("Error"),
                                 tr("Cannot save translation"));
            return;
        }
        QTextStream outputStream(&outputFile);
        outputStream << m_dict->translate(searchBox->text(), DictCore::Reformat);
    }
}

void MainWindow::queryButtonClicked()
{
    if (searchBox->text().isEmpty())
    {
        setWindowTitle(tr("QStarDict"));
        translationView->clear();
        return;
    }
    wordsList->clear();
    wordsList->addItems(m_dict->find(searchBox->text()));
    translationView->translate(searchBox->text());
}

void MainWindow::wordTranslated(const QString &word)
{
    setWindowTitle(tr("%1 - QStarDict").arg(word));
}

void MainWindow::wordsListItemActivated(QListWidgetItem *item)
{
    searchBox->setText(item->text());
    translationView->translate(item->text());
    setWindowTitle(tr("%1 - QStarDict").arg(translationView->translatedWord()));
}

void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger)
        setVisible(! isVisible());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    hide();
    event->ignore();
}

QString MainWindow::translate(const QString &text) const
{
    return m_dict->translate(text, translationView->translationFlags() & ~DictCore::Html);
}

QString MainWindow::translateHtml(const QString &text) const
{
    return
	"<html>\n"
	"<head>\n"
	"<meta http-equiv=\"Content-Type\" content=\"text/html;charset=UTF-8\">\n"
	"<title>" + text + "</title>\n"
	"<style><!--\n" +
	translationView->cssStyle() + 
	"--></style>\n"
	"</head>\n"
	"<body>\n" +
	m_dict->translate(text, translationView->translationFlags() | DictCore::Html) +
	"</body>\n"
	"</html>\n";
}

void MainWindow::setInstantSearch(bool instantSearch)
{
    if (instantSearch == m_instantSearch)
        return;
    m_instantSearch = instantSearch;
    if (m_instantSearch)
        connect(searchBox, SIGNAL(textEdited(const QString&)), SLOT(queryButtonClicked()));
    else
        disconnect(searchBox, SIGNAL(textEdited(const QString&)), this, SLOT(queryButtonClicked()));
}

