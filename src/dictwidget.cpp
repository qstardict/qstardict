/********************************************************************************
 * dictwidget.cpp - QStarDict, a StarDict clone written with using Qt           *
 * Copyright (C) 2007 Alexander Rodin                                           *
 *                                                                              *
 * This program is free software: you can redistribute it and/or modify         *
 * it under the terms of the GNU General Public License as published by         *
 * the Free Software Foundation, either version 3 of the License, or            *
 * (at your option) any later version.                                          *
 *                                                                              *
 * This program is distributed in the hope that it will be useful,              *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of               *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                *
 * GNU General Public License for more details.                                 *
 *                                                                              *
 * You should have received a copy of the GNU General Public License            *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.        *
 ********************************************************************************/

#include "dictwidget.h"

#include <QTextBrowser>
#include <QHBoxLayout>
#include <cassert>

DictWidget::DictWidget(QWidget *parent, Qt::WindowFlags f)
    : QFrame(parent, f)
{
    translationView = new QTextBrowser(this);
    setFrameStyle(translationView->frameStyle());
    translationView->setFrameStyle(QFrame::NoFrame);
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->addWidget(translationView);
    setLayout(layout);
    m_dict = 0;
    m_flags = DictCore::Html | DictCore::Reformat | DictCore::ExpandAbbreviations;
}

void DictWidget::setDict(DictCore *dict)
{
    m_dict = dict;
}

const DictCore* DictWidget::dict() const
{
    return m_dict;
}

void DictWidget::setTranslationFlags(DictCore::TranslationFlags flags)
{
    m_flags = flags | DictCore::Html;
}

DictCore::TranslationFlags DictWidget::translationFlags() const
{
    return m_flags;
}

bool DictWidget::translate(const QString &str)
{
    const QString htmlStyle = 
        "<style>"
        "font.normal { "
            "font-family: \"Sans Serif\", \"Arial\", \"Sans\"; "
            "color: black; } "
        "font.dict_name { "
            "color: blue; "
            "font-style: italic; }"
        "font.title { "
            "font-size: 16pt; "
            "font-weight: bold; }"
        "font.explanation { "
            "color: #7f7f7f; "
            "font-style: italic; }"
        "font.abbreviature { "
            "font-weight: bold; }"
        "font.example { "
            "font-style: italic; } "
        "font.transcription { "
            "font-weight: bold; }"
        "</style>";
    m_translatedWord = str;
    QString result = m_dict->translate(str, m_flags);
    translationView->setHtml(htmlStyle + result);
    return result.isEmpty();
}

QString DictWidget::translatedWord() const
{
    return m_translatedWord;
}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent

