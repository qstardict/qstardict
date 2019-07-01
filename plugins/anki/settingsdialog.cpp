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
#include <QMessageBox>
#include "../pluginserver.h"

SettingsDialog::SettingsDialog(Anki *plugin, QWidget *parent)
    : QDialog(parent),
      m_plugin(plugin)
{
    setupUi(this);
    connectUrlEdit->setText(m_plugin->connectUrl());
    deckNameEdit->setText(m_plugin->deckName());
    allowDuplicatesBox->setChecked(m_plugin->allowDuplicates());

    basicCardBox->setChecked(m_plugin->basicCard());
    basicCardDeckNameBox->setChecked(m_plugin->basicCardDeckName().has_value());
    if (m_plugin->basicCardDeckName())
        basicCardDeckNameEdit->setText(*m_plugin->basicCardDeckName());
    basicCardDeckNameWidget->setEnabled(basicCardBox->isChecked());
    basicCardDeckNameEdit->setEnabled(basicCardDeckNameBox->isChecked());

    reversedBasicCardDeckNameWidget->setEnabled(reversedBasicCardBox->isChecked());
    reversedBasicCardDeckNameBox->setChecked(m_plugin->reversedBasicCardDeckName().has_value());
    if (m_plugin->reversedBasicCardDeckName())
        reversedBasicCardDeckNameEdit->setText(*m_plugin->reversedBasicCardDeckName());
    reversedBasicCardDeckNameEdit->setEnabled(reversedBasicCardDeckNameBox->isChecked());

    typeInCardDeckNameWidget->setEnabled(typeInCardBox->isChecked());
    typeInCardDeckNameBox->setChecked(m_plugin->typeInCardDeckName().has_value());
    if (m_plugin->typeInCardDeckName())
        typeInCardDeckNameEdit->setText(*m_plugin->typeInCardDeckName());
    typeInCardDeckNameEdit->setEnabled(typeInCardDeckNameBox->isChecked());

    reversedTypeInCardDeckNameWidget->setEnabled(reversedTypeInCardBox->isChecked());
    reversedTypeInCardDeckNameBox->setChecked(m_plugin->reversedTypeInCardDeckName().has_value());
    if (m_plugin->reversedTypeInCardDeckName())
        reversedTypeInCardDeckNameEdit->setText(*m_plugin->reversedTypeInCardDeckName());
    reversedTypeInCardDeckNameEdit->setEnabled(reversedTypeInCardDeckNameBox->isChecked());
}

void SettingsDialog::accept()
{
    // validate data
    if (deckNameEdit->text().isEmpty())
    {
        QMessageBox::critical(this,
                tr("Validation error"),
                tr("Deck name cannot be empty"));
        deckNameEdit->setFocus();
        return;
    }
    if (basicCardDeckNameBox->isChecked() && basicCardDeckNameEdit->text().isEmpty())
    {
        QMessageBox::critical(this,
                tr("Validation error"),
                tr("Deck name for basic card cannot be empty"));
        basicCardDeckNameEdit->setFocus();
        return;
    }
    if (reversedBasicCardDeckNameBox->isChecked() && reversedBasicCardDeckNameEdit->text().isEmpty())
    {
        QMessageBox::critical(this,
                tr("Validation error"),
                tr("Deck name for reversed basic card cannot be empty"));
        reversedBasicCardDeckNameEdit->setFocus();
        return;
    }
    if (typeInCardDeckNameBox->isChecked() && typeInCardDeckNameEdit->text().isEmpty())
    {
        QMessageBox::critical(this,
                tr("Validation error"),
                tr("Deck name for type in card cannot be empty"));
        typeInCardDeckNameEdit->setFocus();
        return;
    }
    if (reversedTypeInCardDeckNameBox->isChecked() && reversedTypeInCardDeckNameEdit->text().isEmpty())
    {
        QMessageBox::critical(this,
                tr("Validation error"),
                tr("Deck name for reversed type in card cannot be empty"));
        return;
    }

    // apply settings
    m_plugin->setConnectUrl(connectUrlEdit->text());
    m_plugin->setDeckName(deckNameEdit->text());
    m_plugin->setAllowDuplicates(allowDuplicatesBox->isChecked());

    m_plugin->setBasicCard(basicCardBox->isChecked());
    if (basicCardDeckNameBox->isChecked())
        m_plugin->setBasicCardDeckName(basicCardDeckNameEdit->text());
    else
        m_plugin->setBasicCardDeckName(std::nullopt);

    m_plugin->setReversedBasicCard(reversedBasicCardBox->isChecked());
    if (reversedBasicCardDeckNameBox->isChecked())
        m_plugin->setReversedBasicCardDeckName(basicCardDeckNameEdit->text());
    else
        m_plugin->setReversedBasicCardDeckName(std::nullopt);

    m_plugin->setTypeInCard(typeInCardBox->isChecked());
    if (typeInCardDeckNameBox->isChecked())
        m_plugin->setTypeInCardDeckName(typeInCardDeckNameEdit->text());
    else
        m_plugin->setTypeInCardDeckName(std::nullopt);

    m_plugin->setReversedTypeInCard(reversedTypeInCardBox->isChecked());
    if (reversedTypeInCardDeckNameBox->isChecked())
        m_plugin->setReversedTypeInCardDeckName(typeInCardDeckNameEdit->text());
    else
        m_plugin->setReversedTypeInCardDeckName(std::nullopt);

    QDialog::accept();
}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

