/*****************************************************************************
 * settingsdialog.cpp - QStarDict, a StarDict clone written with using Qt    *
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

#include "settingsdialog.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QInputDialog>
#include <QSettings>
#include <math.h>
#include "dictcore.h"
#include "mainwindow.h"
#include "popupwindow.h"
#include "application.h"
#include "speaker.h"

namespace
{
int toPercents(double value)
{
    int integralValue = static_cast<int>(value * 100.0);
    if (value * 100.0 - 0.5 > integralValue)
        ++integralValue;
    return integralValue;
}
}

namespace QStarDict {

SettingsDialog::SettingsDialog(QWidget *parent)
        : QDialog(parent)
{
    setupUi(this);


    DictCore *dict = Application::instance()->dictCore();
    m_oldPlugins = dict->loadedPlugins();
    m_oldDicts = dict->loadedDicts();

    m_pluginsModel = new QStandardItemModel(this);
    m_pluginsModel->setHorizontalHeaderLabels(
            QStringList() << tr("Enabled") << tr("Name"));
    loadPluginsList();
    pluginsTableView->setModel(m_pluginsModel);
    pluginsTableView->verticalHeader()->hide();
    pluginsTableView->setColumnWidth(0, 60);
    pluginsTableView->setColumnWidth(1, 320);

    m_dictsModel = new QStandardItemModel(this);
    m_dictsModel->setHorizontalHeaderLabels(QStringList() << tr("Enabled") << tr("Name") << tr("Plugin"));
    loadDictsList();
    dictsTableView->setModel(m_dictsModel);
    dictsTableView->verticalHeader()->hide();
    dictsTableView->setColumnWidth(0, 60);
    dictsTableView->setColumnWidth(1, 200);
    dictsTableView->setColumnWidth(2, 120);

    // Load global settings
    instantSearchBox->setChecked(Application::instance()->mainWindow()->isInstantSearch());
    speechCmdEdit->setText(Application::instance()->speaker()->speechCmd());

    // Load popup window settings
    PopupWindow *popup = Application::instance()->popupWindow();
    useScanBox->setChecked(popup->isScan());
    if (popup->modifierKey())
    {
        useScanModifierBox->setChecked(true);
        QString modifierName;
        switch (popup->modifierKey())
        {
        case Qt::ShiftModifier:
            modifierName = "Shift";
            break;
        case Qt::ControlModifier:
            modifierName = "Control";
            break;
        case Qt::AltModifier:
            modifierName = "Alt";
            break;
        case Qt::MetaModifier:
            modifierName = "Win";
            break;
        }
        modifierKeyBox->setCurrentIndex(modifierKeyBox->findText(modifierName));
    }
    showIfNotFoundBox->setChecked(popup->showIfNotFound());
    popupOpacitySpin->setValue(toPercents(popup->windowOpacity()));
    timeoutBeforeHideSpin->setValue(popup->timeoutBeforeHide() / 1000.0);
    popupDefaultWidthSpin->setValue(popup->defaultSize().width());
    popupDefaultHeightSpin->setValue(popup->defaultSize().height());
    pronounceWordBox->setChecked(popup->pronounceWord());

    // Load translations CSS
    QHash<QString, QString> cssAliases;
    cssAliases["body"] = tr("All translation");
    cssAliases["font.dict_name"] = tr("Dictionary name");
    cssAliases["font.title"] = tr("Title");
    cssAliases["font.explanation"] = tr("Explanation");
    cssAliases["font.abrreviation"] = tr("Abbreviation");
    cssAliases["font.example"] = tr("Example");
    cssAliases["font.transcription"] = tr("Transcription");
    apperanceCSSEdit->setElementsAliases(cssAliases);
    apperanceCSSEdit->setCSS(Application::instance()->mainWindow()->defaultStyleSheet());

    connect(m_pluginsModel, SIGNAL(itemChanged(QStandardItem*)),
            SLOT(pluginsItemChanged(QStandardItem*)));
}

void SettingsDialog::accept()
{
    // Save dicts and plugins settings
    DictCore *dict = Application::instance()->dictCore();
    QStringList loadedPlugins;
    int rowCount = m_pluginsModel->rowCount();
    for (int i = 0; i < rowCount; ++i)
        if (m_pluginsModel->item(i, 0)->checkState() == Qt::Checked)
            loadedPlugins << m_pluginsModel->item(i, 1)->text();
    dict->setLoadedPlugins(loadedPlugins);

    QList<DictCore::Dictionary> loadedDicts;
    rowCount = m_dictsModel->rowCount();
    for (int i = 0; i < rowCount; ++i)
        if (m_dictsModel->item(i, 0)->checkState() == Qt::Checked)
            loadedDicts << DictCore::Dictionary(m_dictsModel->item(i, 2)->text(), m_dictsModel->item(i, 1)->text());
    dict->setLoadedDicts(loadedDicts);

    // Save global settings
    Application::instance()->mainWindow()->setInstantSearch(instantSearchBox->isChecked());
    Application::instance()->speaker()->setSpeechCmd(speechCmdEdit->text());

    // Save popup window settings
    PopupWindow *popup = Application::instance()->popupWindow();
    popup->setScan(useScanBox->isChecked());
    int modifierKey = 0;
    if (useScanModifierBox->isChecked())
        if (modifierKeyBox->currentText() == "Shift")
            modifierKey = Qt::ShiftModifier;
        else if (modifierKeyBox->currentText() == "Control")
            modifierKey = Qt::ControlModifier;
        else if (modifierKeyBox->currentText() == "Alt")
            modifierKey = Qt::AltModifier;
        else if (modifierKeyBox->currentText() == "Win")
            modifierKey = Qt::MetaModifier;
    popup->setShowIfNotFound(showIfNotFoundBox->isChecked());
    popup->setModifierKey(modifierKey);
    popup->setWindowOpacity(popupOpacitySpin->value() / 100.0);
    popup->setTimeoutBeforeHide(static_cast<int>(timeoutBeforeHideSpin->value() * 1000.0));
    popup->setDefaultSize(QSize(popupDefaultWidthSpin->value(), popupDefaultHeightSpin->value()));
    popup->setPronounceWord(pronounceWordBox->isChecked());

    // Save translations CSS
    Application::instance()->mainWindow()->setDefaultStyleSheet(apperanceCSSEdit->css());

    QDialog::accept();
}

void SettingsDialog::reject()
{
    DictCore *dict = Application::instance()->dictCore();
    dict->setLoadedPlugins(m_oldPlugins);
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

void SettingsDialog::loadPluginsList()
{
    m_pluginsModel->setRowCount(0);
    DictCore *dict = Application::instance()->dictCore();
    QStringList plugins = dict->availablePlugins();
    QStringList loaded = dict->loadedPlugins();
    for (int i = 0; i < plugins.size(); ++i)
    {
        QStandardItem *item = new QStandardItem();
        item->setCheckable(true);
        item->setCheckState(loaded.contains(plugins[i]) ? Qt::Checked : Qt::Unchecked);
        m_pluginsModel->setItem(i, 0, item);
        m_pluginsModel->setItem(i, 1, new QStandardItem(plugins[i]));
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
    DictPlugin::DictInfo info = Application::instance()->dictCore()->plugin(plugin)->dictInfo(dict);
    QMessageBox::information(this,
            tr("Information about dictionary \"%1\"").arg(dict),
            tr("<b>Name:</b> %1<br>").arg(dict) +
            tr("<b>Plugin:</b> %1<br>").arg(plugin) +
            tr("<b>Author:</b> %1<br>").arg(info.author()) +
            tr("<b>Words count:</b> %1<br>").arg((info.wordsCount() == -1) ? tr("unknown") : QString::number(info.wordsCount())) +
            tr("<b>Description:</b> %1").arg(info.description()));
}

void SettingsDialog::on_pluginsShowInfoButton_clicked()
{
    int currentRow = pluginsTableView->currentIndex().row();
    if (currentRow == -1)
        return;
    DictPlugin *plugin = Application::instance()->dictCore()->plugin(m_pluginsModel->item(currentRow, 1)->text());
    if (! plugin)
        return;
    QMessageBox::information(this,
            tr("Information about %1 plugin").arg(plugin->name()),
            tr("<b>Name:</b> %1<br>").arg(plugin->name()) +
            tr("<b>Version:</b> %1<br>").arg(plugin->version()) +
            tr("<b>Authors:</b> %1<br>").arg(plugin->authors().replaceInStrings("<", "&lt;").replaceInStrings(">", "&gt;").join(tr("<br>"))) +
            tr("<b>Can search similar words:</b> %1<br>").arg(plugin->features().testFlag(DictPlugin::SearchSimilar) ? tr("yes") : tr("no")) +
            tr("<b>Description:</b> %1").arg(plugin->description()));
}

void SettingsDialog::on_pluginsConfigureButton_clicked()
{
    int currentRow = pluginsTableView->currentIndex().row();
    if (currentRow == -1)
        return;
    DictCore *dict = Application::instance()->dictCore();
    DictPlugin *plugin = dict->plugin(m_pluginsModel->item(currentRow, 1)->text());
    if (plugin && plugin->execSettingsDialog(this) == QDialog::Accepted)
    {
        dict->reloadDicts();
        loadDictsList();
    }
}

void SettingsDialog::pluginsItemChanged(QStandardItem *item)
{
    if (item->isCheckable())
    {
        DictCore *dict = Application::instance()->dictCore();
        QStringList loadedPlugins;
        int rowCount = m_pluginsModel->rowCount();
        for (int i = 0; i < rowCount; ++i)
            if (m_pluginsModel->item(i, 0)->checkState() == Qt::Checked)
                loadedPlugins << m_pluginsModel->item(i, 1)->text();
        dict->setLoadedPlugins(loadedPlugins);
        dict->reloadDicts();
        loadDictsList();
    }
}

}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

