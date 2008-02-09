/*****************************************************************************
 * adddictionarydialog.cpp - QStarDict, a StarDict clone written with using Qt*
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

#include "adddictionarydialog.h"

#include <QFile>
#include <QHttp>
#include <QDomDocument>
#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include <QInputDialog>
#include <QUrl>
#include <QProcess>
#include <QFile>
#include <QProgressDialog>
#include <QLabel>
#include "downloadprogressdialog.h"
#include "iso639.h"
#include "dictcore.h"

namespace QStarDict 
{

AddDictionaryDialog::AddDictionaryDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);
    buttonBox->addButton(addButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(infoButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(refreshButton, QDialogButtonBox::ActionRole);
    dictsList->sortByColumn(0, Qt::AscendingOrder);
}

void AddDictionaryDialog::on_refreshButton_clicked()
{
    refreshDictsList(true);
}

void AddDictionaryDialog::on_addButton_clicked()
{
    QString path = dicts[dictsList->currentItem()->data(1).toInt()].downLink;
    bool ok;
    QString mirror = QInputDialog::getItem(this,
            tr("Mirror selection"),
            tr("Select a mirror for download dictionary"),
            mirrors,
            0,
            false,
            &ok);
    if (! ok)
        return;
    QUrl url(mirror + path);
    QString filename = DictCore::localDictsDir() + QDir::separator() + path;
    if (QFile::exists(filename))
        QFile::remove(filename);
    else if (! QDir(DictCore::localDictsDir()).exists())
        QDir::root().mkpath(DictCore::localDictsDir());
    if (DownloadProgressDialog::httpGet(this, url.host(), url.path(), filename,
                QString(), tr("Fetching %1...").arg(filename), &ok) == QDialog::Rejected)
    {
        qDebug() << "Fetching dialog is rejected";
        return;
    }
    if (! ok)
    {
        QMessageBox::critical(this, tr("Fech failed"), tr("Cannot fetch %1").arg(url.path()));
        return;
    }

    // Extract dictionary
    QProcess extractProcess;
    QProgressDialog extractProgressDialog(this);
    extractProgressDialog.setWindowTitle(tr("Extracting..."));
    extractProgressDialog.setLabel(new QLabel(tr("Extracting %1 to %2").arg(path).arg(DictCore::localDictsDir())));
    extractProgressDialog.setRange(0, 0);
    extractProgressDialog.setMinimumDuration(100);
    connect(&extractProcess, SIGNAL(finished(int, QProcess::ExitStatus)), &extractProgressDialog, SLOT(close()));
    extractProcess.start("tar xjf \"" + filename + "\" -C \"" + DictCore::localDictsDir() + "\"", QIODevice::NotOpen);
    extractProgressDialog.exec();
    if (extractProcess.exitCode() != 0)
    {
        QMessageBox::critical(this, tr("Extraction failed"), tr("Cannot extract %1").arg(url.path()));
        return;
    }
}

void AddDictionaryDialog::on_infoButton_clicked()
{
    Dict dict = dicts[dictsList->currentItem()->data(1).toInt()];
    // Convert description to HTML format
    QString description = dict.description;
    int pos = 0;
    QRegExp regExp("(http|https|ftp)://[^\\s\\;\\,]*");
    while ((pos = regExp.indexIn(description, pos)) != -1)
    {
        QString url = description.mid(pos, regExp.matchedLength());
        QString link = "<a href=\"" + url + "\">" + url + "</a>";
        description.replace(pos, regExp.matchedLength(), link);
        pos += link.length();
    }
    description.replace("\n", "<br>");

    QMessageBox::information(this,
            tr("Dictionary information"),
            tr("<b>Dictionary:</b> %1").arg(dict.name) + "<br>" +
            tr("<b>From:</b> %1").arg(ISO639::languageName(dict.langFrom)) + "<br>" +
            tr("<b>To:</b> %1").arg(ISO639::languageName(dict.langTo)) + "<br>" +
            tr("<b>Filename:</b> %1").arg(dict.downLink) + "<br>" +
            tr("<b>Download size:</b> %1 KB").arg(dict.size / 1024) + "<br>" +
            tr("<b>Description:</b>") + "<br>" + description);
}

void AddDictionaryDialog::refreshDictsList(bool reload)
{
    // Clear data
    dictsList->clear();
    dicts.clear();
    mirrors.clear();

#ifdef Q_OS_UNIX
    QString xmlFilename = QDir::homePath() + "/.config/qstardict/dict_repo.xml";
#else
    QString xmlFilename = QDir::homePath() + "/qstardict_dict_repo.xml";
#endif
    // Create DOM document
    QDomDocument xmlDocument;
    QFile xmlFile(xmlFilename);
    xmlFile.open(QIODevice::ReadOnly);
    if (reload || ! xmlDocument.setContent(&xmlFile))
    {
        xmlFile.close();
        QFile::remove(xmlFilename);
        if (DownloadProgressDialog::httpGet(this, "xdxf.revdanica.com", "/down/repo_xml.php",
                    xmlFilename, QString(), tr("Fetching dictionaries list...")) == QDialog::Rejected)
        {
            qDebug() << "Dialog is rejected!!!";
            return;
        }
        xmlFile.open(QIODevice::ReadOnly);
        if (! xmlDocument.setContent(&xmlFile))
        {
            QMessageBox::critical(this, tr("Fech failed"), tr("Cannot fetch dictionaries list"));
            return;
        }
    }

    // Parse Mirrors
    QDomNodeList xmlMirrorsList = xmlDocument.elementsByTagName("mirror");
    for (uint i = 0; i < xmlMirrorsList.length(); ++i)
    {
        mirrors << xmlMirrorsList.at(i).toElement().text();
    }

    // Parse dictionaries
    QDomNodeList xmlDictsList = xmlDocument.elementsByTagName("dict");
    for (uint i = 0; i < xmlDictsList.length(); ++i)
    {
        Dict dict;
        QDomNodeList attrNodes = xmlDictsList.at(i).childNodes();
        for (uint j = 0; j < attrNodes.length(); ++j)
        {
            QString name = attrNodes.at(j).nodeName();
            QString text = attrNodes.at(j).firstChild().toCharacterData().data();
            if (name == "long_name")
                dict.name = text;
            else if (name == "lang_fr")
                dict.langFrom = text;
            else if (name == "lang_to")
                dict.langTo = text;
            else if (name == "info")
                dict.description = text;
            else if (name == "down_link")
                dict.downLink = "stardict-" + text.replace(".tar.bz2", "-2.4.2.tar.bz2");
            else if (name == "arc_bytes_qntt")
                dict.size = text.toULong();
        }
        dicts << dict;
    }

    dictsList->setRowCount(dicts.size());
    int i;
    for (i = 0; i < dicts.size(); ++i)
    {
        QTableWidgetItem *item;
        item = new QTableWidgetItem;
        item->setText(ISO639::languageName(dicts[i].langFrom));
        item->setData(1, QVariant(i));
        dictsList->setItem(i, 1, item);
        item = new QTableWidgetItem;
        item->setText(ISO639::languageName(dicts[i].langTo));
        item->setData(1, QVariant(i));
        dictsList->setItem(i, 2, item);
        item = new QTableWidgetItem;
        item->setText(QString::number(dicts[i].size / 1024) + tr(" KB"));
        item->setData(1, QVariant(i));
        dictsList->setItem(i, 3, item);
        // Not first because Qt behavior here is odd
        item = new QTableWidgetItem;
        item->setText(dicts[i].name);
        item->setData(1, QVariant(i));
        dictsList->setItem(i, 0, item);
    }
    dictsList->selectRow(0);
    dictsList->resizeColumnsToContents();
}

void AddDictionaryDialog::showEvent(QShowEvent*)
{
    refreshDictsList();
}

}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

