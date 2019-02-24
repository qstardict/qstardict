/*****************************************************************************
 * print.cpp - QStarDict, a StarDict clone written with using Qt             *
 * Copyright (C) 2019 Alexander Rodin                                        *
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

#include "savetofile.h"
#include <QFileDialog>
#include <QDir>
#include <QTextStream>
#include <QMessageBox>

QIcon SaveToFile::pluginIcon() const
{
    return QIcon(":/icons/document-save-as.png");
}

SaveToFile::SaveToFile(QObject *parent)
    : QObject(parent)
{
}

QIcon SaveToFile::toolbarIcon() const {
    return QIcon(":/icons/document-save-as.png");
}

QString SaveToFile::toolbarText() const {
    return tr("&Save translation");
}

void SaveToFile::execute(QStarDict::DictWidget *dictWidget) {
    static QDir dir( QDir::homePath() ); //added by Frank
    static QString filter(tr("Text files (*.txt)")); //added by Frank

    QFileDialog dialog(dictWidget, tr("Save translation"),
                       dir.path(), filter); //updated by Frank
    dialog.selectFile(dictWidget->translatedWord());//added by Frank
    dialog.setNameFilters(QStringList() << tr("HTML files (*.html *.htm)") << tr("Text files (*.txt)"));//updated by Frank
    dialog.selectNameFilter(filter); //added by Frank

    if (dialog.exec() && dialog.selectedFiles().size())
    {
        QString fileName = dialog.selectedFiles().first();
        /*QString*/ filter = dialog.selectedNameFilter();//updated by Frank
        dir = dialog.directory(); //added by Frank
        if (filter == tr("HTML files (*.html, *.htm)") &&
            ! (fileName.endsWith(".html", Qt::CaseInsensitive) || fileName.endsWith(".htm", Qt::CaseInsensitive)))
            fileName += ".html";
        else if (filter == tr("Text files (*.txt)") && ! fileName.endsWith(".txt", Qt::CaseInsensitive))
            fileName += ".txt";

        QFile outputFile(fileName);
        if (! outputFile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMessageBox::warning(dictWidget, tr("Error"),
                                 tr("Cannot save translation as %1").arg(fileName));
            return;
        }
        QTextStream outputStream(&outputFile);
        if (filter == tr("HTML files (*.html, *.htm)"))
            outputStream << dictWidget->translationView()->document()->toHtml("UTF-8");
        else
            outputStream << dictWidget->translationView()->toPlainText();
    }
}
