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

#include "dictbrowsersearch.h"


namespace QStarDict
{

DictBrowserSearch::DictBrowserSearch(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    connect(searchEdit, SIGNAL(textEdited(QString)), this, SLOT(searchAll()));
    connect(caseSensitiveCheckBox, SIGNAL(clicked()), this, SLOT(searchAll()));
    connect(wholeWordsCheckBox, SIGNAL(clicked()), this, SLOT(searchAll()));
}

void DictBrowserSearch::searchAll()
{
    QTextDocument::FindFlags f = 0;
    if (caseSensitiveCheckBox->isChecked())
        f |= QTextDocument::FindCaseSensitively;
    if (wholeWordsCheckBox->isChecked())
        f |= QTextDocument::FindWholeWords;
    emit search(searchEdit->text(), f);
}

void DictBrowserSearch::searchResult(bool success)
{
    QPalette p;
    if (!success)
    {
        p.setColor(QPalette::Base, QColor(229, 152, 169));
    }
    searchEdit->setPalette(p);
}

void DictBrowserSearch::showEvent(QShowEvent * event)
{
    emit searchActive(true);
    QWidget::showEvent(event);
    searchEdit->setFocus(Qt::OtherFocusReason);
    if (searchEdit->text().length())
        searchAll();
}

void DictBrowserSearch::hideEvent(QHideEvent * event)
{
    emit searchActive(false);
    QWidget::hideEvent(event);
}

void DictBrowserSearch::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        hide();
    }
}

} // namespace
