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

namespace QStarDict
{

QString DictBrowser::cssStyle()
{
    return translationCSS;
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
            "<style>\n" + translationCSS + "</style>\n"
            + result;
    }
    return QTextBrowser::loadResource(type, name);
}

}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

