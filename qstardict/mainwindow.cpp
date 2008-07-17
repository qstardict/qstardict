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
#include <QKeyEvent>
#include <QMenu>
#include <QMessageBox>
#include <QSettings>
#include <QTextStream>
#include <QTimerEvent>
#include <QToolBar>
#include "dictcore.h"
#include "application.h"
#include "popupwindow.h"
#include "settingsdialog.h"

namespace QStarDict 
{

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent)
{
    setupUi(this);
    m_dict = 0;
    translationView->setDict(m_dict);

    menu_File->insertActions(actionQuit, translationView->toolBar()->actions());

    menu_Options->insertAction(menu_Options->actions().first(), wordsListDock->toggleViewAction());
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
    on_queryButton_clicked();
}

void MainWindow::createConnections()
{
    connect(actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
    actionScan->setChecked(Application::instance()->popupWindow()->isScan());
    connect(actionScan, SIGNAL(toggled(bool)), 
            Application::instance()->popupWindow(), SLOT(setScan(bool)));
    connect(Application::instance()->popupWindow(), SIGNAL(scanChanged(bool)),
            actionScan, SLOT(setChecked(bool)));
    connect(wordsList, SIGNAL(itemActivated(QListWidgetItem*)),
            SLOT(wordsListItemActivated(QListWidgetItem*)));
    connect(wordsList, SIGNAL(itemClicked(QListWidgetItem*)),
            SLOT(wordsListItemActivated(QListWidgetItem*)));

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
    setInstantSearch(config.value("MainWindow/instantSearch", true).toBool());
    setDefaultStyleSheet(config.value("MainWindow/defaultStyleSheet", defaultStyleSheet()).toString());
}

void MainWindow::saveSettings()
{
    QSettings config;
    config.setValue("MainWindow/geometry", saveGeometry());
    config.setValue("MainWindow/state", saveState());
    config.setValue("MainWindow/visible", isVisible());
    config.setValue("MainWindow/wordsListDock/floating", wordsListDock->isFloating());
    config.setValue("MainWindow/wordsListDock/geometry", wordsListDock->geometry());
    config.setValue("MainWindow/instantSearch", m_instantSearch);
    config.setValue("MainWindow/defaultStyleSheet", defaultStyleSheet());
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this,
            tr("About QStarDict"),
            tr("<b>QStarDict %1 </b> - Qt version of StarDict<br>").arg(QSTARDICT_VERSION) +
            tr("Copyright (C) 2007-2008 Alexander Rodin "
               "<a href=\"http://qstardict.ylsoftware.com\">http://qstardict.ylsoftware.com</a>"));
}

void MainWindow::on_actionSettings_triggered()
{
    SettingsDialog dialog(this);
    dialog.exec();
}

void MainWindow::on_queryButton_clicked()
{
    if (searchBox->text().simplified().isEmpty())
    {
        setWindowTitle(tr("QStarDict"));
        translationView->clear();
        wordsList->clear();
        return;
    }
    wordsList->clear();
    wordsList->addItems(m_dict->findSimilarWords(searchBox->text()));
    translationView->translate(searchBox->text());
}

void MainWindow::reload()
{
    wordsList->clear();
    wordsList->addItems(m_dict->findSimilarWords(translationView->translatedWord()));
    translationView->reload();
}

void MainWindow::queryEdited(const QString &)
{
    if (m_queryTimer)
        killTimer(m_queryTimer);
    m_queryTimer = startTimer(200);
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_queryTimer)
    {
        killTimer(m_queryTimer);
        m_queryTimer = 0;
        on_queryButton_clicked();
    }
    else
        QMainWindow::timerEvent(event);
}

void MainWindow::wordTranslated(const QString &word)
{
    if (word.simplified().isEmpty())
        setWindowTitle(tr("QStarDict"));
    else
        setWindowTitle(tr("%1 - QStarDict").arg(word));
    if (m_queryTimer)
    {
        killTimer(m_queryTimer);
        m_queryTimer = 0;
    }
}

void MainWindow::wordsListItemActivated(QListWidgetItem *item)
{
    searchBox->setText(item->text());
    translationView->translate(item->text());
    setWindowTitle(tr("%1 - QStarDict").arg(translationView->translatedWord()));
}

void MainWindow::setInstantSearch(bool instantSearch)
{
    if (instantSearch == m_instantSearch)
        return;
    m_instantSearch = instantSearch;
    if (m_instantSearch)
        connect(searchBox, SIGNAL(textEdited(const QString&)), SLOT(queryEdited(const QString&)));
    else
        disconnect(searchBox, SIGNAL(textEdited(const QString&)), this, SLOT(queryEdited(const QString&)));
}

void MainWindow::setDict(DictCore *dict)
{
    m_dict = dict;
    translationView->setDict(dict);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->text().size() || event->key() == Qt::Key_Escape)
    {
        if (! searchBox->hasFocus())
        {
            searchBox->setText(event->text());
            searchBox->setFocus(Qt::OtherFocusReason);
        }
        if (event->key() == Qt::Key_Escape)
        {
            if (searchBox->text().isEmpty())
                close();
            else
                searchBox->clear();
        }
    }
    QMainWindow::keyPressEvent(event);
}

}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

