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

#include <QFile>
#include <QDir>
#include <QSettings>
#include <QListWidgetItem>
#include <QTextCodec>
#include "ui_adddictionarydialog.h"

namespace
{
QStringList supportedCharsets()
{
    QList<QByteArray> list = QTextCodec::availableCodecs();
    QStringList newList;
    for (QList<QByteArray>::const_iterator i = list.begin(); i != list.end(); ++i)
        newList << *i;
    return newList;
}
}

SettingsDialog::SettingsDialog(Web *plugin, QWidget *parent)
    : QDialog(parent),
      m_plugin(plugin)
{
    setupUi(this);
    
    QStringList filenames = QDir(plugin->workPath()).entryList(QStringList("*.webdict"), QDir::Files, QDir::Name);
    for (QStringList::iterator i = filenames.begin(); i != filenames.end(); ++i)
    {
        QSettings dict(plugin->workPath() + "/" + *i, QSettings::IniFormat);
        m_oldDicts[i->remove(".webdict")] = 
            Dict(dict.value("author").toString(), dict.value("description").toString(),
            dict.value("query").toString(), dict.value("charset").toByteArray());
    }
    m_dicts = m_oldDicts;

    refresh();
}

void SettingsDialog::on_editDictButton_clicked()
{
    if (dictsList->currentRow() == -1)
        return;
    QString dict = dictsList->currentItem()->text();
    Ui::AddDictionaryDialog ui;
    QDialog dialog(this);
    ui.setupUi(&dialog);
    dialog.setWindowTitle(tr("Edit dictionary"));
    ui.nameEdit->setText(dict);
    ui.authorEdit->setText(m_dicts[dict].author);
    ui.descEdit->setText(m_dicts[dict].description);
    ui.queryEdit->setText(m_dicts[dict].query);
    ui.charsetEdit->addItems(supportedCharsets());
    ui.charsetEdit->setCurrentIndex(ui.charsetEdit->findText(m_dicts[dict].charset));
    if (dialog.exec() != QDialog::Accepted)
        return;
    if (ui.nameEdit->text() != dict)
    {
        m_dicts.remove(dict);
        dict = ui.nameEdit->text();
    }
    m_dicts[dict].author = ui.authorEdit->text();
    m_dicts[dict].description = ui.descEdit->toPlainText();
    m_dicts[dict].query = ui.queryEdit->text();
    m_dicts[dict].charset = ui.charsetEdit->currentText().toAscii();
    refresh();
}

void SettingsDialog::on_addDictButton_clicked()
{
    Ui::AddDictionaryDialog ui;
    QDialog dialog(this);
    ui.setupUi(&dialog);
    ui.charsetEdit->addItems(supportedCharsets());
    ui.charsetEdit->setCurrentIndex(ui.charsetEdit->findText("UTF-8"));
    if (dialog.exec() != QDialog::Accepted)
        return;
    m_dicts[ui.nameEdit->text()] =
        Dict(ui.authorEdit->text(), ui.descEdit->toPlainText(), ui.queryEdit->text());
    refresh();
}

void SettingsDialog::on_removeDictButton_clicked()
{
    QListWidgetItem *item = dictsList->takeItem(dictsList->currentRow());
    m_dicts.remove(item->text());
    delete item;
}

void SettingsDialog::refresh()
{
    dictsList->clear();
    dictsList->insertItems(0, m_dicts.keys());
}

void SettingsDialog::accept()
{
    for (QHash<QString, Dict>::const_iterator i = m_dicts.begin(); i != m_dicts.end(); ++i)
    {
        QSettings dict(m_plugin->workPath() + "/" + i.key() + ".webdict", QSettings::IniFormat);
        dict.setValue("author", i->author);
        dict.setValue("description", i->description);
        dict.setValue("query", i->query);
        dict.setValue("charset", i->charset);
        m_oldDicts.remove(i.key());
    }
    for (QHash<QString, Dict>::const_iterator i = m_oldDicts.begin(); i != m_oldDicts.end(); ++i)
        QFile::remove(m_plugin->workPath() + "/" + i.key() + ".webdict");

    QDialog::accept();
}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

