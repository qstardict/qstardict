/********************************************************************************
 * cssstyle.cpp - QStarDict, a StarDict clone written with using Qt             *
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

#include "cssstyle.h"

CSSStyle::CSSStyle(QObject *parent)
    : QObject(parent)
{
}

void CSSStyle::setElement(const QString &element, const QFont &font, const QColor &color)
{
    m_elements[element].font = font;
    m_elements[element].color = color;
    updateCSS();
}

const QFont& CSSStyle::elementFont(const QString &element) const
{
    if (m_elements.contains(element))
        return m_elements.value(element).font;
}

const QColor& CSSStyle::elementColor(const QString &element) const
{
     if (m_elements.contains(element))
        return m_elements.value(element).color;
}

const QString& CSSStyle::css() const
{
    return m_css;
}

void CSSStyle::updateCSS()
{
    m_css.clear();

    for (QHash<QString, Element>::const_iterator i = m_elements.begin(); i != m_elements.end(); ++i)
    {
        m_css += i->key() + "\n{\n";
        m_css += "color: " +i->value().color.name() + ";\n";
        m_css += "font-family: \"" + i->value().font.family() + "\";\n";
        if (i->value().font.bold())
            m_css += "font-weight: bold;\n";
        if (i->value().font.italic())
            m_css += "font-style: italic;\n";
        if (i->value().font.underline())
            m_css += "text-decoration: underline;\n";
        m_css += "}\n";
    }
}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent

