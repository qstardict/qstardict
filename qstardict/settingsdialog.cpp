/*****************************************************************************
 * settingsdialog.cpp - QStarDict, a quasi-star dictionary                   *
 * Copyright (C) 2007-2019 Alexander Rodin                                   *
 * Copyright (C) 2016 Sergey Il'inykh                                        *
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

#include "settingsdialog.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QInputDialog>
#include <QSettings>
#include <QKeySequence>
#include <math.h>
#include <QStyledItemDelegate>
#include <QPainter>

#include "dictcore.h"
#include "mainwindow.h"
#include "popupwindow.h"
#include "application.h"
#include "speaker.h"
#include "pluginsmodel.h"
#include "pluginmanager.h"
#include "qxt/qxtglobalshortcut.h"
#include "../plugins/pluginserver.h"
#ifdef QSTARDICT_WITH_TRAY_ICON
#include "trayicon.h"
#endif

namespace
{
int toPercents(double value)
{
    int integralValue = static_cast<int>(value * 100.0);
    if (value * 100.0 - 0.5 > integralValue)
        ++integralValue;
    return integralValue;
}

QString getModifierName(int key)
{
    switch (key)
    {
    case Qt::ShiftModifier:
        return "Shift";
    case Qt::ControlModifier:
#ifdef Q_OS_MAC
        return "Command";
#else
        return "Control";
#endif
    case Qt::AltModifier:
#ifdef Q_OS_MAC
        return "Option";
#else
        return "Alt";
#endif
    case Qt::MetaModifier:
#ifdef Q_OS_MAC
        return "Control";
#elif Q_OS_WIN
        return "Win";
#else
        return "Super";
#endif
    default:
        return "";
    }
}

void initModifierComboBox(QComboBox *box, int selectedKey)
{
    box->clear();
    box->addItem(getModifierName(Qt::AltModifier), Qt::AltModifier);
    box->addItem(getModifierName(Qt::ShiftModifier), Qt::ShiftModifier);
    box->addItem(getModifierName(Qt::ControlModifier), Qt::ControlModifier);
    box->addItem(getModifierName(Qt::MetaModifier), Qt::MetaModifier);

    box->setCurrentIndex(qMax(0, box->findData(selectedKey)));
}

}

namespace QStarDict {

SettingsDialog::SettingsDialog(QWidget *parent)
        : QDialog(parent)
{
    setupUi(this);
    Application * const app = Application::instance();

    DictCore *dict = app->dictCore();
    m_oldPlugins = app->pluginManager()->loadedPlugins();
    m_oldDicts = dict->loadedDicts();

    m_dictPluginsModel = new PluginsModel(PluginsModel::LoadType::JustDict, app->pluginManager());
    m_miscPluginsModel = new PluginsModel(PluginsModel::LoadType::ExceptDict, app->pluginManager());

    pluginsTableView->setModel(m_dictPluginsModel);
    miscPluginsView->setModel(m_miscPluginsModel);
    pluginsTableView->configureColumns();
    miscPluginsView->configureColumns();

    m_dictsModel = new QStandardItemModel(this);
    m_dictsModel->setHorizontalHeaderLabels(QStringList() << tr("Enabled") << tr("Name") << tr("Plugin"));
    loadDictsList();
    dictsTableView->setModel(m_dictsModel);
    dictsTableView->verticalHeader()->hide();
    dictsTableView->setColumnWidth(0, 60);
    dictsTableView->setColumnWidth(1, 200);
    dictsTableView->setColumnWidth(2, 120);

    // Load global settings
    runInBackgroundBox->setChecked(!app->mainWindow()->quitOnClose());
    instantSearchBox->setChecked(app->mainWindow()->isInstantSearch());
    speechCmdEdit->setText(app->speaker()->speechCmd());
#ifdef Q_OS_LINUX
    QFile desktop(QDir::homePath() + "/.config/autostart/qstardict.desktop");
    if (desktop.open(QIODevice::ReadOnly) && QString(desktop.readAll())
        .contains(QRegExp("\\bhidden\\s*=\\s*false", Qt::CaseInsensitive))) {
        autostartBox->setChecked(true);
    }
#elif defined(Q_OS_WIN)
    QSettings reg("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\"
                  "CurrentVersion\\Run", QSettings::NativeFormat);
    autostartBox->setChecked(
            reg.contains(QCoreApplication::applicationName()));
#else
    autostartBox->setVisible(false);
#endif
    linksBox->setChecked(app->mainWindow()->showLinks());
    linksModifierBox->setChecked(app->mainWindow()->showLinksModifierKey() != 0);
    initModifierComboBox(linksModifierKeyBox, app->mainWindow()->showLinksModifierKey());

    // Load popup shortcut settings
    shortcutPopupEdit->setText(app->popupShortcut()->shortcut().toString());
    shortcutPopupBox->setChecked(app->popupShortcut()->isEnabled());

    // Load popup window settings
    PopupWindow *popup = app->popupWindow();
    useScanBox->setChecked(popup->isScan());
    useScanModifierBox->setChecked(popup->modifierKey() != 0);
    initModifierComboBox(modifierKeyBox, popup->modifierKey());
    showIfNotFoundBox->setChecked(popup->showIfNotFound());
    popupOpacitySpin->setValue(toPercents(popup->windowOpacity()));
    timeoutBeforeHideSpin->setValue(popup->timeoutBeforeHide() / 1000.0);
    popupDefaultWidthSpin->setValue(popup->defaultSize().width());
    popupDefaultHeightSpin->setValue(popup->defaultSize().height());
    pronounceWordBox->setChecked(popup->pronounceWord());

    // Load translations CSS
    QVector<QPair<QString, QString>> cssAliases;
    cssAliases.append({"body", tr("All translation")});
    cssAliases.append({"font.dict_name", tr("Dictionary name")});
    cssAliases.append({"font.title", tr("Title")});
    cssAliases.append({"font.transcription", tr("Transcription")});
    cssAliases.append({"font.explanation", tr("Explanation")});
    cssAliases.append({"font.abbreviature", tr("Abbreviation")});
    cssAliases.append({"font.example", tr("Example")});
    appearanceCSSEdit->setElementsNames(cssAliases);
    appearanceCSSEdit->setCSS(app->mainWindow()->defaultStyleSheet());

    connect(m_dictPluginsModel, SIGNAL(loadedListChanged()),
            SLOT(dictLoadedPluginsChanged()));
    connect(pluginsTableView, SIGNAL(clicked(QModelIndex)), SLOT(pluginClicked(QModelIndex)));
    connect(miscPluginsView, SIGNAL(clicked(QModelIndex)), SLOT(pluginClicked(QModelIndex)));
}

void SettingsDialog::accept()
{
    Application * const app = Application::instance();

    // Save dicts and plugins settings
    DictCore *dict = app->dictCore();

    //dict->setLoadedPlugins(m_dictPluginsModel->loadedPlugins() + m_miscPluginsModel->loadedPlugins());
#ifdef QSTARDICT_WITH_TRAY_ICON
    app->trayIcon()->reinit();
#endif

    QList<DictCore::Dictionary> loadedDicts;
    int rowCount = m_dictsModel->rowCount();
    for (int i = 0; i < rowCount; ++i)
        if (m_dictsModel->item(i, 0)->checkState() == Qt::Checked)
            loadedDicts << DictCore::Dictionary(m_dictsModel->item(i, 2)->text(), m_dictsModel->item(i, 1)->text());
    dict->setLoadedDicts(loadedDicts);

    // Save global settings
    app->mainWindow()->setQuitOnClose(!runInBackgroundBox->isChecked());
#ifdef QSTARDICT_WITH_TRAY_ICON
    app->trayIcon()->setVisible(runInBackgroundBox->isChecked());
#endif
    app->mainWindow()->setInstantSearch(instantSearchBox->isChecked());
    app->speaker()->setSpeechCmd(speechCmdEdit->text());
#ifdef Q_OS_LINUX
    QDir home = QDir::home();
    if (!home.exists(".config/autostart")) {
        home.mkpath(".config/autostart");
    }
    QFile desktopFile(QSTARDICT_INSTALL_PREFIX "/share/applications/qstardict.desktop");
    if (desktopFile.open(QIODevice::ReadOnly)) {
        QByteArray contents = desktopFile.readAll();
        QFile f(home.absolutePath() +
                "/.config/autostart/qstardict.desktop");

        if (f.open(QIODevice::WriteOnly | QIODevice::Text)) {
            f.write(contents.trimmed());
            f.write(QString("\nHidden=%1").arg(autostartBox->isChecked()?
                                               "false\n":"true\n").toUtf8());
        }
    }
#elif defined(Q_OS_WIN)
    QSettings reg("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\"
                  "CurrentVersion\\Run", QSettings::NativeFormat);
    if(autostartBox->isChecked())
        reg.setValue(QCoreApplication::applicationName(), '"' +
                     QDir::toNativeSeparators(QCoreApplication::
                                              applicationFilePath()) + '"');
    else
        reg.remove(QCoreApplication::applicationName());
#endif
    app->mainWindow()->setShowLinks(linksBox->isChecked());
    app->popupWindow()->setShowLinks(linksBox->isChecked());
    int linksModifierKey = linksModifierBox->isChecked() ? linksModifierKeyBox->currentData().toInt() : 0;
    app->mainWindow()->setShowLinksModifierKey(linksModifierKey);
    app->popupWindow()->setShowLinksModifierKey(linksModifierKey);

    // Save popup shortcut settings
    app->popupShortcut()->setShortcut(QKeySequence(shortcutPopupEdit->text()));
    app->popupShortcut()->setEnabled(shortcutPopupBox->isChecked());

    // Save popup window settings
    PopupWindow *popup = app->popupWindow();
    popup->setScan(useScanBox->isChecked());
    popup->setShowIfNotFound(showIfNotFoundBox->isChecked());
    if (useScanModifierBox->isChecked())
        popup->setModifierKey(modifierKeyBox->currentData().toInt());
    else
        popup->setModifierKey(0);
    popup->setWindowOpacity(popupOpacitySpin->value() / 100.0);
    popup->setTimeoutBeforeHide(static_cast<int>(timeoutBeforeHideSpin->value() * 1000.0));
    popup->setDefaultSize(QSize(popupDefaultWidthSpin->value(), popupDefaultHeightSpin->value()));
    popup->setPronounceWord(pronounceWordBox->isChecked());

    // Save translations CSS
    app->mainWindow()->setDefaultStyleSheet(appearanceCSSEdit->css());
    app->popupWindow()->setDefaultStyleSheet(appearanceCSSEdit->css());

    if (app->mainWindow()->quitOnClose())
        app->mainWindow()->show();

    app->mainWindow()->reload();
    app->mainWindow()->reloadToolbar();
    app->popupWindow()->reloadToolbar();

    app->dictCore()->saveSettings();
    app->mainWindow()->saveSettings();
#ifdef QSTARDICT_WITH_TRAY_ICON
    app->trayIcon()->saveSettings();
#endif
    app->popupWindow()->saveSettings();

    QDialog::accept();
}

void SettingsDialog::reject()
{
    DictCore *dict = Application::instance()->dictCore();
    Application::instance()->pluginManager()->setLoadedPlugins(m_oldPlugins);
    dict->setLoadedDicts(m_oldDicts);
    QDialog::reject();
}

void SettingsDialog::loadDictsList()
{
    int i;
    QList<DictCore::Dictionary> loadedDicts = Application::instance()->dictCore()->loadedDicts();
    m_dictsModel->setRowCount(0);
    for (i = 0; i < loadedDicts.size(); ++i)
    {
        QStandardItem *item = new QStandardItem();
        item->setCheckable(true);
        item->setCheckState(Qt::Checked);
        m_dictsModel->setItem(i, 0, item);
        m_dictsModel->setItem(i, 1, new QStandardItem(loadedDicts[i].name()));
        m_dictsModel->setItem(i, 2, new QStandardItem(loadedDicts[i].plugin()));
    }
    QList<DictCore::Dictionary> dicts = Application::instance()->dictCore()->availableDicts();
    for (QList<DictCore::Dictionary>::const_iterator iter = dicts.begin(); iter != dicts.end(); ++iter)
    {
        if (! loadedDicts.contains(*iter))
        {
            QStandardItem *item = new QStandardItem();
            item->setCheckable(true);
            item->setCheckState(Qt::Unchecked);
            m_dictsModel->setItem(i, 0, item);
            m_dictsModel->setItem(i, 1, new QStandardItem(iter->name()));
            m_dictsModel->setItem(i, 2, new QStandardItem(iter->plugin()));
            ++i;
        }
    }
}

void SettingsDialog::on_dictsMoveUpButton_clicked()
{
    int currentRow = dictsTableView->currentIndex().row();
    if (currentRow > 0)
    {
        m_dictsModel->insertRow(currentRow - 1, m_dictsModel->takeRow(currentRow));
        dictsTableView->selectRow(currentRow - 1);
    }
}

void SettingsDialog::on_dictsMoveDownButton_clicked()
{
    int currentRow = dictsTableView->currentIndex().row();
    if (currentRow < m_dictsModel->rowCount() - 1)
    {
        m_dictsModel->insertRow(currentRow + 1, m_dictsModel->takeRow(currentRow));
        dictsTableView->selectRow(currentRow + 1);
    }
}

void SettingsDialog::on_dictsShowInfoButton_clicked()
{
    int currentRow = dictsTableView->currentIndex().row();
    if (currentRow == -1)
        return;
    QString dict = m_dictsModel->item(currentRow, 1)->text();
    QString plugin = m_dictsModel->item(currentRow, 2)->text();
    DictPlugin::DictInfo info = Application::instance()->pluginManager()->plugin<DictPlugin>(plugin)->dictInfo(dict);
    QMessageBox::information(this,
            tr("Information about dictionary \"%1\"").arg(dict),
            tr("<b>Name:</b> %1<br>").arg(dict) +
            tr("<b>Plugin:</b> %1<br>").arg(plugin) +
            tr("<b>Author:</b> %1<br>").arg(info.author()) +
            tr("<b>Words count:</b> %1<br>").arg((info.wordsCount() == -1) ? tr("unknown") : QString::number(info.wordsCount())) +
            tr("<b>Description:</b> %1").arg(info.description()));
}

void SettingsDialog::pluginClicked(const QModelIndex &index)
{
    QString id = index.data(PluginsModel::IdRole).toString();
    if (index.column() == 2) {// settings
        auto plugin = Application::instance()->pluginManager()->plugin<ConfigurablePlugin>(id);
        if (plugin && plugin->execSettingsDialog(this) == QDialog::Accepted)
        {
            Application::instance()->dictCore()->reloadDicts();
            loadDictsList();
        }
    }
    if (index.column() == 3) {
        auto pm = Application::instance()->pluginManager();

        DictPlugin *dplugin = pm->plugin<DictPlugin>(id);
        const PluginMetadata &md = pm->pluginDesc(id)->metadata;
        QStringList authors = md.authors;
        auto *messageBox = new QMessageBox(this);
        messageBox->setIconPixmap(md.icon.pixmap(QSize(128, 128)));
        messageBox->setWindowTitle(tr("Information about %1 plugin").arg(md.name));
        messageBox->setText(
                tr("<b>Name:</b> %1<br>").arg(md.name) +
                tr("<b>Version:</b> %1<br>").arg(md.version) +
                tr("<b>Authors:</b> %1<br>").arg(authors.replaceInStrings("<", "&lt;").replaceInStrings(">", "&gt;").join(tr("<br>"))) +
                (dplugin? tr("<b>Can search similar words:</b>%1<br>").arg(dplugin->features().testFlag(DictPlugin::Feature::SearchSimilar) ? tr("yes") : tr("no")) : "") +
                tr("<b>Description:</b> %1").arg(md.description));
        messageBox->show();
    }
}

void SettingsDialog::dictLoadedPluginsChanged()
{
    DictCore *dict = Application::instance()->dictCore();
    Application::instance()->pluginManager()->setLoadedPlugins(m_dictPluginsModel->loadedPlugins() + m_miscPluginsModel->loadedPlugins());
    dict->reloadDicts();
    loadDictsList();
}

}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

