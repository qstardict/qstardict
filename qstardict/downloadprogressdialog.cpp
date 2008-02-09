/*****************************************************************************
 * downloadprogressdialog.cpp - QStarDict, a StarDict clone written with using Qt*
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

#include "downloadprogressdialog.h"

#include <QFile>
#include <QLabel>
#include <QHttp>

DownloadProgressDialog::DownloadProgressDialog(QWidget *parent)
    : QProgressDialog(parent),
    m_isOk(false)
{
    setWindowTitle(tr("Fetching progress"));
}

int DownloadProgressDialog::httpGet(QWidget *parent, const QString &host, const QString &path, QIODevice *out,
        const QString &title, const QString &label, bool *isOk)
{
    QHttp http;
    http.setHost(host);
    DownloadProgressDialog progressDialog(parent);
    if (! title.isEmpty())
        progressDialog.setWindowTitle(title);
    if (! label.isEmpty())
        progressDialog.setLabel(new QLabel(label));
    connect(&http, SIGNAL(dataReadProgress(int, int)), &progressDialog, SLOT(setValue(int, int)));
    connect(&http, SIGNAL(done(bool)), &progressDialog, SLOT(loadingDone(bool)));
    http.get(path, out);
    int result = progressDialog.exec();
    if (isOk)
        *isOk = progressDialog.m_isOk;
    return result;
}

int DownloadProgressDialog::httpGet(QWidget *parent, const QString &host, const QString &path, const QString &filename,
        const QString &title, const QString &label, bool *isOk)
{
    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    return httpGet(parent, host, path, &file, title, label, isOk);
}

void DownloadProgressDialog::setValue(int done, int total)
{
    if (maximum() != total)
        setRange(0, total);
    QProgressDialog::setValue(done);
}

void DownloadProgressDialog::loadingDone(bool error)
{
    m_isOk = ! error;
    accept();
}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

