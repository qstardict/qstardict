/*****************************************************************************
 * dictcore.h - QStarDict, a StarDict clone written with using Qt            *
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

#ifndef DICTCORE_H
#define DICTCORE_H

#include <QObject>

#include <QStringList>

namespace QStarDict
{

/**
 * The DictCore is a base dictionary class.
 */
class DictCore: public QObject
{
    Q_OBJECT

    public:
        /**
         * Construct dictionary.
         */
        DictCore(QObject *parent = 0);
        /**
         * Destructor.
         */
        ~DictCore();

        /**
         * Returns true if word is exists in dictionaries,
         * otherwise false.
         */
        bool isTranslatable(const QString &word);
        /**
         * Returns translation for word. If word not found, returns
         * "Not found!"
         */
        QString translate(const QString &word);
        /**
         * Returns a list of similar words contained in dictionaries.
         */
        QStringList findSimilarWords(const QString &word);
};

}

#endif // DICTCORE_H

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

