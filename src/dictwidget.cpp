/*****************************************************************************
 * dictwidget.cpp - QStarDict, a StarDict clone written with using Qt        *
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

#include "dictwidget.h"

#include <QTextBrowser>
#include <QHBoxLayout>
#include <QScrollBar>

namespace
{
const QString translationCSS = 
    "font.normal {\n"
        " }\n"
    "font.dict_name {\n"
        "color: blue;\n"
        "font-style: italic; }\n"
    "font.title {\n"
        "font-size: x-large;\n"
        "font-weight: bold; }\n"
    "font.explanation {\n"
        "color: #7f7f7f;\n"
        "font-style: italic; }\n"
    "font.abbreviature {\n"
        "font-style: italic; }\n"
    "font.example {\n"
        "font-style: italic; }\n"
    "font.transcription {\n"
    "font-weight: bold; }\n";
}

DictWidget::DictWidget(QWidget *parent, Qt::WindowFlags f)
    : QFrame(parent, f)
{
    translationView = new QTextBrowser(this);
    setFrameStyle(translationView->frameStyle());
    translationView->setFrameStyle(QFrame::NoFrame);
    translationView->verticalScrollBar()->setCursor(Qt::ArrowCursor);
    translationView->horizontalScrollBar()->setCursor(Qt::ArrowCursor);
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->addWidget(translationView);
    setLayout(layout);
    m_dict = 0;
    m_translationFlags = DictCore::Html | DictCore::Reformat | DictCore::ExpandAbbreviations;
}

bool DictWidget::translate(const QString &str)
{
    m_translatedWord = str;
    QString result = m_dict->translate(str, m_translationFlags);
    translationView->setHtml("<style>\n" + translationCSS + "</style>\n" + result);
    return result.isEmpty();
}

QString DictWidget::translatedWord() const
{
    return m_translatedWord;
}

void DictWidget::clear()
{
    translationView->clear();
}

const QString& DictWidget::cssStyle()
{
    return translationCSS;
}
