/********************************************************************************
 * cssstyle.h - QStarDict, a StarDict clone written with using Qt               *
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

#ifndef CSSSTYLE_H
#define CSSSTYLE_H

#include <QObject>
#include <QFont>
#include <QColor>
#include <QHash>

class CSSStyle
{
    public:
        CSSStyle(QObject *parent = 0);

        void setElement(const QString &element, const QFont &font, const QColor &color = QColor("#000000"));
        const QFont& elementFont(const QString &element) const;
        const QColor& elementColor(const QString &element) const;

        const QString& css() const;

    private:
        void updateCSS();

        struct Element
        {
            QFont font;
            QColor color;
        };
        QHash<QString, Element> m_elements;
        QString m_css;
};

#endif // CSSSTYLE_H

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent

