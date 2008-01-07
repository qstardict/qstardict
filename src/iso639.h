/*****************************************************************************
 * iso639.h - QStarDict, a StarDict clone written with using Qt              *
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

#ifndef ISO639_H
#define ISO639_H

#include <QStringList>
#include <QHash>

class ISO639
{
    public:
        /**
         * Return a list of ISO 639 3-letter language codes
         */
        static QStringList languages();
        /**
         * Return language name from 3-letter ISO 639 code
         */
        static QString languageName(const QString &code);

    private:
        static void init();

        static QHash<QString, QString> m_languages;
};

#endif // ISO639_H

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent

