/*****************************************************************************
 * dictbrowser.cpp - QStarDict, a StarDict clone written with using Qt       *
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

#include "dictbrowser.h"

#include <QDesktopServices>
#include <QMouseEvent>
#include <QTextBlock>
#include <QTextCharFormat>
#include <QTextDocument>
#include <QTextDocumentFragment>
#include "keyboard.h"

#include <QDebug>

namespace
{
const QString translationCSS = 
    "body {\n"
        "font-size: 10pt; }\n"
    "font.dict_name {\n"
        "color: blue;\n"
        "font-style: italic; }\n"
    "font.title {\n"
        "font-size: 16pt;\n"
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

namespace QStarDict
{

DictBrowser::DictBrowser(QWidget *parent)
    : QTextBrowser(parent),
      m_dict(0),
      m_highlighted(false)
{
    document()->setDefaultStyleSheet(translationCSS);
}

QVariant DictBrowser::loadResource(int type, const QUrl &name)
{
    if (type == QTextDocument::HtmlResource)
    {
        QString result = m_dict->translate(name.toString());
        if (result.isEmpty())
            result = "<table><tr><td><img src=\":/icons/dialog-warning.png\" width=64 height=64/></td><td valign=middle>" +
                tr("The word <b>%1</b> is not found.").arg(name.toString()) +
                "</td></tr></table>";
        return "<title>Translation for \"" + name.toString() + "\"</title>\n"
            + "<body>" + result + "</body>";
    }
    return QTextBrowser::loadResource(type, name);
}

void DictBrowser::mouseMoveEvent(QMouseEvent *event)
{
    if (m_highlighted)
    {
        m_oldCursor.setCharFormat(m_oldFormat);
        m_highlighted = false;
    }
    if (Keyboard::activeModifiers().testFlag(Qt::ControlModifier))
    {
        QTextCursor cursor = cursorForPosition(event->pos());
        cursor.select(QTextCursor::WordUnderCursor);
        QString selection = cursor.selection().toPlainText().simplified();
        if (m_dict->isTranslatable(selection))
        {
            m_oldCursor = cursor;
            m_oldFormat = cursor.charFormat();

            QTextCharFormat format = m_oldFormat;
            format.setForeground(Qt::blue);
            format.setFontUnderline(true);
            cursor.setCharFormat(format);

            m_highlighted = true;
        }
    }

    QTextBrowser::mouseMoveEvent(event);
}

void DictBrowser::mousePressEvent(QMouseEvent *event)
{
    if (Keyboard::activeModifiers().testFlag(Qt::ControlModifier))
    {
        QTextCursor cursor = cursorForPosition(event->pos());
        cursor.select(QTextCursor::WordUnderCursor);
        QString selection = cursor.selection().toPlainText().simplified();
        if (m_dict->isTranslatable(selection))
        {
            setSource(selection);
            if (m_highlighted)
                m_highlighted = false;
        }
    }
    QTextBrowser::mousePressEvent(event);
}

}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

