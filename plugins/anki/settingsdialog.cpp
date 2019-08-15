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
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include "../pluginserver.h"

SettingsDialog::SettingsDialog(Anki *plugin, QWidget *parent)
    : QDialog(parent),
      m_plugin(plugin)
{
    setupUi(this);
    connectUrlEdit->setText(m_plugin->connectUrl());
    connect(connectTestButton, &QAbstractButton::clicked,
            this, &SettingsDialog::testAnkiConnection);

    deckNameEdit->setText(m_plugin->deckName());
    allowDuplicatesBox->setChecked(m_plugin->allowDuplicates());

    basicCardBox->setChecked(m_plugin->basicCard());
    basicCardDeckNameBox->setChecked(m_plugin->basicCardDeckName().has_value());
    if (m_plugin->basicCardDeckName())
        basicCardDeckNameEdit->setText(*m_plugin->basicCardDeckName());
    else
        basicCardDeckNameEdit->setText("Basic");
    basicCardDeckNameWidget->setEnabled(basicCardBox->isChecked());
    basicCardDeckNameEdit->setEnabled(basicCardDeckNameBox->isChecked());

    reversedBasicCardBox->setChecked(m_plugin->reversedBasicCard());
    reversedBasicCardDeckNameWidget->setEnabled(reversedBasicCardBox->isChecked());
    reversedBasicCardDeckNameBox->setChecked(m_plugin->reversedBasicCardDeckName().has_value());
    if (m_plugin->reversedBasicCardDeckName())
        reversedBasicCardDeckNameEdit->setText(*m_plugin->reversedBasicCardDeckName());
    else
        reversedBasicCardDeckNameEdit->setText("Basic Reversed");
    reversedBasicCardDeckNameEdit->setEnabled(reversedBasicCardDeckNameBox->isChecked());

    typeInCardBox->setChecked(m_plugin->typeInCard());
    typeInCardDeckNameWidget->setEnabled(typeInCardBox->isChecked());
    typeInCardDeckNameBox->setChecked(m_plugin->typeInCardDeckName().has_value());
    if (m_plugin->typeInCardDeckName())
        typeInCardDeckNameEdit->setText(*m_plugin->typeInCardDeckName());
    else
        typeInCardDeckNameEdit->setText("Type In");
    typeInCardDeckNameEdit->setEnabled(typeInCardDeckNameBox->isChecked());

    reversedTypeInCardBox->setChecked(m_plugin->reversedTypeInCard());
    reversedTypeInCardDeckNameWidget->setEnabled(reversedTypeInCardBox->isChecked());
    reversedTypeInCardDeckNameBox->setChecked(m_plugin->reversedTypeInCardDeckName().has_value());
    if (m_plugin->reversedTypeInCardDeckName())
        reversedTypeInCardDeckNameEdit->setText(*m_plugin->reversedTypeInCardDeckName());
    else
        reversedTypeInCardDeckNameEdit->setText("Type In Reversed");
    reversedTypeInCardDeckNameEdit->setEnabled(reversedTypeInCardDeckNameBox->isChecked());
}

void SettingsDialog::testAnkiConnection()
{
    if (!connectUrlEdit->isEnabled())
        return;

    connectTestButton->setChecked(true);
    connectUrlEdit->setEnabled(false);

    QJsonObject requestObject;
    requestObject.insert("action", QString("version"));
    requestObject.insert("version", 6);
    QJsonDocument requestDocument;
    requestDocument.setObject(requestObject);

    auto networkAccessManager = new QNetworkAccessManager(this);
    QNetworkRequest request(connectUrlEdit->text());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    connect(networkAccessManager, &QNetworkAccessManager::finished, [=](QNetworkReply *reply) {
        try
        {
            if (reply->error())
                throw tr("Unable to connect to Anki. Check if Anki is running and the URL is correct.");

            QJsonParseError error;
            auto document = QJsonDocument::fromJson(reply->readAll(), &error);
            if (error.error != QJsonParseError::NoError)
                throw tr("Unable to parse Anki response: %1").arg(error.errorString());

            if (!document.isObject())
                throw tr("Returned by Anki JSON is not an object");

            auto object = document.object();
            auto errorValue = object.value("error");
            if (errorValue != QJsonValue::Null && errorValue != QJsonValue::Undefined)
                throw tr("Anki returned an error: %1").arg(errorValue.toString());

            QMessageBox::information(this, tr("Connection succeded"),
                    tr("The connection to Anki is working!"));
        }
        catch (QString message)
        {
            QMessageBox::warning(this, tr("Connection error"), message);
        }

        connectTestButton->setChecked(false);
        connectUrlEdit->setEnabled(true);
        connectUrlEdit->setFocus();
        networkAccessManager->deleteLater();
    });
    networkAccessManager->post(request, requestDocument.toJson());
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
    if (basicCardDeckNameEdit->isEnabled() && basicCardDeckNameEdit->text().isEmpty())
    {
        QMessageBox::critical(this,
                tr("Validation error"),
                tr("Deck name for basic card cannot be empty"));
        basicCardDeckNameEdit->setFocus();
        return;
    }
    if (reversedBasicCardDeckNameEdit->isEnabled() && reversedBasicCardDeckNameEdit->text().isEmpty())
    {
        QMessageBox::critical(this,
                tr("Validation error"),
                tr("Deck name for reversed basic card cannot be empty"));
        reversedBasicCardDeckNameEdit->setFocus();
        return;
    }
    if (typeInCardDeckNameEdit->isEnabled() && typeInCardDeckNameEdit->text().isEmpty())
    {
        QMessageBox::critical(this,
                tr("Validation error"),
                tr("Deck name for type in card cannot be empty"));
        typeInCardDeckNameEdit->setFocus();
        return;
    }
    if (reversedTypeInCardDeckNameEdit->isEnabled() && reversedTypeInCardDeckNameEdit->text().isEmpty())
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
        m_plugin->setReversedBasicCardDeckName(reversedBasicCardDeckNameEdit->text());
    else
        m_plugin->setReversedBasicCardDeckName(std::nullopt);

    m_plugin->setTypeInCard(typeInCardBox->isChecked());
    if (typeInCardDeckNameBox->isChecked())
        m_plugin->setTypeInCardDeckName(typeInCardDeckNameEdit->text());
    else
        m_plugin->setTypeInCardDeckName(std::nullopt);

    m_plugin->setReversedTypeInCard(reversedTypeInCardBox->isChecked());
    if (reversedTypeInCardDeckNameBox->isChecked())
        m_plugin->setReversedTypeInCardDeckName(reversedTypeInCardDeckNameEdit->text());
    else
        m_plugin->setReversedTypeInCardDeckName(std::nullopt);

    m_plugin->saveSettings();
    QDialog::accept();
}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

