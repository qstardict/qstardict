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
#include <QStandardItemModel>
#include <QHeaderView>
#include "dictcore.h"
#include "mainwindow.h"
#include "popupwindow.h"
#include "adddictionarydialog.h"
#include "application.h"

namespace QStarDict {

SettingsDialog::SettingsDialog(QWidget *parent)
        : QDialog(parent)
{
    setupUi(this);

    m_dictsModel = new QStandardItemModel(this);
    m_dictsModel->setHorizontalHeaderLabels(
            QStringList() << tr("Enabled") << tr("Name") << tr("Plugin") << tr("Words count"));
    loadDictsList();
    dictsTableView->setModel(m_dictsModel);

    // Load global settings
    instantSearchBox->setChecked(Application::instance()->mainWindow()->isInstantSearch());

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
    popupOpacitySpin->setValue(static_cast<int>(popup->windowOpacity() * 100));
    timeoutBeforeHideSpin->setValue(popup->timeoutBeforeHide() / 1000.0);
    popupDefaultWidthSpin->setValue(popup->defaultSize().width());
    popupDefaultHeightSpin->setValue(popup->defaultSize().height());
    pronounceWordBox->setChecked(popup->pronounceWord());

    connect(this, SIGNAL(accepted()), SLOT(apply()));
}

void SettingsDialog::apply()
{
    // Save global settings
    Application::instance()->mainWindow()->setInstantSearch(instantSearchBox->isChecked());

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
}

void SettingsDialog::loadDictsList()
{
    m_dictsModel->clear();
    int i;
    QList<DictCore::Dictionary> loadedDicts = Application::instance()->dictCore()->loadedDicts();
    for (i = 0; i < loadedDicts.size(); ++i)
    {
        QStandardItem *item = new QStandardItem();
        item->setCheckable(true);
        item->setCheckState(Qt::Checked);
        m_dictsModel->setItem(i, 0, item);
        m_dictsModel->setItem(i, 1, new QStandardItem(loadedDicts[i].name()));
        m_dictsModel->setItem(i, 2, new QStandardItem(loadedDicts[i].plugin()));
        long wordsCount = Application::instance()->dictCore()->dictInfo(loadedDicts[i]).wordsCount();
        m_dictsModel->setItem(i, 3, new QStandardItem(QString::number(wordsCount)));
    }
    QList<DictCore::Dictionary> dicts = Application::instance()->dictCore()->avialableDicts();
    for (QList<DictCore::Dictionary>::const_iterator iter = dicts.begin(); iter != dicts.end(); ++iter)
    {
        if (! loadedDicts.contains(*iter))
        {
            ++i;
            QStandardItem *item = new QStandardItem();
            item->setCheckable(true);
            item->setCheckState(Qt::Unchecked);
            m_dictsModel->setItem(i, 0, item);
            m_dictsModel->setItem(i, 1, new QStandardItem(iter->name()));
            m_dictsModel->setItem(i, 2, new QStandardItem(iter->plugin()));
            long wordsCount = Application::instance()->dictCore()->dictInfo(*iter).wordsCount();
            m_dictsModel->setItem(i, 3, new QStandardItem(QString::number(wordsCount)));
        }
    }
}

void SettingsDialog::on_dictsMoveUpButton_clicked()
{

}

void SettingsDialog::on_dictsMoveDownButton_clicked()
{

}

void SettingsDialog::on_dictsShowInfoButton_clicked()
{

}

void SettingsDialog::on_pluginsMoveUpButton_clicked()
{

}

void SettingsDialog::on_pluginsMoveDownButton_clicked()
{

}

void SettingsDialog::on_pluginsShowInfoButton_clicked()
{

}

void SettingsDialog::on_pluginsConfigureButton_clicked()
{

}

}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

