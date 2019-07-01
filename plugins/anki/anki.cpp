/*****************************************************************************
 * anki.cpp - QStarDict, a StarDict clone written with using Qt              *
 * Copyright (C) 2018 Alexander Rodin                                        *
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

#include "anki.h"
#include "settingsdialog.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSettings>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QMessageBox>
#include <QTextDocumentFragment>

namespace {
std::optional<QString> variantToOptional(const QVariant &variant)
{
    if (variant.isNull())
        return std::nullopt;
    else
        return variant.toString();
}
}

QIcon Anki::pluginIcon() const
{
    return QIcon(":/icons/anki.png");
}

Anki::Anki(QObject *parent)
    : QObject(parent)
{
    QSettings settings("qstardict", "qstardict");
    m_connectUrl = settings.value("Anki/connectUrl", "http://127.0.0.1:8765").toString();
    m_deckName = settings.value("Anki/deckName", "Default").toString();
    m_allowDuplicates = settings.value("Anki/allowDuplicates", false).toBool();
    m_basicCard = settings.value("Anki/basicCard", true).toBool();
    m_basicCardDeckName = variantToOptional(settings.value("Anki/basicCardDeckName"));
    m_reversedBasicCard = settings.value("Anki/reversedBasicCard", true).toBool();
    m_reversedBasicCardDeckName = variantToOptional(settings.value("Anki/reversedBasicCardDeckName"));
    m_typeInCard = settings.value("Anki/typeInCard", true).toBool();
    m_typeInCardDeckName = variantToOptional(settings.value("Anki/typeInCardDeckName"));
    m_reversedTypeInCard = settings.value("Anki/reversedTypeInCard", true).toBool();
    m_reversedTypeInCardDeckName = variantToOptional(settings.value("Anki/reversedTypeInCardDeckName"));
}

Anki::~Anki()
{
    QSettings settings("qstardict", "qstardict");
    settings.setValue("Anki/connectUrl", m_connectUrl);
    settings.setValue("Anki/deckName", m_deckName);
    settings.setValue("Anki/allowDuplicates", m_allowDuplicates);
}

QIcon Anki::toolbarIcon() const {
    return QIcon(":/icons/anki.png");
}

QString Anki::toolbarText() const {
    return tr("Add word to &Anki");
}


void Anki::execute(QStarDict::DictWidget *dictWidget) {
    auto translatedWord = dictWidget->translatedWord();
    auto cursor = dictWidget->translationView()->textCursor();
    QString translation;
    if (cursor.hasSelection()) {
        translation = cursor.selection().toHtml("UTF-8");
    } else {
        translation = dictWidget->translationView()->document()->toHtml("UTF-8");
    }

    QVector<Card> cards;
    if (m_basicCard) {
        cards.push_back({
            "Basic",
            m_basicCardDeckName ? *m_basicCardDeckName : m_deckName,
            translatedWord,
            translation
        });
    }
    if (m_reversedBasicCard) {
        cards.push_back({
            "Basic",
            m_reversedBasicCardDeckName ? *m_reversedBasicCardDeckName : m_deckName,
            translation,
            translatedWord
        });
    }
    if (m_typeInCard) {
        cards.push_back({
            "Basic (type in the answer)",
            m_typeInCardDeckName ? *m_typeInCardDeckName : m_deckName,
            translatedWord,
            translation
        });
    }
    if (m_reversedTypeInCard) {
        cards.push_back({
            "Basic (type in the answer)",
            m_reversedTypeInCardDeckName ? *m_reversedTypeInCardDeckName : m_deckName,
            translation,
            translatedWord
        });
    }

    m_showError = true;
    for (const auto& card: cards) {
        createDeck(card.deckName, [=](bool success) {
            if (!success)
            {
                showError(dictWidget);
                return;
            }
            auto cursor = dictWidget->translationView()->textCursor();
            if (cursor.hasSelection()) {
                cursor.clearSelection();
                dictWidget->translationView()->setTextCursor(cursor);
            }
            createCard(card, [=](bool success) {
                if (!success)
                    showError(dictWidget);
            });
        });
    }
}

void Anki::createDeck(const QString &deckName, std::function<void(bool)> callback)
{
    QJsonObject createDeckObject;
    createDeckObject.insert("action", QString("createDeck"));
    createDeckObject.insert("version", 6);
    QJsonObject createDeckParamsObject;
    createDeckParamsObject.insert("deck", deckName);
    createDeckObject.insert("params", createDeckParamsObject);

    sendRequest(createDeckObject, callback);
}

void Anki::createCard(const Card &card, std::function<void(bool)> callback)
{
    QJsonObject requestObject;
    requestObject.insert("action", QString("addNote"));
    requestObject.insert("version", 6);
    QJsonObject paramsObject;
    QJsonObject noteObject;
    noteObject.insert("deckName", card.deckName);
    noteObject.insert("modelName", card.modelName);
    QJsonObject fieldsObject;
    fieldsObject.insert("Front", card.front);
    fieldsObject.insert("Back", card.back);
    noteObject.insert("fields", fieldsObject);
    QJsonObject optionsObject;
    optionsObject.insert("allowDuplicate", m_allowDuplicates);
    noteObject.insert("options", optionsObject);
    QJsonArray tagsArray;
    noteObject.insert("tags", tagsArray);
    paramsObject.insert("note", noteObject);
    requestObject.insert("params", paramsObject);

    sendRequest(requestObject, callback);
}

void Anki::sendRequest(const QJsonObject &requestObject, std::function<void(bool)> callback)
{
    QJsonDocument requestDocument;
    requestDocument.setObject(requestObject);

    auto networkAccessManager = new QNetworkAccessManager(this);
    QNetworkRequest request(m_connectUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    connect(networkAccessManager, &QNetworkAccessManager::finished, [=](QNetworkReply *reply) {
        callback(reply->error() == QNetworkReply::NoError);
        networkAccessManager->deleteLater();
    });

    networkAccessManager->post(request, requestDocument.toJson());
}

void Anki::showError(QWidget *parent)
{
    if (!m_showError)
        return;
    m_showError = false;
    QMessageBox::critical(
        parent,
        tr("Anki error"),
        tr("Unable to add the word to Anki: network error. <br>" \
           "Check if Anki is running and " \
           "<a href=\"https://ankiweb.net/shared/info/2055492159\">AnkiConnect</a> " \
           "add-on is installed to Anki. You probably would like to also install " \
           "<a href=\"https://ankiweb.net/shared/info/85158043\">Minimize to tray</a> "\
           "add-on to Anki."));
}

int Anki::execSettingsDialog(QWidget *parent)
{
    ::SettingsDialog dialog(this, parent);
    return dialog.exec();
}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc
