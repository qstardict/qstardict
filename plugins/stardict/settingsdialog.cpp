/*****************************************************************************
 * settingsdialog.cpp - QStarDict, a StarDict clone written with using Qt    *
 * Copyright (C) 2008 Alexander Rodin                                        *
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
#include "stardict.h"

SettingsDialog::SettingsDialog(StarDict *plugin, QWidget *parent)
    : QDialog(parent),
      m_plugin(plugin)
{
    setupUi(this);

    reformatListsBox->setChecked(m_plugin->m_reformatLists);
    expandAbbreviationsBox->setChecked(m_plugin->m_expandAbbreviations);
    dictDirsList->addItems(m_plugin->m_dictDirs);

    connect(this, SIGNAL(accepted()), SLOT(apply()));
}

void SettingsDialog::on_addDictDirButton_clicked()
{
    QString dirName = QFileDialog::getExistingDirectory(this, tr("Select dictionaries directory"));
    if (! dirName.isEmpty())
    {
        dictDirsList->addItem(dirName);
    }
}

void SettingsDialog::on_removeDictDirButton_clicked()
{
    delete dictDirsList->takeItem(dictDirsList->currentRow());
}

void SettingsDialog::on_moveUpDictDirButton_clicked()
{
    if (dictDirsList->currentRow() > 0)
    {
        dictDirsList->insertItem(dictDirsList->currentRow(),
                                 dictDirsList->takeItem(dictDirsList->currentRow()));
        dictDirsList->setCurrentRow(dictDirsList->currentRow() - 1);
    }
}

void SettingsDialog::on_moveDownDictDirButton_clicked()
{
    if (dictDirsList->currentRow() < dictDirsList->count() - 1)
    dictDirsList->insertItem(dictDirsList->currentRow(),
                             dictDirsList->takeItem(dictDirsList->currentRow() + 1));
}

void SettingsDialog::apply()
{
    m_plugin->m_reformatLists = reformatListsBox->isChecked();
    m_plugin->m_expandAbbreviations = expandAbbreviationsBox->isChecked();
    m_plugin->m_dictDirs.clear();
    for (int i = 0; i < dictDirsList->count(); ++i)
        m_plugin->m_dictDirs << dictDirsList->item(i)->text();
}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

