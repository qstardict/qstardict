/*****************************************************************************
 * downloadprogressdialog.h - QStarDict, a StarDict clone written with using Qt*
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

#ifndef DOWNLOADPROGRESSDIALOG_H
#define DOWNLOADPROGRESSDIALOG_H

#include <QProgressDialog>

namespace QStarDict
{

/**
 * The DownloadProgressDialog is a dialog that can be connected to
 * QHttp and show download progress.
 */
class DownloadProgressDialog: public QProgressDialog
{
    Q_OBJECT

    public:
        DownloadProgressDialog(QWidget *parent = 0);
         
        /**
         * Get data and write to out.
         */
        static int httpGet(QWidget *parent, const QString &host, const QString &path, QIODevice *out,
                const QString &title = QString(), const QString &label = QString(), bool *isOk = 0);
        /**
         * Get data and write to file.
         */
        static int httpGet(QWidget *parent, const QString &host, const QString &path, const QString &filename,
                const QString &title = QString(), const QString &label = QString(), bool *isOk = 0);

    private slots:
        void setValue(int done, int total);
        void loadingDone(bool error);

    private:
        bool m_isOk;
};

}

#endif // DOWNLOADPROGRESSDIALOG_H

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent

