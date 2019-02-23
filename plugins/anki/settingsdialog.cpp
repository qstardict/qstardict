/*****************************************************************************
 * settingsdialog.cpp - QStarDict, a StarDict clone written using Qt         *
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

#include <QSettings>
#include "../pluginserver.h"

SettingsDialog::SettingsDialog(Anki *plugin, QWidget *parent)
    : QDialog(parent),
      m_plugin(plugin)
{
    setupUi(this);
    connectUrlEdit->setText(m_plugin->connectUrl());
    deckNameEdit->setText(m_plugin->deckName());
    modelNameEdit->setText(m_plugin->modelName());
    allowDuplicatesBox->setChecked(m_plugin->allowDuplicates());
}

void SettingsDialog::accept()
{
    m_plugin->setConnectUrl(connectUrlEdit->text());
    m_plugin->setDeckName(deckNameEdit->text());
    m_plugin->setModelName(modelNameEdit->text());
    m_plugin->setAllowDuplicates(allowDuplicatesBox->isChecked());
    QDialog::accept();
}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

