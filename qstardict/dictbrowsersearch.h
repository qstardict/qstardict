/*****************************************************************************
 * keyboard.h - QStarDict, a StarDict clone written with using Qt            *
 * Copyright (C) 2007 Petr Vanek                                             *
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

#ifndef DICTBROWSERSEARCH_H
#define DICTBROWSERSEARCH_H

#include <QTextDocument>

#include "ui_dictbrowsersearch.h"

namespace QStarDict
{

class DictBrowserSearch : public QWidget, private Ui::DictBrowserSearch
{
    Q_OBJECT

public:
    explicit DictBrowserSearch(QWidget *parent = 0);

signals:
    void search(const QString & exp, QTextDocument::FindFlags options);

public slots:
    void searchResult(bool success);

private slots:
    void searchAll();

private:
    void showEvent(QShowEvent * event);
    void closeEvent(QCloseEvent * event);
};

} // namespace

#endif // DICTBROWSERSEARCH_H
